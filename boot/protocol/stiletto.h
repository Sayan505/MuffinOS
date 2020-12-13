#pragma once


#include "stiletto_video.h"
#include "stiletto_acpi.h"
#include "stiletto_memory.h"


typedef struct {
    stiletto_video_t  stiletto_video;
    stiletto_acpi_t   stiletto_acpi;
    stiletto_memory_t stiletto_memory;
} stiletto_t;
