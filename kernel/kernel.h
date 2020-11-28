#pragma once


// boot protocol
#include <sys/bootprotocol/stiletto.h>

// serial port
#include <sys/devices/serial/serial.h>


#include <stdint.h>

#define PutPixel(__x__,__y__,__pxfmt__) fb[__y__*ppsl+__x__]=__pxfmt__
