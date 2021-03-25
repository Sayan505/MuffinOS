#pragma once


#include <sys/video/video.h>
#include <sys/video/stdout/font.h>

// instances
extern stiletto_video_t video;
extern uint32_t         color;

#define FONT font

void k_putchr(const unsigned char _char);
void k_putstr(const char* _str);

void k_putchr_rgb(const unsigned char _char, uint32_t _color);
void k_putstr_rgb(const char* _str, uint32_t _color);
