// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.1
// LVGL version: 8.3.11
// Project name: whatsder

#include "../ui.h"

void ui_Screen4_screen_init(void)
{
    ui_Screen4 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen4, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Screen4, lv_color_hex(0xC9C0C0), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(ui_Screen4, 255, LV_PART_MAIN | LV_STATE_FOCUSED);

    ui_Screen4LabelLabel4 = lv_label_create(ui_Screen4);
    lv_obj_set_width(ui_Screen4LabelLabel4, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Screen4LabelLabel4, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Screen4LabelLabel4, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Screen4LabelLabel4, "12");

    ui_Screen4ButtonButton13 = lv_btn_create(ui_Screen4);
    lv_obj_set_width(ui_Screen4ButtonButton13, 100);
    lv_obj_set_height(ui_Screen4ButtonButton13, 50);
    lv_obj_set_x(ui_Screen4ButtonButton13, -53);
    lv_obj_set_y(ui_Screen4ButtonButton13, -82);
    lv_obj_set_align(ui_Screen4ButtonButton13, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Screen4ButtonButton13, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Screen4ButtonButton13, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Screen4ButtonButton14 = lv_btn_create(ui_Screen4);
    lv_obj_set_width(ui_Screen4ButtonButton14, 100);
    lv_obj_set_height(ui_Screen4ButtonButton14, 50);
    lv_obj_set_x(ui_Screen4ButtonButton14, 56);
    lv_obj_set_y(ui_Screen4ButtonButton14, -78);
    lv_obj_set_align(ui_Screen4ButtonButton14, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Screen4ButtonButton14, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Screen4ButtonButton14, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Screen4ButtonButton15 = lv_btn_create(ui_Screen4);
    lv_obj_set_width(ui_Screen4ButtonButton15, 100);
    lv_obj_set_height(ui_Screen4ButtonButton15, 50);
    lv_obj_set_x(ui_Screen4ButtonButton15, -51);
    lv_obj_set_y(ui_Screen4ButtonButton15, 58);
    lv_obj_set_align(ui_Screen4ButtonButton15, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Screen4ButtonButton15, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Screen4ButtonButton15, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Screen4ButtonButton16 = lv_btn_create(ui_Screen4);
    lv_obj_set_width(ui_Screen4ButtonButton16, 100);
    lv_obj_set_height(ui_Screen4ButtonButton16, 50);
    lv_obj_set_x(ui_Screen4ButtonButton16, 57);
    lv_obj_set_y(ui_Screen4ButtonButton16, 61);
    lv_obj_set_align(ui_Screen4ButtonButton16, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Screen4ButtonButton16, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Screen4ButtonButton16, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    lv_obj_add_event_cb(ui_Screen4, ui_event_Screen4, LV_EVENT_ALL, NULL);

}
