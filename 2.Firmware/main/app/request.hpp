#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_err.h"
#include "esp_websocket_client.h"
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

typedef enum
{
    ASSISTANT_ROLE,  // gpt 回复
    USER_ROLE        // 用户提问
} message_role_t;

typedef enum
{
    OPEN,
    CLOSE,
    OPENING
} websocket_state_t;

namespace request {

void base_test(void);

class Message {
  public:
    Message();
    Message(message_role_t role);
    Message(message_role_t role, char *str, size_t str_len);
    Message(message_role_t role, char *str, size_t str_len, char *record_uuid);

    void set_role(message_role_t role);
    /* 添加录音的 uuid ，重放时通过 uuid 在 server上查找 */
    void add_recode(char *uuid);
    /* message_content 随着语音输入不断变化，每次更新内容 */
    void update_message_content(const char *str, size_t str_len);
    /* 把不再更新的内容添加到 fix_message_content */
    void done_message_content(const char *str, size_t str_len);
    /* 最终拿到的 message_content 是 fix_message_content 和 unfix_message_content 拼接的 */
    char                    *get_message_content();
    inline void              add_msg_id(uint32_t msg_id) { this->msg_id = msg_id; }
    void                     all_done();
    inline uint32_t          get_msg_id() { return this->msg_id; }
    inline char             *get_record_uuid() { return this->record_uuid; }
    inline bool              get_done_flag() { return done_flag; }
    inline SemaphoreHandle_t get_mutex() { return mutex; }

  private:
    void lock();
    void unlock();

  private:
    SemaphoreHandle_t mutex;
    message_role_t    role;
    char              fix_message[1024];
    char              unfix_message[256];
    char              record_uuid[37];
    bool              done_flag;
    uint32_t          msg_id;
};

/*
gui 进入聊天界面的时候建立 websocket 连接，当 RECODER 事件组收到开始信号发送百度语音识别建立信号，
当接收到 RECODE_QUEUE 数据就把 pcm data 发送到服务端
 */

class Websocket {
  public:
    static Websocket &getInstance()
    {
        static Websocket instance;  // 局部静态变量
        return instance;
    }

    static void send_pcm_task(void *pvParameters);

    Websocket(const Websocket &)            = delete;
    Websocket &operator=(const Websocket &) = delete;

    void        open();
    void        close();
    void        recoder_start();
    void        recoder_end();
    void        update_cur_msg(const char *new_data, size_t new_data_size);
    inline void clear_update_message_flag() { this->update_message_flag = 0; }
    inline bool get_update_message_flag() { return this->update_message_flag; }
    inline Message    *get_cur_msg() { return cur_msg; }

  public:
    esp_websocket_client_handle_t client;

  private:
    Websocket();

  private:
    Message          *cur_msg;
    websocket_state_t state;
    websocket_state_t recorder_state;
    QueueHandle_t     recoder_queue;
    TaskHandle_t      send_pcm_task_handle;
    TimerHandle_t     update_message_timer_handle;
    bool              update_message_flag;
};

}  // namespace request