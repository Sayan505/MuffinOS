#include <sys/stdout/stdout.h>


void putchar_internal(unsigned char _char, uint32_t _rgb) {
    static uint32_t posi_x = 8;
    static uint32_t posi_y = 12;

    // CR
    if (_char == '\r') {
        posi_x = 8;

        return;
    }
    // LF
    if (_char == '\n') {
        posi_y += 12;

        return;
    }

    int32_t cx, cy;
    int32_t mask[8] = { 1, 2, 4, 8, 16, 32, 64, 128 };
    unsigned char* glyph = FONT + _char * 16ULL;

    for (cy = 0; cy < 16; ++cy) {
        for (cx = 0; cx < 8; ++cx) {
            if (glyph[cy] & mask[cx]) putpixel_rgb(posi_x - cx, posi_y + cy - 12, _rgb);
        }
    }

    // next posi
    posi_x += 8;
}


// TODO: Implement overloads for typedef struct { ... } pixel_t, color_t;
