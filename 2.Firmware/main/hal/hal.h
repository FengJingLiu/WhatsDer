#pragma once

#include <stdint.h>
#include "lvgl.h"
#include "esp_err.h"
#include "driver/i2s_std.h"
#include "esp_codec_dev.h"
#include "esp_io_expander.h"

#ifdef __cplusplus
extern "C" {
#endif

void hal_all_init(void);
void hal_update(void);

/* I2C */
esp_err_t hal_i2c_init(void);
esp_err_t hal_nvs_init(void);
esp_err_t hal_audio_init(const i2s_std_config_t *i2s_config);

void hal_mic_open(void);
void hal_mic_close(void);
void hal_mic_read(void *recording_buffer, int buffer_size);
esp_codec_dev_handle_t hal_mic_init(void);

esp_codec_dev_handle_t hal_spk_init(void);
void hal_spk_open(void);
void hal_spk_close(void);
void hal_spk_play(void* pcm_data, int data_len);

esp_io_expander_handle_t hal_io_expander_init(void);

/* camera */
esp_err_t hal_cam_init(void);
esp_err_t hal_cam_capture(void);

/* Wifi */
void hal_wifi_init(void);

/* Screen */
lv_disp_t *hal_screen_init(void);

/* Encode */
lv_indev_t *encode_init(lv_disp_t *disp_handle);
void encode_test_init(void);

#ifdef __cplusplus
}
#endif