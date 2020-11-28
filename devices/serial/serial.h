#include "../../sys/io_ports.h"

void init_serial(uint16_t port);
int  is_serial_bus_clear(uint16_t port);
void serial_char_out(uint16_t port, char datb);
int is_serial_recv(uint16_t port);
char serial_char_in(uint16_t port);

static inline void set_baud(uint16_t port, uint8_t baud_rate) {
    outb(port + 0, baud_rate);
    outb(port + 1, 0x00);
}

#define com1_out(__datb__) serial_char_out(COM1, __datb__)
#define com1_in() serial_char_in(COM1)