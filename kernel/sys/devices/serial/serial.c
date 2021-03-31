#include <sys/devices/serial/serial.h>


VOID set_baud(UI16 port, BYTE baud_rate) {
    outb(port + 0, baud_rate);
    outb(port + 1, 0x00);
}


VOID init_serial(UI16 port) {
    outb(port + 1, 0x00);
    outb(port + 3, 0x80);

    set_baud(port, BAUD_115200);    // set baud rate to 115200

    outb(port + 3, 0x03);
    outb(port + 2, 0xC7);
    outb(port + 4, 0x0B);
}

UI32 is_serial_bus_clear(UI16 port) {
    return inb(port + 5) & 0x20;
}

VOID serial_char_out(UI16 port, BYTE datb) {
    while (is_serial_bus_clear(port) == 0);

    outb(port, datb);
}

UI32 is_serial_recv(UI16 port) {
    return inb(port + 5) & 1;
}

BYTE serial_char_in(UI16 port) {
    while(is_serial_recv(port) == 0);

    return inb(port);
}
