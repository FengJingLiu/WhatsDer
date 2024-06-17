#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_heap_trace.h"
#include "esp_heap_caps.h"

#include "string.h"
#include <stdio.h>
#include "esp_log.h"

#include "audio.h"
#include "../hal/hal.h"
#include "requests.h"

static char *TAG = "audio";

#define BUFFER_SIZE     (1024)

static char *recording_buffer;
static uint32_t buffer_size = 0;
static recoding_state recording_flag = done_stop;

static void record_start_task()
{
    char *recording_buffer_ptr;

    if (recording_buffer == NULL)
    {
        /* 1Mbs */
        ESP_LOGI(TAG, "recording_buffer == NULL, start heap_caps_malloc");
        recording_buffer = (char*)heap_caps_malloc(BUFFER_SIZE * BUFFER_SIZE, MALLOC_CAP_SPIRAM);
    }
    assert(recording_buffer != NULL);

    ESP_LOGI(TAG, "Recording start");
    hal_mic_open();

    recording_flag = is_recoding;
    buffer_size = 0;
    
    recording_buffer_ptr = recording_buffer;
    do{
        /* Read data from codec and write it to SPIFFS */
        hal_mic_read((void*)recording_buffer_ptr, BUFFER_SIZE);
        recording_buffer_ptr += 1024;
        buffer_size += 1024;
    } while (recording_flag == is_recoding && buffer_size < BUFFER_SIZE * BUFFER_SIZE);

    recording_flag = done_stop;
    ESP_LOGI(TAG, "Recording stop, length: %lu bytes", buffer_size);
    hal_mic_close();
    vTaskDelete(NULL);
}

void record_start()
{
    if (recording_flag == done_stop)
    {
        ESP_LOGI(TAG, "create record_start_task()");
        // record_start_task();
        xTaskCreate(&record_start_task, "record_start_task", 8096, NULL, 5, NULL);
    }
    else if (recording_flag == is_recoding)
    {
        ESP_LOGI(TAG, "record_start btn press stop");
        recording_flag = is_stoping;

        char* vop_rsp_buffer = (char*)heap_caps_malloc(2048, MALLOC_CAP_SPIRAM);
        assert(vop_rsp_buffer != NULL);
        req_vop(recording_buffer, vop_rsp_buffer, buffer_size);
        ESP_LOGI(TAG, "vop response is: %s", vop_rsp_buffer);

        char* gpt_rsp_buffer = (char*)heap_caps_malloc(2048, MALLOC_CAP_SPIRAM);
        req_gpt_text(vop_rsp_buffer, gpt_rsp_buffer, strlen(vop_rsp_buffer));
        ESP_LOGI(TAG, "gpt response is: %s", gpt_rsp_buffer);
        free(vop_rsp_buffer);
        free(gpt_rsp_buffer);
        /* hal_spk_open();
        int already_write = 0;
        char *recording_buffer_ptr = recording_buffer;
        while (buffer_size > already_write)
        {
            already_write += BUFFER_SIZE;
            hal_spk_play((void*) recording_buffer_ptr, ((buffer_size - already_write) > BUFFER_SIZE) ? BUFFER_SIZE : (buffer_size - already_write));
            recording_buffer_ptr += BUFFER_SIZE;
        }
        hal_spk_close(); */
    }
}

void record_stop(char **record_data, uint32_t *data_len)
{
    if (recording_flag != is_recoding)
    {
        return;
    }
    
    recording_flag = is_stoping;
    while (recording_flag != done_stop);
    
    *record_data = recording_buffer;
    *data_len = buffer_size;
}

recoding_state get_recording_flag()
{
    return recording_flag;
}


void record_and_play(void)
{
    #include "esp_timer.h"
    int16_t *recording_buffer = heap_caps_malloc(BUFFER_SIZE, MALLOC_CAP_DEFAULT);
    assert(recording_buffer != NULL);

    hal_mic_open();
    hal_spk_open();

    int time_1;
    int time_2;

    ESP_LOGI(TAG, "record_and_play start");
    while (1) {
        /* Read data from codec and write it to SPIFFS */
        time_1 = esp_timer_get_time();
        hal_mic_read(recording_buffer, BUFFER_SIZE);
        time_2 = esp_timer_get_time();
        ESP_LOGI(TAG, "record remain time :%d", time_2 - time_1);
        hal_spk_play(recording_buffer, BUFFER_SIZE);
        time_1 = esp_timer_get_time();
        ESP_LOGI(TAG, "speaker_play remain time :%d", time_1 - time_2);
    }
}
