#pragma once

#include <stddef.h>
#include <stdint.h>

#include "esp_err.h"

typedef enum {
    is_recoding = 0,
    is_stoping,
    done_stop
} recoding_state;

void record_start(void);

void record_stop(char **record_data, uint32_t *data_len);

recoding_state get_recording_flag();

void record_and_play(void);