#include <stdint.h>

// COM
#define COM1 (0x3F8)
#define COM2 (0x2F8)

// Baud Rates
#define BAUD_115200 (0x01)
#define BAUD_57600  (0x02)
#define BAUD_38400  (0x03)


static inline void outb(uint16_t port, uint8_t datb) {
    __asm__ volatile ("outb %0, %1"
                      :
                      : "a"(datb), "Nd"(port)
                     );
}

static inline uint8_t inb(uint16_t port) {
    uint8_t datb;

    __asm__ volatile ("inb %1, %0"
                      : "=a"(datb)
                      : "Nd"(port)
                      );
    
    return datb;
}
