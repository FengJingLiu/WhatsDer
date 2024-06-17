#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

#include "hal/hal.h"
// #include "gui_guider.h"
#include "../app/button.h"
#include "../app/audio.h"

// static char *TAG = "app_main";

// lv_ui guider_ui;
// void gui_init(void)
// {
//     /* Initialize display and LVGL */
//     bsp_display_start();

//     /* Set display brightness to 100% */
//     bsp_display_backlight_on();

//     void events_init(lv_ui *ui);

//     void events_init_screen(lv_ui *ui);

//     ESP_LOGI(TAG, "Display LVGL demo");
//     bsp_display_lock(0);
//     setup_ui(&guider_ui);
//     events_init(&guider_ui);
//     bsp_display_unlock();
// }

void app_main(void)
{
    hal_all_init();
    // record_and_play();
    button_init(0);

#if LOG_MEM_INFO
    static char buffer[128];    /* Make sure buffer is enough for `sprintf` */
    while (1) {
        sprintf(buffer, "   Biggest /     Free /    Total\n"
                "\t  SRAM : [%8d / %8d / %8d]\n"
                "\t PSRAM : [%8d / %8d / %8d]",
                heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL),
                heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
                heap_caps_get_total_size(MALLOC_CAP_INTERNAL),
                heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM),
                heap_caps_get_free_size(MALLOC_CAP_SPIRAM),
                heap_caps_get_total_size(MALLOC_CAP_SPIRAM));
        ESP_LOGI("MEM", "%s", buffer);

        vTaskDelay(pdMS_TO_TICKS(500));
    }
#endif
}