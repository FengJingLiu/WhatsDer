#include "../common/config.h"
#include "cJSON.h"
#include "esp_debug_helpers.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "esp_websocket_client.h"
#include "event_declare.h"
#include <string.h>
#include "recoder.hpp"
#include "request.hpp"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>

#define TAG                                    "websocket"
#define MESSAGE_BASE_SIZE                      128
#define USAGE_SPACE(TOTAL_SPACE, REMAIN_SPACE) (TOTAL_SPACE - REMAIN_SPACE - 1)

request::Message::Message() : done_flag(0), msg_id(0)
{
    memset(fix_message, 0, 512);
    memset(unfix_message, 0, 256);

    mutex = xSemaphoreCreateRecursiveMutex();

    assert(mutex != nullptr);
}

request::Message::Message(message_role_t role) : Message() { this->role = role; }

request::Message::Message(message_role_t role, char *str, size_t str_len) : Message(role)
{
    this->done_message_content(str, str_len);
}

request::Message::Message(message_role_t role, char *str, size_t str_len, char *record_uuid)
    : Message(role, str, str_len)
{
    this->add_recode(record_uuid);
}

void request::Message::set_role(message_role_t role)
{
    this->lock();
    this->role = role;
    this->unlock();
}

void request::Message::add_recode(char *uuid)
{
    this->lock();
    memcpy(record_uuid, uuid, 36);
    record_uuid[36] = '\0';
    this->unlock();
}

void request::Message::update_message_content(const char *str, size_t str_len)
{
    this->lock();
    memcpy(this->unfix_message, str, str_len + 1);
    ESP_LOGI(TAG, "after unfix_message string: %s", this->unfix_message);
    this->unlock();
}

void request::Message::done_message_content(const char *str, size_t str_len)
{
    this->lock();
    memcpy(this->fix_message + strlen(this->fix_message), str, str_len + 1);
    ESP_LOGI(TAG, "after fix_message string: %s", this->fix_message);
    this->unlock();
}

char *request::Message::get_message_content()
{
    this->lock();
    // ESP_LOGI(TAG, "this->get_done_flag() : %d", this->get_done_flag());
    // ESP_LOGI(TAG, "this->done_flag : %d", this->done_flag);
    if (this->done_flag)
    {
        return this->fix_message;
    }
    memcpy(this->fix_message + strlen(this->fix_message), this->unfix_message, strlen(this->unfix_message) + 1);

    this->unlock();

    return this->fix_message;
}

void request::Message::all_done()
{
    this->lock();

    this->done_flag = true;
    memcpy(this->fix_message + strlen(this->fix_message), this->unfix_message, strlen(this->unfix_message) + 1);

    this->unlock();
}

// update_message_timer 拿锁的时候莫名bug 先取消互斥锁
void request::Message::lock()
{
    // ESP_LOGI("mutex", "lock in ");
    // ESP_LOGI(TAG, "uxSemaphoreGetCount: %d", uxSemaphoreGetCount(mutex));
    // esp_backtrace_print(20);
    
    // xSemaphoreTakeRecursive(mutex, portMAX_DELAY);
}

void request::Message::unlock()
{
    // ESP_LOGI("mutex", "unlock");
    // ESP_LOGI(TAG, "uxSemaphoreGetCount: %d", uxSemaphoreGetCount(mutex));
    // esp_backtrace_print(20);
    // xSemaphoreGiveRecursive(mutex);
}

static void
websocket_state_event_handler(void *handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_id == EVENT_SOCKET_OPEN)
    {
        ESP_LOGI(TAG, "receive EVENT_SOCKET_OPEN!");
        request::Websocket::getInstance().open();
    }
    else if (event_id == EVENT_SOCKET_CLOSE)
    {
        ESP_LOGI(TAG, "receive EVENT_SOCKET_CLOSE!");
        request::Websocket::getInstance().close();
    }
    else if (event_id == EVENT_RECORDR_START)
    {
        ESP_LOGI(TAG, "receive EVENT_RECORDR_START!");
        request::Websocket::getInstance().recoder_start();
    }
    else if (event_id == EVENT_RECORDR_STOP)
    {
        ESP_LOGI(TAG, "receive EVENT_RECORDR_STOP!");
        request::Websocket::getInstance().recoder_end();
    }
}

