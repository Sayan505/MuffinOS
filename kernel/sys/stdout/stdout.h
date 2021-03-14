#pragma once


#include <stdint.h>

// video
#include <sys/stdout/extern/video_vars.h>
#include <sys/video/video_subroutines.h>

// fonts
#include <sys/video/font.h>
#define FONT font

void putchar_internal(unsigned char _char, uint32_t _rgb);
