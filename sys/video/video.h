#pragma once


#include <stdint.h>


#include <sys/bootprotocol/stiletto_video.h>

// Pixel Format
#include <sys/video/pixel.h>


stiletto_video_t video;         // kernel's instance of stiletto_video_t

uint32_t v_color;               // current draw color; 32bpp
pixel_t  rgb;                   // EFI_PIXEL_BITMASK; 32bpp


void init_video(stiletto_video_t stiletto_video);


static inline void putpixel_internal(uint32_t x, uint32_t y, uint32_t color) {
    video.pFrame_buffer_base[y * video.ppsl + x] = color;
}

static inline void clr_scr() {
    // draw vertically for the vsync
    for(uint32_t __y__ = 0; __y__ < video.vert; ++__y__) {
        for(uint32_t __x__ = 0; __x__ < video.horiz; ++__x__) {
            putpixel_internal(__x__, __y__, 0x000000);
        }
    }
}


#define putpixel(__x__, __y__)              putpixel_internal(__x__, __y__, v_color)
#define putpixel_rgb(__x__, __y__, __color__)   putpixel_internal(__x__, __y__, __color__)
