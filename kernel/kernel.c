/*
    The MuffinOS Kernel
*/

#include <kernel/kernel.h>


__attribute__((noreturn))
void start_kernel(stiletto_t *stiletto) {
    init_video(stiletto->stiletto_video);

    clr_scr();

    init_com1();
    
    com1_out('x');

    __endkernel__: goto __endkernel__;    // end of kernel
}
