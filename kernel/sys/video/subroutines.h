#pragma once


#include <sys/stdtypes.h>


#include <sys/bootprotocol/stiletto_video.h>


static inline VOID putpixel_internal(UI32 x, UI32 y, UI32 color) {
    video.pFrame_buffer_base[y * video.ppsl + x] = color;
}

static inline VOID clr_scr() {
    // draw vertically for the vsync
    for(UI32 __y__ = 0; __y__ < video.vert; ++__y__) {
        for(UI32 __x__ = 0; __x__ < video.horiz; ++__x__) {
            putpixel_internal(__x__, __y__, 0x080C0E);
        }
    }
}

static inline VOID fill_scr(UI32 hex_rgb_color) {
    // draw vertically for the vsync
    for(UI32 __y__ = 0; __y__ < video.vert; ++__y__) {
        for(UI32 __x__ = 0; __x__ < video.horiz; ++__x__) {
            putpixel_internal(__x__, __y__, hex_rgb_color);
        }
    }
}


#define putpixel(__x__, __y__)                  putpixel_internal(__x__, __y__, color)
#define putpixel_rgb(__x__, __y__, __color__)   putpixel_internal(__x__, __y__, __color__)

#define set_color(__color__) color = __color__
