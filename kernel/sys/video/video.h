#pragma once


#include <stdint.h>


#include <sys/bootprotocol/stiletto_video.h>

// Pixel Format
#include <sys/video/pixel.h>

// global vars
#include <sys/video/video_vars.h>

// global subroutines (incl. after vars)
#include <sys/video/video_subroutines.h>


// TODO: Implement overloads for typedef struct { ... } pixel_t, color_t;
// TODO: Implement a global set_color subroutine.
// TODO: Font renderer