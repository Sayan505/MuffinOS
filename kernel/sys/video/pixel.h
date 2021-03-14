#pragma once


#include <stdint.h>


typedef struct {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t _res;   // reserved; 0
} pixel_t, color_t;
