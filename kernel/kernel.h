#pragma once


#include <stdint.h>


// boot protocol
#include <sys/bootprotocol/stiletto.h>


// serial port
#include <sys/devices/serial/serial.h>


// video
#include <sys/video/video.h>
extern void init_video(stiletto_video_t stiletto_video);


// stdout
#include <sys/video/stdout/stdout.h>
