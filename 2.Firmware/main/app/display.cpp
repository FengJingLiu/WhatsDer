#include "core/lv_group.h"
#include "core/lv_obj.h"
#include "display.hpp"

#include "../gui/ui.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "event_declare.h"
#include "request.hpp"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>

namespace display {

#define TAG "display"

// 添加新聊天栏
static lv_obj_t *create_new_message_panel(lv_obj_t *pareet)
{
    lv_obj_t *new_msg_panel = lv_obj_create(pareet);
    lv_obj_set_width(new_msg_panel, 200);
    lv_obj_set_height(new_msg_panel, 50);
    lv_obj_align_to(pareet, lv_obj_get_child(pareet, -1), LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    lv_obj_add_flag(new_msg_panel, LV_OBJ_FLAG_SCROLL_ON_FOCUS);  /// Flags
    lv_obj_clear_flag(new_msg_panel, LV_OBJ_FLAG_SCROLLABLE);     /// Flags
    lv_obj_set_style_bg_color(new_msg_panel, lv_color_hex(0xD6CECE), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(new_msg_panel, 255, LV_PART_MAIN | LV_STATE_FOCUSED);

    lv_obj_t *new_msg_lable = lv_label_create(new_msg_panel);
    lv_obj_set_width(new_msg_lable, lv_pct(100));
    lv_obj_set_height(new_msg_lable, lv_pct(100));
    lv_obj_set_align(new_msg_lable, LV_ALIGN_CENTER);
    lv_label_set_long_mode(new_msg_lable, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_font(new_msg_lable, &ui_font_Chinease, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(new_msg_lable, "新窗口");

    lv_obj_scroll_to_view(new_msg_panel, LV_ANIM_ON);
    return new_msg_panel;
}

static void
websocket_state_event_handler(void *handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_id == EVENT_UPDATE_MESSAGE)
    {
        display::ChatManager::getInstance().update_message_to_screen();
    }
    else if (event_id == EVENT_CREATE_MESSAGE)
    {
        event_data_wrap_t *event_data_post = (event_data_wrap_t*)event_data;
        // ESP_LOGI(TAG, "websocket_state_event_handler event_data_post->ptr address %p", event_data_post->ptr);
        request::Message *new_msg = (request::Message *)(event_data_post->ptr);
        // ESP_LOGI(TAG, "websocket_state_event_handler new_msg address %p", new_msg);
        display::ChatManager::getInstance().add_message(new_msg);
    }
    else if (event_id == EVENT_SCREEN_ACTIVATE)
    {
        display::ChatManager::getInstance().set_screen_activate(true);
    }
    else if (event_id == EVENT_SCREEN_DISACTIVATE)
    {
        display::ChatManager::getInstance().set_screen_activate(false);
    }
}

void display_init()
{
    lv_group_t *group = lv_group_create();
    lv_group_set_default(group);

    lv_indev_set_group(encoder_handle, group);
    lv_group_focus_freeze(group, false);

    esp_event_handler_register(APP_DISPLAY_EVENTS, ESP_EVENT_ANY_ID, websocket_state_event_handler, nullptr);
    ChatManager::getInstance();
    ui_init();
}

ChatManager::ChatManager() : message_cnt(0), screen_activate(false)
{
    cur_screen        = ui_Screen2;
    cur_message_panel = nullptr;
    cur_message_label = nullptr;
    head_msg_list     = (msg_list_t *)heap_caps_malloc(sizeof(msg_list_t), MALLOC_CAP_SPIRAM);
    tail_msg_list     = head_msg_list;
    memset(head_msg_list, 0, sizeof(msg_list_t));
}

void ChatManager::add_message(request::Message *msg)
{
    msg_list_t *new_msg = (msg_list_t *)heap_caps_malloc(sizeof(msg_list_t), MALLOC_CAP_SPIRAM);
    assert(new_msg != nullptr);
    new_msg->msg       = msg;
    new_msg->prev      = this->tail_msg_list;
    new_msg->next      = nullptr;
    new_msg->msg_panel = create_new_message_panel(ui_Screen2Panelcontainer);
    new_msg->msg_label = lv_obj_get_child(new_msg->msg_panel, 0);

    this->tail_msg_list->next = new_msg;
    this->tail_msg_list       = new_msg;

    cur_message_panel = new_msg->msg_panel;
    cur_message_label = new_msg->msg_label;
    // ESP_LOGI(TAG, "add_message msg address %p", new_msg->msg);
    message_cnt++;
}

request::Message *ChatManager::get_message(int log_id) const
{
    msg_list_t *cur_node = head_msg_list;
    while (cur_node && cur_node->msg->get_msg_id() != log_id)
    {
        return cur_node->msg;
    }
    return nullptr;
}

request::Message *ChatManager::get_message(const char *uuid) const
{
    msg_list_t *cur_node = head_msg_list;
    while (cur_node && strcmp(cur_node->msg->get_record_uuid(), uuid) == 0)
    {
        return cur_node->msg;
    }
    return nullptr;
}

}  // namespace display