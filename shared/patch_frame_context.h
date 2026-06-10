#pragma once

#include <stdint.h>

typedef struct patch_frame_context
{
    const uintptr_t frame_base;
    uintptr_t frame_start;
    uintptr_t frame_size;
    uint64_t frame_consumed;
    dynlib_info frame_info;
} patch_frame_context;
