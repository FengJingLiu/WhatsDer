// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.1
// LVGL version: 8.3.11
// Project name: whatsder

#ifndef _UI_EVENTS_H
#define _UI_EVENTS_H

#ifdef __cplusplus
extern "C" {
#endif

void websocket_open(lv_event_t * e);
void websocket_close(lv_event_t * e);
void ui_screen_load_cb(lv_event_t * e);
void into_obj(lv_event_t * e);
void exit_obj(lv_event_t * e);
void recode_btn_press(lv_event_t * e);
void edit_mod(lv_event_t * e);
void diff_obj_to_group(lv_event_t * e);
void add_obj_to_group(lv_event_t * e);
void ui_editing_on(lv_event_t * e);
void ui_screen_unload_cb(lv_event_t * e);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif