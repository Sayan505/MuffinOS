#pragma once


#include <stdint.h>

stiletto_video_t video;         // kernel's instance of stiletto_video_t

uint32_t v_color;               // current draw color; 32bpp
pixel_t  rgb;                   // EFI_PIXEL_BITMASK; 32bpp
