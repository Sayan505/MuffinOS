/*
    CONDITION: BAREBONES; STUB KERNEL.
*/

#include "../boot/protocol/stiletto.h"
#include "../devices/serial/serial.h"

#include <stdint.h>

#define PutPixel(__x__,__y__,__pxfmt__) fb[__y__*ppsl+__x__]=__pxfmt__


__attribute__((noreturn))
void start_kernel(stiletto_t *stiletto) {

    uint32_t *fb   = (uint32_t *)stiletto->bootprot_video.pFrame_buffer_base;   // 32bpp
    uint32_t horiz = stiletto->bootprot_video.horiz;
    uint32_t vert  = stiletto->bootprot_video.vert;
    uint32_t ppsl  = stiletto->bootprot_video.ppsl;

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
