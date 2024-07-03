#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

#include "hal/hal.h"
#include "esp_log.h"
#include "../app/app.hpp"
#include "../app/request.hpp"

#define LOG_MEM_INFO    (0)

static void print_memory_info()
{
#if LOG_MEM_INFO
    static char buffer[128];    /* Make sure buffer is enough for `sprintf` */
    sprintf(buffer, "   Biggest /     Free /    Total\n"
            "\t  SRAM : [%8d / %8d / %8d]\n"
            "\t PSRAM : [%8d / %8d / %8d]",
            heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL) / 1024,
            heap_caps_get_free_size(MALLOC_CAP_INTERNAL) / 1024,
            heap_caps_get_total_size(MALLOC_CAP_INTERNAL) / 1024,
            heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM) / 1024,
            heap_caps_get_free_size(MALLOC_CAP_SPIRAM) / 1024,
            heap_caps_get_total_size(MALLOC_CAP_SPIRAM) / 1024);
    ESP_LOGI("MEM", "%s", buffer);

    vTaskDelay(pdMS_TO_TICKS(500));
#endif
}

extern "C" void app_main(void)
{
    hal_all_init();
    app::app_init();

    // record_and_play();
    // button_init(0);

    while (true)
    {
        // hal_cam_capture();

        print_memory_info();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}