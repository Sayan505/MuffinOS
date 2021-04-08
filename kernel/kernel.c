/*
    The MuffinOS Kernel
*/


#include <kernel.h>


__attribute__((noreturn))
void start_kernel(stiletto_t *stiletto) {
    
    // init serial
    init_com1();

    com1_outs("OK");


    // init video
    init_video(stiletto->stiletto_video);

    clr_scr();

    k_putstr("MEOWWWWM!\n\r");


    __endkernel__: goto __endkernel__;    // end of kernel
}

//sudo dd if=OS.img of=/dev/sdc bs=1M count=64 oflag=sync
