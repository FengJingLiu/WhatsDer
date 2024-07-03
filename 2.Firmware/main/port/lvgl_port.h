#pragma once 

#include "lvgl.h"
#include "err_check.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"

esp_err_t port_lvgl_init(esp_lcd_panel_handle_t lcd, lv_disp_t **disp);
bool lvgl_port_lock(uint32_t timeout_ms);
void lvgl_port_unlock(void);

void lv_practice_layout_1();