#pragma once

#include "macro.h"
#include <stdint.h>
#include <stddef.h>
#include <sys/cdefs.h>

struct sym_entry
{
    const char* name;
    uint32_t handle;
    uintptr_t offset;
    const char* exp;
};

#define sym_offset(name) name##_offset
#define sym_hook(name) __export_##name##_hook_offset
#define sym(name, handle)                                                          \
    {                                                                              \
        #name, handle, sym_offset(name), FUNC_LINE ": sym(" #name ", " #handle ")" \
    }
__BEGIN_DECLS
int write_syms_(const int pid, uint8_t* code_copy, const struct sym_entry* symlist, const size_t nsymlist, const char* func);
__END_DECLS
#define write_syms(p, c, s, sl) write_syms_(p, c, s, sl, FILE_FUNC_LINE)
