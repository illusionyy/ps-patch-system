#pragma once

#include <stdint.h>

union ATTRIBUTE
{
    uint32_t v;
    struct
    {
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t kPS4Pro : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
    };
};

union ATTRIBUTE2
{
    uint32_t v;
    struct
    {
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t kLimitedGPUClock : 1;
        uint32_t kPS5 : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
    };
};

union ATTRIBUTE3
{
    uint32_t v;
    struct
    {
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t kHFR : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t kVRR : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
    };
};

union ATTRIBUTE4
{
    uint32_t v;
};

struct attr
{
    union ATTRIBUTE ATTRIBUTE;
    union ATTRIBUTE2 ATTRIBUTE2;
    union ATTRIBUTE3 ATTRIBUTE3;  // not valid on ps4
    union ATTRIBUTE4 ATTRIBUTE4;  // only valid on ps5 11.00
};
