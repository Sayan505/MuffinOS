#include "serial.h"


void init_serial(uint16_t port) {
    outb(port + 1, 0x00);
    outb(port + 3, 0x80);

    set_baud(port, BAUD_115200);    // set baud rate to 115200

    outb(port + 3, 0x03);
    outb(port + 2, 0xC7);
    outb(port + 4, 0x0B);
}

int is_serial_bus_clear(uint16_t port) {
    return inb(port + 5) & 0x20;
}

void serial_char_out(uint16_t port, char datb) {
    while (is_serial_bus_clear(port) == 0);

    outb(port, datb);
}

int is_serial_recv(uint16_t port) {
    return inb(port + 5) & 1;
}

char serial_char_in(uint16_t port) {
    while(is_serial_recv(port) == 0);

    return inb(port);
}
