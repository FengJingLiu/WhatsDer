#pragma once

#include "request.hpp"
#include <cassert>
#include <cstddef>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "../gui/ui.h"
#include "../hal/hal.h"
#include "lvgl.h"

extern lv_indev_t *encoder_handle;

namespace display {

void display_init(void);

typedef enum
{
    recoder_running,
    recoder_stop,
} recoder_status_t;

typedef struct Entry
{
    Entry            *prev, *next;
    lv_obj_t         *msg_panel;
    lv_obj_t         *msg_label;
    request::Message *msg;
} msg_list_t;

class ChatManager {
  public:
    static ChatManager &getInstance()
    {
        static ChatManager instance;  // 局部静态变量
        return instance;
    }

    ChatManager(const ChatManager &)            = delete;
    ChatManager &operator=(const ChatManager &) = delete;

    void              add_message(request::Message *msg);
    request::Message *get_message(int log_id) const;        // 通过 log id 查询
    request::Message *get_message(const char *uuid) const;  // 通过 record id uuid 查询
    inline void       update_message_to_screen()
    {
        char *message_content = this->tail_msg_list->msg->get_message_content();
        // ESP_LOGI("update message", "msg address %p", this->tail_msg_list->msg);
        ESP_LOGI("update message", "%s", message_content);
        lv_label_set_text(this->cur_message_label, message_content);
        /* 更新标签大小 */
        lv_obj_set_height(this->cur_message_label, LV_SIZE_CONTENT);
        /* 更新面板大小以适应标签 */
        lv_obj_set_height(this->cur_message_panel, lv_obj_get_height(this->cur_message_label));
    }
    inline void set_screen_activate(bool flag) { this->screen_activate = flag; }

  private:
    ChatManager();

  private:
    lv_obj_t   *cur_screen;
    lv_obj_t   *cur_message_panel;
    lv_obj_t   *cur_message_label;
    msg_list_t *head_msg_list;
    msg_list_t *tail_msg_list;
    size_t      message_cnt;
    bool        screen_activate;
};

}  // namespace display
