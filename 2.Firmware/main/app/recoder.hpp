#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <stdint.h>

#define RECODER_BUFFER_SIZE 620  // 16000hz 16bit 每秒需要 31kb, 620 个 1024 字节的元素可以保存 20 秒音频
#define RECODER_BATCH_SIZE  1024

typedef enum
{
    RUNNING,
    STOP
} recoder_state_t;

typedef struct
{
    void    *data;
    uint16_t data_size;
} pcm_data_t;

namespace recoder {
class Recoder {
  public:
    static Recoder &getInstance()
    {
        static Recoder instance;  // 局部静态变量
        return instance;
    }

    Recoder(const Recoder &)            = delete;
    Recoder &operator=(const Recoder &) = delete;

    void start();
    void stop();
    bool is_recoder_stop();

    QueueHandle_t recoder_queue;

  private:
    Recoder();

  private:
    recoder_state_t state;
    TaskHandle_t   *task_handle;
};

}  // namespace recoder
