/*
    The MuffinOS Kernel
*/


#include <kernel/kernel.h>


__attribute__((noreturn))
void start_kernel(stiletto_t *stiletto) {
    init_video(stiletto->stiletto_video);

    clr_scr();

    k_putstr("MEOWWWWM!\n\r");


    init_com1();
    
    com1_out('O');
    com1_out('K');

    __endkernel__: goto __endkernel__;    // end of kernel
}

//sudo dd if=OS.img of=/dev/sdc bs=1M count=64 oflag=sync
