#pragma once


#include <stdint.h>


#include <sys/bootprotocol/stiletto_video.h>

stiletto_video_t video;
uint32_t         color;

void init_video(stiletto_video_t stiletto_video);

static inline void putpixel_internal(uint32_t x, uint32_t y, uint32_t color) {
    video.pFrame_buffer_base[y * video.ppsl + x] = color;
}

static inline void clr_scr() {
    // draw vertically for the vsync
    for(uint32_t __y__ = 0; __y__ < video.vert; ++__y__) {
        for(uint32_t __x__ = 0; __x__ < video.horiz; ++__x__) {
            putpixel_internal(__x__, __y__, 0x080C0E);
        }
    }
}

static inline void fill_scr(uint32_t hex_rgb_color) {
    // draw vertically for the vsync
    for(uint32_t __y__ = 0; __y__ < video.vert; ++__y__) {
        for(uint32_t __x__ = 0; __x__ < video.horiz; ++__x__) {
            putpixel_internal(__x__, __y__, hex_rgb_color);
        }
    }
}


#define putpixel(__x__, __y__)                  putpixel_internal(__x__, __y__, color)
#define putpixel_rgb(__x__, __y__, __color__)   putpixel_internal(__x__, __y__, __color__)
