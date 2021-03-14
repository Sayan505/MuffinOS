#pragma once


#include <stdint.h>


#include <sys/bootprotocol/stiletto_video.h>

// Pixel Format
#include <sys/video/pixel.h>


extern stiletto_video_t video;         // kernel's instance of stiletto_video_t

extern uint32_t v_color;               // current draw color; 32bpp
