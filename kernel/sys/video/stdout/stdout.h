#pragma once


#include <sys/video/video.h>
#include <sys/video/stdout/font.h>


#define FONT font

void putchr(unsigned char);
void putstr(const char*);
