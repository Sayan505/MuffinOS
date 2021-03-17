/*
    The MuffinOS Kernel
*/


#include <kernel/kernel.h>


__attribute__((noreturn))
void start_kernel(stiletto_t *stiletto) {
    init_video(stiletto->stiletto_video);

    clr_scr();

    putstr("MEOWWWW!\n\r");
    putchr('>');

    init_com1();
    
    com1_out('O');
    com1_out('K');

    __endkernel__: goto __endkernel__;    // end of kernel
}
