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
#define CONFIG_CODEC_MCLK_PIN         5
#define CONFIG_CODEC_BCLK_PIN         9
#define CONFIG_CODEC_WS_PIN           7
#define CONFIG_CODEC_DATA_OUT_PIN     6
#define CONFIG_CODEC_DATA_IN_PIN      -1

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

/* SCREEN */
#define CONFIG_LCD_BK_LIGHT_GPIO 1
#define CONFIG_LCD_PCLK_GPIO 12
#define CONFIG_LCD_DATA0_GPIO 11
#define CONFIG_LCD_DC_GPIO 3
#define CONFIG_LCD_CS_GPIO 10
#define CONFIG_LCD_RES_GPIO 2

#define CONFIG_ENCODE_BTN 42
#define CONFIG_ENCODE_A 45
#define CONFIG_ENCODE_B 46

#define CONFIG_BSP_DISPLAY_LVGL_TASK_CORE_ID -1
#define CONFIG_BSP_DISPLAY_LVGL_TASK_STACK_SIZE_KB 4
#define CONFIG_BSP_DISPLAY_LVGL_TASK_PRIORITY 2

#define CONFIG_LCD_H_RES 240
#define CONFIG_LCD_V_RES 240
#define CONFIG_LVGL_BUFFER_HEIGHT 240
#define CONFIG_BSP_DISPLAY_LVGL_TICK 5
#define CONFIG_BSP_DISPLAY_LVGL_TASK_DELAY 10

/* CAMERA */
#define COMFIG_CAM_PIN_PWDN -1  //power down is not used
#define COMFIG_CAM_PIN_RESET -1 //software reset will be performed
#define COMFIG_CAM_PIN_XCLK 4
#define COMFIG_CAM_PIN_SIOD 47
#define COMFIG_CAM_PIN_SIOC 48

#define COMFIG_CAM_PIN_D7 13
#define COMFIG_CAM_PIN_D6 40
#define COMFIG_CAM_PIN_D5 17
#define COMFIG_CAM_PIN_D4 39
#define COMFIG_CAM_PIN_D3 14
#define COMFIG_CAM_PIN_D2 38
#define COMFIG_CAM_PIN_D1 13
#define COMFIG_CAM_PIN_D0 21
#define COMFIG_CAM_PIN_VSYNC 8
#define COMFIG_CAM_PIN_HREF 9
#define COMFIG_CAM_PIN_PCLK 41

#define CONFIG_WEBSOCKET_URI "ws://192.168.31.147:8000/recode"
