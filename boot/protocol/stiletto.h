#pragma once


#include "stiletto_video.h"
#include "stiletto_acpi.h"
#include "stiletto_memory.h"

typedef struct {
    stiletto_video_t  bootprot_video;
    stiletto_acpi_t   bootprot_acpi;
    stiletto_memory_t bootprot_memory;
} stiletto_t;
