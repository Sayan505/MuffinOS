/*
    The MuffinOS Kernel
*/


#include <kernel/kernel.h>


__attribute__((noreturn))
void start_kernel(stiletto_t *stiletto) {
    init_video(stiletto->stiletto_video);

    clr_scr();

    putstr("MEOWWWWM!\n\r");
    putchr(0+48);
    putchr('>');


    init_com1();
    
    com1_out('O');
    com1_out('K');

    __endkernel__: goto __endkernel__;    // end of kernel
}
