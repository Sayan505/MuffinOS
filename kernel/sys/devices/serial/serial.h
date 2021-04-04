#pragma once


#include <sys/stdtypes.h>

#include <sys/devices/io_ports.h>


// init serial
VOID init_serial(UI16 port);

// serial in
UI32  is_serial_bus_clear(UI16 port);
VOID  serial_char_out(UI16 port, BYTE datb);

// serial out
UI32 is_serial_recv(UI16 port);
BYTE serial_char_in(UI16 port);

// set baud rate
VOID set_baud(UI16 port, BYTE baud_rate);


#define init_com1()        init_serial(COM1)
#define com1_out(__datb__) serial_char_out(COM1, __datb__)
#define com1_in()          serial_char_in(COM1)
