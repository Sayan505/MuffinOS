/*
    The MuffinOS Kernel
*/


#include <kernel.h>
#include <sys/debug/debug.h>

__attribute__((noreturn))
void start_kernel(stiletto_t *stiletto) {
    
    // init serial
    init_com1();

    com1_out('O');
    com1_out('K');


    // init video
    init_video(stiletto->stiletto_video);

    clr_scr();

    k_putstr("MEOWWWWM!\n\r");
    
    //INT3;




    __endkernel__: goto __endkernel__;    // end of kernel
}

//sudo dd if=OS.img of=/dev/sdc bs=1M count=64 oflag=sync
