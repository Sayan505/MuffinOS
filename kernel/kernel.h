#pragma once


#include <stdint.h>

// boot protocol
#include <sys/bootprotocol/stiletto.h>

// serial port
#include <sys/devices/serial/serial.h>


#define PutPixel(__x__,__y__,__pxfmt__) fb[__y__*ppsl+__x__]=__pxfmt__

#define init_com1() init_serial(COM1)