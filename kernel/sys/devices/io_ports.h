#pragma once


#include <sys/stdtypes.h>

/*Ports:***********************/
// COM
#define COM1 (0x3F8)
#define COM2 (0x2F8)

/******************************/

// Baud Rates
#define BAUD_115200 (0x01)
#define BAUD_57600  (0x02)
#define BAUD_38400  (0x03)


VOID outb(UI16 port, BYTE datb);
BYTE inb(UI16 port);
