#pragma once

static constexpr double sz_to_mb(const double s)
{
    return s / (1024.0 * 1024.0);
}

#define print_load(h, proc, pid, sz, eval)                                  \
    {                                                                       \
        eval double d_mb = sz_to_mb(sz);                                    \
        h("loaded \"%s\" at pid %d image size %.2lfmb\n", proc, pid, d_mb); \
    }

#define print_load_const(h, proc, pid, sz) print_load(h, proc, pid, sz, const)
#define print_load_constexpr(h, proc, pid, sz) print_load(h, proc, pid, sz, constexpr)
