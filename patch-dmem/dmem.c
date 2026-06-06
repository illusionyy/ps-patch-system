#include <ps5/kernel.h>
#include <stdio.h>
#include "../shared/macro.h"
#include "../shared/notify.h"
#include "../shared/file.h"

static ssize_t match_line_in_file(const char* filename, const char* dbuf, const size_t dbufsz)
{
    FILE* f = fopen(filename, "rb");
    if (!f)
    {
        notify(
            "%s not found\n"
            "creating default\n",
            filename);
        if (write_file(filename, dbuf, dbufsz) == 0)
        {
            f = fopen(filename, "rb");
        }
    }
    // if opened in write default path, try again
    if (!f)
    {
        return -__LINE__;
    }
    char line[1024 + 1] = {};
    size_t n = 0;
    while (fgets(line, _countof_1(line), f))
    {
        char* p = line;
        while (*p == ' ' || *p == '\t')
        {
            p++;
        }

        if (*p == '#')
        {
            continue;
        }
        printf("%s", p);
        size_t add_n = 0;
        if (sscanf(p, "%ld %ld", &n, &add_n) == 2)
        {
            n += add_n;
        }
        break;
    }

    fclose(f);
    return n;
}

#define match_line_in_file_text(f, d) match_line_in_file(f, d, _countof_1(d))

static void print_u64(const uintptr_t kaddr, const char* kn)
{
    size_t buffer = 0;
    kernel_copyout(kaddr, &buffer, sizeof(buffer));
    printf("%s (0x%lx): %ld (0x%lx)\n", kn, kaddr, buffer, buffer);
}

#define print_u64(k) print_u64(k, #k)

// int kernel_copyout(unsigned long kaddr, void *uaddr, unsigned long len)
// int32_t kernel_copyin(const void *udaddr, intptr_t kaddr, size_t len)

int main(void)
{
    const ssize_t dmem = match_line_in_file_text("/data/user/ps4_dmem.txt", "4736 0\n");
    printf("dmem %ld\n", dmem);
    const size_t base_dmem = 4736;
    const size_t base_dmem2x = base_dmem * 2;
    const size_t mb = (1024 * 1024);
    if (dmem >= base_dmem && dmem <= base_dmem2x)
    {
        const size_t dmem_mul = dmem * mb;
        printf("dmem mul 0x%lx\n", dmem_mul);
        // TODO: only for demo, make this a pattern
        const uintptr_t dmem_addr = (KERNEL_ADDRESS_TEXT_BASE + (0xffffffffc3d314a0 - 0xffffffffc3390000));
        print_u64(dmem_addr);
        kernel_copyin(&dmem_mul, dmem_addr, sizeof(dmem_mul));
        print_u64(dmem_addr);
        notify("patched ps4 direct memory size from %ldmb to %ldmb\n", base_dmem, dmem);
    }
    return 0;
}
