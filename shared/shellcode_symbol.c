#include "shellcode_symbol.h"
#include "notify.h"
#include <stdio.h>
#include <string.h>

#include "dynlib.h"

int write_syms_(const int pid, uint8_t* code_copy, const struct sym_entry* symlist, const size_t nsymlist, const char* func)
{
    int r = 0;
    for (size_t i = 0; i < nsymlist; i++)
    {
        const struct sym_entry* e = &symlist[i];
        uintptr_t addr = kernel_dynlib_dlsym(pid, e->handle, e->name);
        printf("%s[%ld]: %lx\n", e->name, i + 1, addr);
        if (addr)
        {
            memcpy(code_copy + e->offset, &addr, sizeof(addr));
        }
        else
        {
            notify_(e->exp,
                    "failed to resolve %s\n"
                    "aborting %s patch!\n",
                    func,
                    e->name);
            r += 1;
        }
    }
    return r;
}
