// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.1
// LVGL version: 8.3.11
// Project name: whatsder

#include "ui.h"
#include "ui_helpers.h"
///////////////////// VARIABLES ////////////////////


// SCREEN: ui_Screen2
void ui_Screen2_screen_init(void);
void ui_event_Screen2(lv_event_t * e);
lv_obj_t * ui_Screen2;
void ui_event_Screen2Panelcontainer(lv_event_t * e);
lv_obj_t * ui_Screen2Panelcontainer;
void ui_event_Screen2PanelPanel3(lv_event_t * e);
lv_obj_t * ui_Screen2PanelPanel3;
lv_obj_t * ui_Screen2LabelLabel5;
void ui_event_Screen2PanelPanel1(lv_event_t * e);
lv_obj_t * ui_Screen2PanelPanel1;
lv_obj_t * ui_Screen2LabelLabel7;
void ui_event_Screen2Panelrecode(lv_event_t * e);
lv_obj_t * ui_Screen2Panelrecode;
lv_obj_t * ui_Screen2LabelLabel3;


// SCREEN: ui_Screen4
void ui_Screen4_screen_init(void);
void ui_event_Screen4(lv_event_t * e);
lv_obj_t * ui_Screen4;
lv_obj_t * ui_Screen4LabelLabel4;
lv_obj_t * ui_Screen4ButtonButton13;
lv_obj_t * ui_Screen4ButtonButton14;
lv_obj_t * ui_Screen4ButtonButton15;
lv_obj_t * ui_Screen4ButtonButton16;


// SCREEN: ui_Screen5
void ui_Screen5_screen_init(void);
void ui_event_Screen5(lv_event_t * e);
lv_obj_t * ui_Screen5;
lv_obj_t * ui_Screen5ImageImage2;


// SCREEN: ui_Screen6
void ui_Screen6_screen_init(void);
void ui_event_Screen6(lv_event_t * e);
lv_obj_t * ui_Screen6;
lv_obj_t * ui_Screen6LabelLabel6;
lv_obj_t * ui_Screen6ButtonButton21;
lv_obj_t * ui_Screen6ButtonButton22;
lv_obj_t * ui_Screen6ButtonButton23;
lv_obj_t * ui_Screen6ButtonButton24;


// SCREEN: ui_Screen3
void ui_Screen3_screen_init(void);
void ui_event_Screen3(lv_event_t * e);
lv_obj_t * ui_Screen3;
lv_obj_t * ui_Screen3LabelLabel1;
lv_obj_t * ui_Screen3ButtonButton3;
lv_obj_t * ui_Screen3ButtonButton4;
lv_obj_t * ui_Screen3ButtonButton5;
lv_obj_t * ui_Screen3ButtonButton6;


