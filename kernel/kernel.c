/*
    The MuffinOS Kernel
*/

#include <kernel/kernel.h>

__attribute__((noreturn))
void start_kernel(stiletto_t *stiletto) {

    uint32_t *fb   = (uint32_t *)stiletto->stiletto_video.pFrame_buffer_base;   // 32bpp
    uint32_t horiz = stiletto->stiletto_video.horiz;
    uint32_t vert  = stiletto->stiletto_video.vert;
    uint32_t ppsl  = stiletto->stiletto_video.ppsl;

    uint32_t color = 0x34495E;  // use any RGB hex


    // draw vertically for the vsync
    for(uint32_t y = 0; y < vert; ++y) {
        for(uint32_t x = 0; x < horiz; ++x) {
            PutPixel(x, y, color);
        }
    }

    init_serial(COM1);
    
    com1_out('x');

    __endkernel__: goto __endkernel__;    // end of kernel
}
