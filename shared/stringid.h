#pragma once

#include <stdint.h>

#if !defined(hash_base)
#define hash_base 0x811c9dc5
#endif

#if !defined(cxx_constexpr)
#define cxx_constexpr
#endif

typedef uint32_t StringId;

cxx_constexpr static StringId stringid_l(const char* str, int64_t len)
{
    StringId base = hash_base;
    while (*str && len--)
    {
        base = 0x01000193 * (base ^ *str++);
    }
    return base;
}

#define sid_len(s, l) stringid_l(s, l)
#define sid_startswith(s) stringid_l(s, _countof_1(s))