// SCREEN: ui_Screen1
void ui_Screen1_screen_init(void);
void ui_event_Screen1(lv_event_t * e);
lv_obj_t * ui_Screen1;
lv_obj_t * ui_Screen1LabelLabel2;
lv_obj_t * ui_Screen1ButtonButton1;
lv_obj_t * ui_Screen1ButtonButton2;
lv_obj_t * ui_Screen1ButtonButton7;
lv_obj_t * ui_Screen1ButtonButton8;
lv_obj_t * ui_Startevents___initial_actions0;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////
void ui_event_Screen2(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_SCREEN_LOADED) {
        ui_screen_load_cb(e);
        websocket_open(e);
    }
    if(event_code == LV_EVENT_SCREEN_UNLOAD_START) {
        websocket_close(e);
    }
}
void ui_event_Screen2Panelcontainer(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        into_obj(e);
    }
    if(event_code == LV_EVENT_LONG_PRESSED_REPEAT) {
        exit_obj(e);
    }
}
void ui_event_Screen2PanelPanel3(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_LONG_PRESSED_REPEAT) {
        exit_obj(e);
    }
}
void ui_event_Screen2PanelPanel1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_LONG_PRESSED_REPEAT) {
        exit_obj(e);
    }
}
void ui_event_Screen2Panelrecode(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        recode_btn_press(e);
    }
}
void ui_event_Screen4(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_LEFT) {
        _ui_screen_change(&ui_Screen6, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Screen6_screen_init);
    }
    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_RIGHT) {
        _ui_screen_change(&ui_Screen5, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Screen5_screen_init);
    }
    if(event_code == LV_EVENT_FOCUSED) {
        edit_mod(e);
    }
    if(event_code == LV_EVENT_LONG_PRESSED) {
        _ui_screen_change(&ui_Screen1, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Screen1_screen_init);
    }
    if(event_code == LV_EVENT_SCREEN_UNLOAD_START) {
        diff_obj_to_group(e);
    }
    if(event_code == LV_EVENT_SCREEN_LOAD_START) {
        add_obj_to_group(e);
    }
}
void ui_event_Screen5(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_LEFT) {
        _ui_screen_change(&ui_Screen4, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Screen4_screen_init);
    }
    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_RIGHT) {
        _ui_screen_change(&ui_Screen6, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Screen6_screen_init);
    }
    if(event_code == LV_EVENT_FOCUSED) {
        ui_editing_on(e);
    }
    if(event_code == LV_EVENT_LONG_PRESSED) {
        _ui_screen_change(&ui_Screen2, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Screen2_screen_init);
    }
    if(event_code == LV_EVENT_SCREEN_UNLOAD_START) {
        ui_screen_unload_cb(e);
    }
    if(event_code == LV_EVENT_SCREEN_LOAD_START) {
        ui_screen_load_cb(e);
    }
}
void ui_event_Screen6(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_LEFT) {
        _ui_screen_change(&ui_Screen5, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Screen5_screen_init);
    }
    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_RIGHT) {
        _ui_screen_change(&ui_Screen4, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Screen4_screen_init);
    }
    if(event_code == LV_EVENT_FOCUSED) {
        edit_mod(e);
    }
    if(event_code == LV_EVENT_LONG_PRESSED) {
        _ui_screen_change(&ui_Screen3, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Screen3_screen_init);
    }
    if(event_code == LV_EVENT_SCREEN_UNLOAD_START) {
        diff_obj_to_group(e);
    }
    if(event_code == LV_EVENT_SCREEN_LOAD_START) {
        add_obj_to_group(e);
    }
}
void ui_event_Screen3(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_KEY) {
        _ui_screen_change(&ui_Screen6, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Screen6_screen_init);
    }
    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_LEFT) {
        _ui_screen_change(&ui_Screen2, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Screen2_screen_init);
    }
    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_RIGHT) {
        _ui_screen_change(&ui_Screen1, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Screen1_screen_init);
    }
    if(event_code == LV_EVENT_FOCUSED) {
        edit_mod(e);
    }
    if(event_code == LV_EVENT_SCREEN_UNLOAD_START) {
        diff_obj_to_group(e);
    }
    if(event_code == LV_EVENT_SCREEN_LOAD_START) {
        add_obj_to_group(e);
    }
}
void ui_event_Screen1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_KEY) {
        _ui_screen_change(&ui_Screen4, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Screen4_screen_init);
    }
    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_LEFT) {
        _ui_screen_change(&ui_Screen3, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Screen3_screen_init);
    }
    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_RIGHT) {
        _ui_screen_change(&ui_Screen2, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Screen2_screen_init);
    }
    if(event_code == LV_EVENT_FOCUSED) {
        edit_mod(e);
    }
    if(event_code == LV_EVENT_SCREEN_UNLOAD_START) {
        diff_obj_to_group(e);
    }
    if(event_code == LV_EVENT_SCREEN_LOADED) {
        add_obj_to_group(e);
    }
}

///////////////////// SCREENS ////////////////////

void ui_init(void)
{
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_Screen2_screen_init();
    ui_Screen4_screen_init();
    ui_Screen5_screen_init();
    ui_Screen6_screen_init();
    ui_Screen3_screen_init();
    ui_Screen1_screen_init();
    ui_Startevents___initial_actions0 = lv_obj_create(NULL);
    lv_disp_load_scr(ui_Screen2);
}