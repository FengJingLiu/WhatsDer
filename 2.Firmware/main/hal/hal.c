#include "hal.h"

#include "hal_wifi.c"
#include "esp_err.h"
#include "nvs_flash.h"
#include "driver/i2c.h"
#include "driver/i2c_master.h"

#include "../common/config.h"
#include "../common/err_check.h"

static bool i2c_initialized = false;

/**************************************************************************************************
 *
 * I2C Function
 *
 **************************************************************************************************/
esp_err_t hal_i2c_init(void)
{
    /* I2C was initialized before */
    if (i2c_initialized) {
        return ESP_OK;
    }

    i2c_config_t i2c_conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = CONFIG_MCU_SDA_PIN,
        .sda_pullup_en = GPIO_PULLUP_DISABLE,
        .scl_io_num = CONFIG_MCU_SCL_PIN,
        .scl_pullup_en = GPIO_PULLUP_DISABLE,
        .master.clk_speed = CONFIG_I2C_CLK_SPEED_HZ
    };

    BSP_ERROR_CHECK_RETURN_ERR(i2c_param_config(CONFIG_I2C_NUM, &i2c_conf));
    BSP_ERROR_CHECK_RETURN_ERR(i2c_driver_install(CONFIG_I2C_NUM, i2c_conf.mode, 0, 0, 0));

    i2c_initialized = true;

    return ESP_OK;
}

// Initialize NVS
esp_err_t hal_nvs_init(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    return ESP_OK;
}

void hal_all_init(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    
    hal_nvs_init();
    hal_wifi_init();

    hal_i2c_init();
    hal_screen_init();
    // hal_cam_init();
    
    hal_audio_init(NULL);
    hal_mic_init();
    hal_spk_init();
}