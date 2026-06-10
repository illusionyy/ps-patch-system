#pragma once

#include <stdio.h>
#include "macro.h"

#define debug_print_always(a, ...) printf(FILE_FUNC_LINE ": " a, ##__VA_ARGS__)
#define debugf debug_print_always
#define func_ret(e) debugf(#e " returned %d\n", e)
#define func_ret_hex(e) debugf(#e " returned %x\n", e)
#define func_ret_fmt(e, f) debugf(#e " returned " f "\n", e)
#define perror_on_cond(e, v, c, es)         \
    e;                                      \
    if (c)                                  \
    {                                       \
        perror(#c " because " es);          \
        debugf(es ": %d (0x%08x)\n", v, v); \
    }
#define perror_on_non_zero(e, v) perror_on_cond(e, v, v != 0, #e)
#define perror_on_less_zero(e, v) perror_on_cond(e, v, v < 0, #e)
#define perror_on_greater_zero(e, v) perror_on_cond(e, v, v > 0, #e)
