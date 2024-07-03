#include "hal.h"
#include "esp_lvgl_port.h"
#include "esp_log.h"
#include "lvgl.h"
#include "ui.h"
#include "../common/config.h"

#define TAG "HAL_ENCODE"

lv_indev_t* encoder_handle = NULL;

lv_indev_t *encode_init(lv_display_t *disp_handle)
{
    
    const button_config_t encoder_btn_config = {
        .type = BUTTON_TYPE_GPIO,
        .gpio_button_config.active_level = true,
        .gpio_button_config.gpio_num = CONFIG_ENCODE_BTN,
    };

    const knob_config_t encoder_a_b_config = {
        .default_direction = 0,
        .gpio_encoder_a = CONFIG_ENCODE_A,
        .gpio_encoder_b = CONFIG_ENCODE_B,
    };

    /* Encoder configuration structure */
    const lvgl_port_encoder_cfg_t encoder = {
        .disp = disp_handle,
        .encoder_a_b = &encoder_a_b_config,
        .encoder_enter = &encoder_btn_config
    };

    /* Add encoder input (for selected screen) */
    encoder_handle = lvgl_port_add_encoder(&encoder);
    
    return encoder_handle;
}

static void _button_press_down_cb(void *arg, void *data)
{
    ESP_LOGI(TAG, "BTN: BUTTON_PRESS_DOWN");
}

static void _button_press_up_cb(void *arg, void *data)
{
    ESP_LOGI(TAG, "BTN: BUTTON_PRESS_UP[%d]", iot_button_get_ticks_time((button_handle_t)arg));
}

static void _knob_right_cb(void *arg, void *data)
{
    ESP_LOGI(TAG, "KONB: KONB_RIGHT,count_value:%d", iot_knob_get_count_value((button_handle_t)arg));
}

static void _knob_left_cb(void *arg, void *data)
{
    ESP_LOGI(TAG, "KONB: KONB_LEFT,count_value:%d", iot_knob_get_count_value((button_handle_t)arg));
}

static button_handle_t s_btn = 0;
static knob_handle_t s_knob = 0;

static void _button_init(void)
{
    button_config_t cfg = {
        .type = BUTTON_TYPE_GPIO,
        .long_press_time = 1000,
        .short_press_time = 200,
        .gpio_button_config = {
            .gpio_num  = CONFIG_ENCODE_BTN,
            .active_level = 0,
        },
    };
    s_btn = iot_button_create(&cfg);
    iot_button_register_cb(s_btn, BUTTON_PRESS_DOWN, _button_press_down_cb, NULL);
    iot_button_register_cb(s_btn, BUTTON_PRESS_UP, _button_press_up_cb, NULL);
}

static void _knob_init(void)
{
    knob_config_t cfg = {
        .default_direction = 0,
        .gpio_encoder_a = CONFIG_ENCODE_A,
        .gpio_encoder_b = CONFIG_ENCODE_B,
    };
    s_knob = iot_knob_create(&cfg);
    if (NULL == s_knob) {
        ESP_LOGE(TAG, "knob create failed");
    }

    iot_knob_register_cb(s_knob, KNOB_LEFT, _knob_left_cb, NULL);
    iot_knob_register_cb(s_knob, KNOB_RIGHT, _knob_right_cb, NULL);
}

void encode_test_init()
{
    _button_init();
    _knob_init();
}