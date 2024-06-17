#pragma once

/* WiFi */
#define CONFIG_WIFI_SSID_DEFAULT     "902"
#define CONFIG_WIFI_PASSWORD_DEFAULT "11223344"

#define CONFIG_WIFI_SSID_SET_1     "CMCC-CC"
#define CONFIG_WIFI_PASSWORD_SET_1 "13973203087"

/* I2C */
// #define CONFIG_MCU_SDA_PIN            18 // lyrat
// #define CONFIG_MCU_SCL_PIN            23 // lyrat
#define CONFIG_MCU_SDA_PIN            47 // lcd
#define CONFIG_MCU_SCL_PIN            48 // lcd
#define CONFIG_I2C_CLK_SPEED_HZ              400000              
#define CONFIG_I2C_NUM                       1

/* I2S */
// #define CONFIG_CODEC_MCLK_PIN         0
// #define CONFIG_CODEC_BCLK_PIN         5
// #define CONFIG_CODEC_WS_PIN           25
// #define CONFIG_CODEC_DATA_OUT_PIN     26
// #define CONFIG_CODEC_DATA_IN_PIN      35
#define CONFIG_CODEC_MCLK_PIN         5
#define CONFIG_CODEC_BCLK_PIN         9
#define CONFIG_CODEC_WS_PIN           7
#define CONFIG_CODEC_DATA_OUT_PIN     6
#define CONFIG_CODEC_DATA_IN_PIN      -1

// #define CONFIG_ADC_MCLK_PIN           0
// #define CONFIG_ADC_BCLK_PIN           32
// #define CONFIG_ADC_WS_PIN             33
// #define CONFIG_ADC_DATA_OUT_PIN       -1
// #define CONFIG_ADC_DATA_IN_PIN        36
#define CONFIG_ADC_MCLK_PIN           5
#define CONFIG_ADC_BCLK_PIN           16
#define CONFIG_ADC_WS_PIN             7
#define CONFIG_ADC_DATA_OUT_PIN       -1
#define CONFIG_ADC_DATA_IN_PIN        15

#define CONFIG_POWER_AMP_IO        (1ULL << 0)

#define CONFIG_BSP_I2S_NUM        1
/* PA */  
#define CONFIG_MCU_PA_PIN             21

#define CONFIG_ES8311_ADDR            0x18
#define CONFIG_ES7210_ADDR            0x41
#define CONFIG_ES7243_ADDR            0x13
