#pragma once


#include <sys/video/video.h>
#include <sys/video/stdout/font.h>

// instances
extern stiletto_video_t video;
extern uint32_t         color;

#define FONT font

void putchr(unsigned char);
void putstr(const char*);
