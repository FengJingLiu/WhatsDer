#pragma once

#include "esp_err.h"


/// @brief 百度语音识别标准版 vop 请求
/// @param  pcm 二进制原始数据, 数据大小
/// @return 
esp_err_t req_vop(char* pcm_data, char* rsp_data, size_t size);

esp_err_t req_gpt_image(char* img_data, char* text_data, size_t size);
esp_err_t req_gpt_text(char* question, char* rsp_data, int size);

