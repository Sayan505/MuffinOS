#include <sys/devices/io_ports.h>

void outb(uint16_t port, uint8_t datb) {
    __asm__ volatile ("outb %0, %1"
                      :
                      : "a"(datb), "Nd"(port)
                     );
}

uint8_t inb(uint16_t port) {
    uint8_t datb;

    __asm__ volatile ("inb %1, %0"
                      : "=a"(datb)
                      : "Nd"(port)
                     );
    
    return datb;
}
