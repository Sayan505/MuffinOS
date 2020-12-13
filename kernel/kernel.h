#pragma once


#include <stdint.h>


// boot protocol
#include <sys/bootprotocol/stiletto.h>


// serial port
#include <sys/devices/serial/serial.h>

#define init_com1() init_serial(COM1)


// video
#include <sys/video/video.h>
