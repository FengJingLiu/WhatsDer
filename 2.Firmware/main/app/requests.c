#include "requests.h"

#include <string.h>
#include <inttypes.h> 
#include <cJSON.h>

#include "esp_http_client.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_check.h"
#include "esp_system.h"
#include "../common/password.h"

#define MAX_HTTP_OUTPUT_BUFFER 2048
static const char *TAG = "HTTP_REQUEST";

/* TODO token 一个月有效期，先完成其他的 requests */
static esp_err_t req_get_baidu_token()
{
    esp_err_t ret = ESP_OK;

    return ret;
}

/* 解析百度语音转文字的 rsp json, 解析之后直接保存在输入的 json_data 地址 */
static esp_err_t decode_vop_rsp(char* json_data)
{
    // 解析 JSON 字符串
    cJSON *root = cJSON_Parse(json_data);
    if (root == NULL) {
        ESP_LOGE(TAG, "Error parsing JSON\n");
        return ESP_FAIL;
    }

    // 获取 "err_no" 字段
    cJSON *err_no = cJSON_GetObjectItem(root, "err_no");
    if (err_no == NULL || !cJSON_IsNumber(err_no)) {
        ESP_LOGE(TAG, "Error finding 'err_no' field or it's not a number\n");
        cJSON_Delete(root);
        return ESP_FAIL;
    }

    // 检查 err_no 是否等于 0
    if (err_no->valueint == 0) {
        // 获取并打印 "result" 数组
        cJSON *result = cJSON_GetObjectItem(root, "result");
        if (result != NULL && cJSON_IsArray(result)) {
            cJSON *item = NULL;
            cJSON_ArrayForEach(item, result) {
                if (cJSON_IsString(item)) {
                    ESP_LOGI(TAG, "result: %s ", item->valuestring);
                    strcpy(json_data, item->valuestring);
                }
            }
        } else {
            ESP_LOGE(TAG, "Error finding 'result' field or it's not an array\n");
            cJSON_Delete(root);
            return ESP_FAIL;
        }
    } else {
        ESP_LOGE(TAG, "err_no is not 0, skipping 'result' extraction\n");
        cJSON_Delete(root);
        return ESP_FAIL;
    }

    // 释放 JSON 对象
    cJSON_Delete(root);

    return ESP_OK;
}

