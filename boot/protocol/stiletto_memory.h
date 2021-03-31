#pragma once


#include "../types.h"


typedef struct {
    void     *pMem_map;
    uint64_t map_sz;
    uint64_t desc_sz;
} stiletto_memory_t;
