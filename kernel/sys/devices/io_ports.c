#include <sys/devices/io_ports.h>


VOID outb(UI16 port, BYTE datb) {
    __asm__ volatile ("outb %0, %1"
                      :
                      : "a"(datb), "Nd"(port)
                     );
}

BYTE inb(UI16 port) {
    BYTE datb;

    __asm__ volatile ("inb %1, %0"
                      : "=a"(datb)
                      : "Nd"(port)
                     );
    
    return datb;
}
