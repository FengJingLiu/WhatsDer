#include "hal.h"

#include "esp_camera.h"
#include "esp_log.h"
#include "../common/config.h"

#define TAG "hal_camera"

esp_err_t hal_cam_init(void)
{
    camera_config_t camera_config = {
        .pin_pwdn = COMFIG_CAM_PIN_PWDN,
        .pin_reset = COMFIG_CAM_PIN_RESET,
        .pin_xclk = COMFIG_CAM_PIN_XCLK,
        .pin_sccb_sda = -1,
        .pin_sccb_scl = -1,

        .pin_d7 = COMFIG_CAM_PIN_D7,
        .pin_d6 = COMFIG_CAM_PIN_D6,
        .pin_d5 = COMFIG_CAM_PIN_D5,
        .pin_d4 = COMFIG_CAM_PIN_D4,
        .pin_d3 = COMFIG_CAM_PIN_D3,
        .pin_d2 = COMFIG_CAM_PIN_D2,
        .pin_d1 = COMFIG_CAM_PIN_D1,
        .pin_d0 = COMFIG_CAM_PIN_D0,
        .pin_vsync = COMFIG_CAM_PIN_VSYNC,
        .pin_href = COMFIG_CAM_PIN_HREF,
        .pin_pclk = COMFIG_CAM_PIN_PCLK,
        .xclk_freq_hz = 20000000,//EXPERIMENTAL: Set to 16MHz on ESP32-S2 or ESP32-S3 to enable EDMA mode
        .ledc_timer = LEDC_TIMER_0,
        .ledc_channel = LEDC_CHANNEL_0,
        .pixel_format = PIXFORMAT_RGB565, //YUV422,GRAYSCALE,RGB565,JPEG
        .frame_size = FRAMESIZE_VGA,    //QQVGA-UXGA, For ESP32, do not use sizes above QVGA when not JPEG. The performance of the ESP32-S series has improved a lot, but JPEG mode always gives better frame rates.

        .jpeg_quality = 12, //0-63, for OV series camera sensors, lower number means higher quality
        .fb_count = 1,       //When jpeg mode is used, if fb_count more than one, the driver will work in continuous mode.
        .fb_location = CAMERA_FB_IN_PSRAM,
        .grab_mode = CAMERA_GRAB_WHEN_EMPTY,
        .sccb_i2c_port = CONFIG_I2C_NUM,
    };
    //initialize the camera
    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Camera Init Failed");
        return err;
    }

    return ESP_OK;
}

#include "../gui/ui.h"

esp_err_t hal_cam_capture(){
    //acquire a frame
    camera_fb_t * fb = esp_camera_fb_get();
    if (!fb) {
        ESP_LOGE(TAG, "Camera Capture Failed");
        return ESP_FAIL;
    }
    
    ESP_LOGI(TAG,"fb->width:%d, fb->height:%d, fb->format:%d, fb->len:%d", fb->width, fb->height, fb->format, fb->len);    

    //return the frame buffer back to the driver for reuse
    esp_camera_fb_return(fb);
    return ESP_OK;
}