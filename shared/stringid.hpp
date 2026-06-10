#pragma once

#if defined(__cplusplus)

#include <stdint.h>
#include "macro.h"

#if !defined(cxx_constexpr)
#define cxx_constexpr __attribute__((noinline)) constexpr
#endif

#include "stringid.h"

cxx_constexpr StringId stringid(const char* str)
{
    return stringid_l(str, -1);
}

#define sid(s) stringid(s)

#else
#error "constexpr not available in c mode. please use `stringid.h` instead!"
#endif
