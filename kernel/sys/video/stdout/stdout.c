#include <sys/video/stdout/stdout.h>


void k_putstr(const char* _str) {
    while (*_str != '\0') {
        k_putchr(*_str);

        ++_str;
    }
}

void k_putchr(unsigned char _char) {
    static int posi_x = char_width;
    static int posi_y = char_height;

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

    int cx, cy;
    int mask[8] = { 1, 2, 4, 8, 16, 32, 64, 128 };
    unsigned char* glyph = FONT + _char * 16ULL;

    for (cy = 0; cy < 16; ++cy) {
        for (cx = 0; cx < 8; ++cx) {
            if (glyph[cy] & mask[cx]) putpixel(posi_x - cx, posi_y + cy - 12);
        }
    }

    // next posi
    posi_x += 8;
}
