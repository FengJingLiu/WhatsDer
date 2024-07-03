#if 0
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_timer.h"
#include "lvgl.h"

#include "hal.h"
#include "err_check.h"
#include "config.h"
#include "lvgl_port.h"

#define LVGL_TICK_PERIOD_MS         (CONFIG_BSP_DISPLAY_LVGL_TICK)
#define LVGL_BUFFER_MALLOC          (MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT)

static const char *TAG = "lvgl_port";
static SemaphoreHandle_t lvgl_mux;                  // LVGL mutex
static TaskHandle_t lvgl_task_handle = NULL;

void flush_callback(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map)
{
    esp_lcd_panel_handle_t panel_handle = (esp_lcd_panel_handle_t) drv->user_data;
    const int offsetx1 = area->x1;
    const int offsetx2 = area->x2;
    const int offsety1 = area->y1;
    const int offsety2 = area->y2;

    /* Just copy data from the color map to the RGB frame buffer */
    esp_lcd_panel_draw_bitmap(panel_handle, offsetx1, offsety1, offsetx2 + 1, offsety2 + 1, color_map);

    lv_disp_flush_ready(drv);
}

static void update_callback(lv_disp_drv_t *drv)
{
    esp_lcd_panel_handle_t panel_handle = (esp_lcd_panel_handle_t) drv->user_data;

    switch (drv->rotated) {
    case LV_DISP_ROT_NONE:
        esp_lcd_panel_swap_xy(panel_handle, false);
        esp_lcd_panel_mirror(panel_handle, false, false);
        break;
    case LV_DISP_ROT_90:
        esp_lcd_panel_swap_xy(panel_handle, true);
        esp_lcd_panel_mirror(panel_handle, false, true);
        break;
    case LV_DISP_ROT_180:
        esp_lcd_panel_swap_xy(panel_handle, false);
        esp_lcd_panel_mirror(panel_handle, true, true);
        break;
    case LV_DISP_ROT_270:
        esp_lcd_panel_swap_xy(panel_handle, true);
        esp_lcd_panel_mirror(panel_handle, true, false);
        break;
    }
}

static lv_disp_t *display_init(esp_lcd_panel_handle_t lcd)
{
    BSP_NULL_CHECK(lcd, NULL);

    static lv_disp_draw_buf_t disp_buf = { 0 };     // Contains internal graphic buffer(s) called draw buffer(s)
    static lv_disp_drv_t disp_drv = { 0 };          // Contains LCD panel handle and callback functions

    // alloc draw buffers used by LVGL
    void *buf1 = NULL;
    void *buf2 = NULL;
    int buffer_size = 0;

    ESP_LOGD(TAG, "Malloc memory for LVGL buffer");

    // Normmaly, for RGB LCD, we just use one buffer for LVGL rendering
    buffer_size = CONFIG_LCD_H_RES * CONFIG_LVGL_BUFFER_HEIGHT;
    buf1 = heap_caps_malloc(buffer_size * sizeof(lv_color_t), LVGL_BUFFER_MALLOC);
    BSP_NULL_CHECK(buf1, NULL);
    ESP_LOGI(TAG, "LVGL buffer size: %dKB", buffer_size * sizeof(lv_color_t) / 1024);


    // initialize LVGL draw buffers
    lv_disp_draw_buf_init(&disp_buf, buf1, buf2, buffer_size);

    ESP_LOGD(TAG, "Register display driver to LVGL");
    lv_disp_drv_init(&disp_drv);

    disp_drv.hor_res = CONFIG_LCD_H_RES;
    disp_drv.ver_res = CONFIG_LCD_V_RES;

    disp_drv.flush_cb = flush_callback;
    disp_drv.drv_update_cb = update_callback;
    disp_drv.draw_buf = &disp_buf;
    disp_drv.user_data = lcd;
    disp_drv.full_refresh = 1;
    disp_drv.direct_mode = 1;
    return lv_disp_drv_register(&disp_drv);
}

static void tick_increment(void *arg)
{
    /* Tell LVGL how many milliseconds have elapsed */
    lv_tick_inc(LVGL_TICK_PERIOD_MS);
}

static esp_err_t tick_init(void)
{
    // Tick interface for LVGL (using esp_timer to generate 2ms periodic event)
    const esp_timer_create_args_t lvgl_tick_timer_args = {
        .callback = &tick_increment,
        .name = "LVGL tick"
    };
    esp_timer_handle_t lvgl_tick_timer = NULL;
    BSP_ERROR_CHECK_RETURN_ERR(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer));
    return esp_timer_start_periodic(lvgl_tick_timer, LVGL_TICK_PERIOD_MS * 1000);
}

static void lvgl_port_task(void *arg)
{
    ESP_LOGI(TAG, "Starting LVGL task");
    while (1) {
        lvgl_port_lock(0);
        uint32_t task_delay_ms = lv_timer_handler();
        lvgl_port_unlock();
        if (task_delay_ms > 500) {
            task_delay_ms = 500;
        } else if (task_delay_ms < CONFIG_BSP_DISPLAY_LVGL_TASK_DELAY) {
            task_delay_ms = CONFIG_BSP_DISPLAY_LVGL_TASK_DELAY;
        }
        vTaskDelay(pdMS_TO_TICKS(task_delay_ms));
    }
}

esp_err_t port_lvgl_init(esp_lcd_panel_handle_t lcd, lv_disp_t **disp)
{
    BSP_NULL_CHECK(lcd, ESP_ERR_INVALID_ARG);
    BSP_NULL_CHECK(disp, ESP_ERR_INVALID_ARG);

    lv_init();
    BSP_ERROR_CHECK_RETURN_ERR(tick_init());
    BSP_NULL_CHECK(*disp = display_init(lcd), ESP_FAIL);

    lvgl_mux = xSemaphoreCreateRecursiveMutex();
    BSP_NULL_CHECK(lvgl_mux, ESP_FAIL);
    ESP_LOGI(TAG, "Create LVGL task");
    BaseType_t core_id = (CONFIG_BSP_DISPLAY_LVGL_TASK_CORE_ID < 0) ? tskNO_AFFINITY : CONFIG_BSP_DISPLAY_LVGL_TASK_CORE_ID;
    BaseType_t ret = xTaskCreatePinnedToCore(
                         lvgl_port_task, "LVGL", CONFIG_BSP_DISPLAY_LVGL_TASK_STACK_SIZE_KB * 1024, NULL,
                         CONFIG_BSP_DISPLAY_LVGL_TASK_PRIORITY, &lvgl_task_handle, core_id
                     );
    if (ret != pdPASS) {
        ESP_LOGE(TAG, "Failed to create LVGL task");
        return ESP_FAIL;
    }

    return ESP_OK;
}

bool lvgl_port_lock(uint32_t timeout_ms)
{
    assert(lvgl_mux && "bsp_lvgl_port_init must be called first");

    const TickType_t timeout_ticks = (timeout_ms == 0) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
    return xSemaphoreTakeRecursive(lvgl_mux, timeout_ticks) == pdTRUE;
}

void lvgl_port_unlock(void)
{
    assert(lvgl_mux && "bsp_lvgl_port_init must be called first");
    xSemaphoreGiveRecursive(lvgl_mux);
}
#endif