static void websocket_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_websocket_event_data_t *data = (esp_websocket_event_data_t *)event_data;
    switch (event_id)
    {
    case WEBSOCKET_EVENT_CONNECTED:
        ESP_LOGI(TAG, "websocket_event_handler WEBSOCKET_EVENT_CONNECTED");
        break;
    case WEBSOCKET_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "websocket_event_handler WEBSOCKET_EVENT_DISCONNECTED");
        break;
    case WEBSOCKET_EVENT_DATA:
        ESP_LOGI(TAG, "websocket_event_handler WEBSOCKET_EVENT_DATA");
        ESP_LOGI(TAG, "Received opcode=%d", data->op_code);
        if (data->op_code == 0x08 && data->data_len == 2)
        {
            ESP_LOGW(TAG, "Received closed message with code=%d", 256 * data->data_ptr[0] + data->data_ptr[1]);
        }
        else if (data->op_code == 0x09)  // ping操作
        {
            ESP_LOGI(TAG, "Received ping reply pong");
            esp_websocket_client_send_with_opcode(
                request::Websocket::getInstance().client, WS_TRANSPORT_OPCODES_PONG, NULL, 0, portMAX_DELAY);
        }
        else if (data->op_code == 0x01)  // 文本帧
        {
            request::Websocket::getInstance().update_cur_msg(data->data_ptr, data->data_len);
            ESP_LOGI(TAG, "Received=%.*s\n\n", data->data_len, (char *)data->data_ptr);
        }
        else if (data->op_code == 0x02)  // bytes帧
        {
        }

        ESP_LOGD(TAG,
                 "Total payload length=%d, data_len=%d, current payload "
                 "offset=%d\r\n",
                 data->payload_len,
                 data->data_len,
                 data->payload_offset);

        break;
    case WEBSOCKET_EVENT_ERROR:
        ESP_LOGI(TAG, "websocket_event_handler WEBSOCKET_EVENT_ERROR");
        break;
    }
}

static void update_message_timer(TimerHandle_t xTimer)
{
    if (request::Websocket::getInstance().get_update_message_flag()) {
        ESP_LOGI(TAG, "update_message_timer");
        ESP_LOGI(TAG, "get_done_flag: %d", request::Websocket::getInstance().get_cur_msg()->get_done_flag());
        
        esp_event_post(APP_DISPLAY_EVENTS, EVENT_UPDATE_MESSAGE, NULL, 0, portMAX_DELAY);
        request::Websocket::getInstance().clear_update_message_flag();
    }
}

request::Websocket::Websocket() : state(CLOSE)
{
    this->recoder_queue = recoder::Recoder::getInstance().recoder_queue;

    esp_event_handler_register(APP_WEBSOCKET_EVENTS, ESP_EVENT_ANY_ID, websocket_state_event_handler, nullptr);

    esp_websocket_client_config_t websocket_cfg = {};

    websocket_cfg.uri                  = CONFIG_WEBSOCKET_URI;
    websocket_cfg.reconnect_timeout_ms = 1000;

    ESP_LOGI(TAG, "Websocket init");

    this->client = esp_websocket_client_init(&websocket_cfg);
    esp_websocket_register_events(this->client, WEBSOCKET_EVENT_ANY, websocket_event_handler, (void *)client);
    update_message_timer_handle =
        xTimerCreate("update_message_timer", pdMS_TO_TICKS(500), pdTRUE, NULL, update_message_timer);
}

void request::Websocket::send_pcm_task(void *pvParameters)
{
    ESP_LOGI(TAG, "send_pcm_task start");
    Websocket &websocket = request::Websocket::getInstance();
    pcm_data_t pcm_data  = {.data = nullptr, .data_size = 0};
    int        t;
    while (websocket.state == OPEN)
    {
        // ESP_LOGI(TAG, "QueueMessagesWaiting :%d", uxQueueMessagesWaiting(websocket.recoder_queue));
        if (xQueueReceive(websocket.recoder_queue, &pcm_data, 100) == pdPASS)
        {
            t = esp_timer_get_time();
            esp_websocket_client_send_bin(websocket.client, (char *)pcm_data.data, pcm_data.data_size, portMAX_DELAY);
            ESP_LOGD(TAG, "send bin cost time :%lld", esp_timer_get_time() - t);
            free(pcm_data.data);
        }
    }
    vTaskDelete(NULL);
    ESP_LOGI(TAG, "send_pcm_task end");
}

void request::Websocket::open()
{
    if (esp_websocket_client_is_connected(this->client))
    {
        return;
    }
    this->state = OPENING;
    ESP_LOGI(TAG, "Websocket open");
    esp_websocket_client_start(this->client);
    this->state = OPEN;
}

