#include "hal.h"

#include <stdio.h>
#include <string.h>

#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "unity.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_commands.h"
#include "esp_lvgl_port.h"
#include "esp_err.h"
#include "err_check.h"
#include "../common/config.h"


#define RGB565_Blue 0x001F
#define RGB565_Green 0x07E0
#define RGB565_Red 0xF800

lv_disp_t *hal_screen_init()
{
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_handle_t panel_handle = NULL;
    lv_disp_t *disp = NULL;

    const lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
    BSP_ERROR_CHECK_RETURN_NULL(lvgl_port_init(&lvgl_cfg));

    // turn off backlight
    gpio_config_t bk_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << CONFIG_LCD_BK_LIGHT_GPIO
    };
    ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));
    gpio_set_level(CONFIG_LCD_BK_LIGHT_GPIO, 1);

    spi_bus_config_t buscfg = {
        .sclk_io_num = CONFIG_LCD_PCLK_GPIO,
        .mosi_io_num = CONFIG_LCD_DATA0_GPIO,
        .miso_io_num = -1,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 100 * 100 * sizeof(uint16_t),
    };

    ESP_ERROR_CHECK(spi_bus_initialize(1, &buscfg, SPI_DMA_CH_AUTO));

    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = CONFIG_LCD_DC_GPIO,
        .cs_gpio_num = CONFIG_LCD_CS_GPIO,
        .pclk_hz = 80 * 1000 * 1000,
        .spi_mode = 0,
        .trans_queue_depth = 10,
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
        .on_color_trans_done = NULL,
        .user_ctx = NULL,
    };

    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)1, &io_config, &io_handle));

    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = CONFIG_LCD_RES_GPIO,
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
        .data_endian = LCD_RGB_DATA_ENDIAN_LITTLE,
        .bits_per_pixel = 16,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_handle, &panel_config, &panel_handle));

    esp_lcd_panel_reset(panel_handle);
    esp_lcd_panel_init(panel_handle);
    esp_lcd_panel_mirror(panel_handle, false, false);
    esp_lcd_panel_disp_on_off(panel_handle, true);
    esp_lcd_panel_invert_color(panel_handle, true);

    /* Add LCD screen */
    const lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = io_handle,
        .panel_handle = panel_handle,
        .buffer_size = 240*60,
        .double_buffer = false,
        .hres = 240,
        .vres = 240,
        .monochrome = false,
        .rotation = {
            .swap_xy = false,
            .mirror_x = false,
            .mirror_y = false,
        },
        .flags = {
            .buff_dma = true,
            .buff_spiram = false // spiram 不支持dma传输
        }
    };
    disp = lvgl_port_add_disp(&disp_cfg);

    encode_init(disp);

    return disp;
}
