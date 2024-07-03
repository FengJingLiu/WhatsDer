#include "esp_event.h"
#include "recoder.hpp"

#include "../hal/hal.h"
#include "esp_log.h"
#include "request.hpp"
#include "event_declare.h"

#define TAG "recoder"

static void record_task(void *pvParameters)
{
    ESP_LOGI(TAG, "Recording start");
    hal_mic_open();
    pcm_data_t pcm_data;
    size_t     total_size = 0;

    xQueueReset(recoder::Recoder::getInstance().recoder_queue);
    while (!(recoder::Recoder::getInstance().is_recoder_stop()))
    {
        pcm_data.data = heap_caps_malloc(RECODER_BATCH_SIZE, MALLOC_CAP_SPIRAM);
        assert(pcm_data.data != nullptr);
        pcm_data.data_size = RECODER_BATCH_SIZE;

        hal_mic_read((void *)pcm_data.data, RECODER_BATCH_SIZE);
        total_size += RECODER_BATCH_SIZE;
        xQueueSend(recoder::Recoder::getInstance().recoder_queue, &pcm_data, portMAX_DELAY);
        // ESP_LOGI(TAG, "record_task while loop");
    }

    ESP_LOGI(TAG, "Recording stop, length: %zu bytes", total_size);
    hal_mic_close();
    vTaskDelete(NULL);
}

static void recoder_event_handler(void *handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    (void)handler_arg;
    (void)event_data;

    if (event_id == EVENT_RECODER_START)
    {
        ESP_LOGI(TAG, "receive EVENT_START!");
        esp_event_post(APP_WEBSOCKET_EVENTS, EVENT_RECORDR_START, NULL, 0, portMAX_DELAY);
        recoder::Recoder::getInstance().start();
    }
    else if (event_id == EVENT_RECODER_STOP)
    {
        ESP_LOGI(TAG, "receive EVENT_STOP!");
        esp_event_post(APP_WEBSOCKET_EVENTS, EVENT_RECORDR_STOP, NULL, 0, portMAX_DELAY);
        recoder::Recoder::getInstance().stop();
    }
}

recoder::Recoder::Recoder() : state(STOP)
{
    this->recoder_queue = xQueueCreate(RECODER_BUFFER_SIZE, sizeof(pcm_data_t));
    if (this->recoder_queue == nullptr)
    {
        ESP_LOGE(TAG, "recoder task create error!");
    }

    ESP_ERROR_CHECK(esp_event_handler_register(APP_RECODER_EVENTS, ESP_EVENT_ANY_ID, recoder_event_handler, nullptr));
}

void recoder::Recoder::start()
{
    if (this->state == RUNNING)
        return;

    this->state = RUNNING;
    if (xTaskCreate(record_task, "recoder", 8192, this, 2, this->task_handle) != pdPASS)
    {
        ESP_LOGE(TAG, "recoder task create error!");
        this->state = STOP;
        return;
    }
}

void recoder::Recoder::stop() { this->state = STOP; }

bool recoder::Recoder::is_recoder_stop() { return this->state == STOP; }
