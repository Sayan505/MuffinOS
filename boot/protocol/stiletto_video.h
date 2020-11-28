#pragma once


#include <stdint.h>

typedef struct {
    uint64_t *pFrame_buffer_base;
    uint64_t frame_buffer_sz;
    uint32_t horiz;
    uint32_t vert;
    uint32_t ppsl;
} stiletto_video_t;