/* 用 esp_http_client_perform 在 response chunked 的时候有坑，改用 native 方式请求 */
esp_err_t req_vop(char* pcm_data, char* rsp_data, size_t size)
{
    esp_err_t ret = ESP_OK;
    int content_length = 0;
    char query[] = "cuid=" VOP_CLIENT_ID "&token=" VOP_TOLEN "&dev_pid=80001";

    esp_http_client_config_t config = {
        .host = "vop.baidu.com",
        .path = "/pro_api",
        .query = query,
        .method = HTTP_METHOD_POST, 
        .disable_auto_redirect = true,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_http_client_set_header(client, "Content-Type", "audio/pcm;rate=16000");
    ret |= esp_http_client_open(client, size);
    if (ret == ESP_OK) {
        int wlen = esp_http_client_write(client, pcm_data, size);
        if (wlen < 0) {
            ESP_LOGE(TAG, "Write failed");
        }
        content_length = esp_http_client_fetch_headers(client);
        if (content_length < 0) {
            ESP_LOGE(TAG, "HTTP client fetch headers failed");
        } else {
            int data_read = esp_http_client_read_response(client, rsp_data, MAX_HTTP_OUTPUT_BUFFER);
            rsp_data[data_read] = '\0';
            if (data_read >= 0) {
                ESP_LOGI(TAG, "HTTP POST Status = %d, content_length = %"PRId64,
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
                // ESP_LOG_BUFFER_HEX(TAG, rsp_data, data_read);

                ret |= decode_vop_rsp(rsp_data);
            } else {
                ESP_LOGE(TAG, "Failed to read response");
            }
        }
    } else {
        ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(ret));
    }
    ret |= esp_http_client_close(client);
    ret |= esp_http_client_cleanup(client);
    return ret;
}

static int encode_json_str_gpt_text(char* question, char* json_str)
{
    // 创建 JSON 对象
    cJSON* root = cJSON_CreateObject();

    // 添加 "model" 字段
    cJSON_AddStringToObject(root, "model", "gpt-3.5-turbo");

    // 创建 "messages" 数组
    cJSON* messages = cJSON_AddArrayToObject(root, "messages");

    // 添加第一个消息对象
    cJSON* message1 = cJSON_CreateObject();
    cJSON_AddStringToObject(message1, "role", "system");
    cJSON_AddStringToObject(message1, "content", "用中文回答我的问题.");
    cJSON_AddItemToArray(messages, message1);

    // 添加第二个消息对象
    cJSON* message2 = cJSON_CreateObject();
    cJSON_AddStringToObject(message2, "role", "user");
    cJSON_AddStringToObject(message2, "content", question);
    cJSON_AddItemToArray(messages, message2);

    // 将 JSON 对象转换为字符串
    char* string = cJSON_PrintUnformatted(root);
    int str_len = strlen(string);
    strcpy(json_str, string);
    // 打印 JSON 字符串
    ESP_LOGI(TAG, "gen_json_gpt_text json_str : %s", string);

    // 释放内存
    cJSON_Delete(root);
    return str_len;
}

/* 解析 chatgpt 的 rsp json, 解析之后直接保存在输入的 json_data 地址 */
static esp_err_t decode_gpt_rsp(char* json_data)
{
    // 解析 JSON 字符串
    cJSON *root = cJSON_Parse(json_data);
    if (root == NULL) {
        ESP_LOGE(TAG, "Error parsing JSON");
        return ESP_FAIL;
    }

    // 导航到 "choices" 数组
    cJSON *choices = cJSON_GetObjectItem(root, "choices");
    if (choices == NULL || !cJSON_IsArray(choices)) {
        cJSON_Delete(root);
        ESP_LOGE(TAG, "Error finding 'choices' array");
        return ESP_FAIL;
    }

    // 获取数组中的第一个元素
    cJSON *choice = cJSON_GetArrayItem(choices, 0);
    if (choice == NULL) {
        cJSON_Delete(root);
        ESP_LOGE(TAG, "Error getting first element of 'choices' array");
        return ESP_FAIL;
    }

    // 获取 "message" 对象
    cJSON *message = cJSON_GetObjectItem(choice, "message");
    if (message == NULL) {
        cJSON_Delete(root);
        ESP_LOGE(TAG, "Error finding 'message' object");
        return ESP_FAIL;
    }

    // 获取 "content" 字段
    cJSON *content = cJSON_GetObjectItem(message, "content");
    if (content == NULL || !cJSON_IsString(content)) {
        cJSON_Delete(root);
        ESP_LOGE(TAG, "Error finding 'content' field");
        return ESP_FAIL;
    }

    // 打印 content 内容
    strcpy(json_data, content->valuestring);
    ESP_LOGI(TAG, "Message content: %s", json_data);

    // 释放 JSON 对象
    cJSON_Delete(root);

    return ESP_OK;
}

esp_err_t req_gpt_text(char* question, char* rsp_data, int size)
{
    // Declare local_response_buffer with size (MAX_HTTP_OUTPUT_BUFFER + 1) to prevent out of bound access when
    // it is used by functions like strlen(). The buffer should only be used upto size MAX_HTTP_OUTPUT_BUFFER
    esp_err_t ret = ESP_OK;
    int content_length = 0;
    esp_http_client_config_t config = {
        .url = "http://ai-yyds.com/v1/chat/completions",
        .method = HTTP_METHOD_POST, 
        .disable_auto_redirect = true,
    };

    // char post_data[512];
    int post_data_len = encode_json_str_gpt_text(question, question);
    ESP_LOGI(TAG, "post_data : %s data_len: %d post_data_len: %d", question, strlen(question), post_data_len);

    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_header(client, "Accept", "application/json");
    esp_http_client_set_header(client, "Authorization", CONFIG_YYDS_API_KEY);

    ret |= esp_http_client_open(client, post_data_len);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(ret));
    } else {
        int wlen = esp_http_client_write(client, question, post_data_len);
        if (wlen < 0) {
            ESP_LOGE(TAG, "Write failed");
        }
        content_length = esp_http_client_fetch_headers(client);
        if (content_length < 0) {
            ret = ESP_FAIL;
            ESP_LOGE(TAG, "HTTP client fetch headers failed");
        } else {
            int data_read = esp_http_client_read_response(client, rsp_data, MAX_HTTP_OUTPUT_BUFFER);
            if (data_read >= 0) {
                ESP_LOGI(TAG, "HTTP POST Status = %d, content_length = %"PRId64,
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
                ret |= decode_gpt_rsp(rsp_data);
            } else {
                ret = ESP_FAIL;
                ESP_LOGE(TAG, "Failed to read response");
            }
        }
    }
    esp_http_client_cleanup(client);
    return ret;
}