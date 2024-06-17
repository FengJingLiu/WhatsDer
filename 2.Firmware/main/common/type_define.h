#pragma once

/**
 * @brief Codec Device type
 */
typedef enum {
    ESP_CODEC_DEV_TYPE_NONE,
    ESP_CODEC_DEV_TYPE_IN = (1 << 0),  /*!< Codec input device like ADC (capture data from microphone) */
    ESP_CODEC_DEV_TYPE_OUT = (1 << 1), /*!< Codec output device like DAC (output analog signal to speaker) */
    ESP_CODEC_DEV_TYPE_IN_OUT = (ESP_CODEC_DEV_TYPE_IN | ESP_CODEC_DEV_TYPE_OUT), /*!< Codec input and output device */
} esp_codec_dev_type_t;


/**
 * @brief Codec audio sample information
 *        Notes: channel_mask is used to filter wanted channels in driver side
 *               when set to 0, default filter all channels
 *               when channel is 2, can filter channel 0 (set to 1) or channel 1 (set to 2)
 *               when channel is 4, can filter either 3,2 channels or 1 channel
 */
typedef struct {
    uint8_t  bits_per_sample;   /*!< Bit lengths of one channel data */
    uint8_t  channel;           /*!< Channels of sample */
    uint16_t channel_mask;      /*!< Channel mask indicate which channel to be selected */
    uint32_t sample_rate;       /*!< Sample rate of sample */
    int mclk_multiple;          /*!< The multiple of MCLK to the sample rate
                                     If value is 0, mclk = sample_rate * 256
                                     If bits_per_sample is 24bit, mclk_multiple should be the multiple of 3
                                */
} esp_codec_dev_sample_info_t;