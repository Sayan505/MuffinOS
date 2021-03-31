#pragma once


#include <sys/stdtypes.h>

#include <sys/video/video.h>
#include <sys/video/stdout/font.h>

// instances
extern stiletto_video_t video;
extern DWORD            color;

#define FONT font

void k_putchr(const CHAR  _char);
void k_putstr(const CHAR* _str);

void k_putchr_rgb(const CHAR  _char, DWORD _color);
void k_putstr_rgb(const CHAR* _str,  DWORD _color);