void request::Websocket::close()
{
    if (!esp_websocket_client_is_connected(this->client))
    {
        return;
    }
    this->state = OPENING;
    ESP_LOGI(TAG, "Websocket close");
    esp_websocket_client_close(this->client, portMAX_DELAY);
    this->state = CLOSE;
}

void request::Websocket::recoder_start()
{
    while (this->state == OPENING)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    if (this->state == CLOSE)
    {
        this->open();
    }
    this->recorder_state = OPEN;
    this->cur_msg        = new Message(USER_ROLE);
    ESP_LOGI(TAG, "new Message done_flag: %d", cur_msg->get_done_flag());
    // ESP_LOGI(TAG, "recoder_start cur_msg address %p", this->cur_msg);
    event_data_wrap_t event_data_post = {.ptr = this->cur_msg};
    esp_event_post(APP_DISPLAY_EVENTS, EVENT_CREATE_MESSAGE, &event_data_post, sizeof(event_data_wrap_t), portMAX_DELAY);
    char data[] = "{\"action\":\"ASR_START\"}";
    esp_websocket_client_send_text(this->client, data, sizeof(data), portMAX_DELAY);
    xTaskCreate(this->send_pcm_task, "send_pcm_task", 8192, this->client, 1, &send_pcm_task_handle);
    xTimerStart(update_message_timer_handle, 0);
}

void request::Websocket::recoder_end()
{
    // 等待队列中数据传输完毕
    this->recorder_state = CLOSE;

    char data[] = "{\"action\":\"ASR_STOP\"}";
    ESP_LOGI(TAG, "recoder_end data: %s", data);

    esp_websocket_client_send_text(this->client, data, sizeof(data), portMAX_DELAY);

    this->cur_msg->all_done();

    char openai_json[200];
    snprintf(openai_json,
             200,
             "{\"action\":\"OPENAI_ASSISTANT\", \"message\":\"%s\"}",
             this->cur_msg->get_message_content());

    ESP_LOGI(TAG, "openai_json: %s", openai_json);
    esp_websocket_client_send_text(this->client, data, sizeof(data), portMAX_DELAY);
    xTimerStop(update_message_timer_handle, 0);
}

/*
{"log_id":2105566789,"type":"MID_TEXT","result":"被海水冲上岸的干芦荟叶子这画风看起来就像还活着的某种惊悚海底生"}
{"log_id":2105566789,"type":"FIN_TEXT","result":"被海水冲上岸的干芦荟叶子，这画风看起来就像还活着的某种惊悚海底生物。"}
*/
void request::Websocket::update_cur_msg(const char *new_data, size_t new_data_size)
{
    cJSON *root = cJSON_ParseWithLength(new_data, new_data_size);
    if (root == NULL)
    {
        ESP_LOGI(TAG, "Error parsing JSON\n");
        return;
    }

    uint32_t log_id = cJSON_GetObjectItemCaseSensitive(root, "log_id")->valueint;
    char    *type   = cJSON_GetObjectItemCaseSensitive(root, "type")->valuestring;
    char    *result = cJSON_GetObjectItemCaseSensitive(root, "result")->valuestring;
    if (strcmp(type, "MID_TEXT") == 0)
    {
        this->cur_msg->add_msg_id(log_id);
        this->cur_msg->update_message_content(result, strlen(result));
    }
    else if (strcmp(type, "FIN_TEXT") == 0)
    {
        this->cur_msg->done_message_content(result, strlen(result));
    }
    else if (strcmp(type, "OPENAI_DATA") == 0)
    {
        this->cur_msg->done_message_content(result, strlen(result));
    }
    else if (strcmp(type, "OPENAI_START") == 0)
    {
        void *msg_ptr = heap_caps_malloc(sizeof(Message), MALLOC_CAP_SPIRAM);
        this->cur_msg = new (msg_ptr) Message(ASSISTANT_ROLE);
        esp_event_post(APP_DISPLAY_EVENTS, EVENT_CREATE_MESSAGE, this->cur_msg, sizeof(Message), portMAX_DELAY);
    }
    else if (strcmp(type, "OPENAI_END") == 0)
    {
        this->cur_msg->all_done();
    }
    cJSON_Delete(root);
    this->update_message_flag = 1;
    // esp_event_post(APP_DISPLAY_EVENTS, EVENT_UPDATE_MESSAGE, NULL, 0, portMAX_DELAY);
}
