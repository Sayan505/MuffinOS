/*
    The MuffinOS Kernel
*/

#include <kernel/kernel.h>




/*
#include <sys/video/font.h>

void putchr(unsigned char);
void putstr(const char*);


void write() {
    putstr("Meow!\n\r");
    putstr("Hello, \nthere!\n\r");
    putchr('X');
}

void putstr(const char* _str) {
    while (*_str != '\0') {
        putchr(*_str);

        ++_str;
    }
}

void putchr(unsigned char _char) {
    static int posi_x = 8;
    static int posi_y = 12;

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
    unsigned char* glyph = font + _char * 16ULL;

    for (cy = 0; cy < 16; ++cy) {
        for (cx = 0; cx < 8; ++cx) {
            if (glyph[cy] & mask[cx]) putpixel(posi_x - cx, posi_y + cy - 12);
        }
    }

    // next posi
    posi_x += 8;
}
*/




__attribute__((noreturn))
void start_kernel(stiletto_t *stiletto) {
    init_video(stiletto->stiletto_video);

    clr_scr();

    putstr("MEOWWWW!\n\r");
    putchr('>');

    init_com1();
    
    com1_out('O');
    com1_out('K');

    //write();

    __endkernel__: goto __endkernel__;    // end of kernel
}
