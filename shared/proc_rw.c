#include "proc_rw.h"
#include "my_assert.h"

#include "../shared/platform_mdbg.h"
#include "../shared/platform_kernel.h"
#include <stdlib.h>
#include <sys/user.h>
#include <stdio.h>

int userland_copyout_(pid_t pid, intptr_t addr, void* buf, size_t len, const char* user)
{
    // unpatched on 8.20+
    const int r = mdbg_copyout(pid, addr, buf, len);
    if (0 && r == 0)
    {
        printf("%s: copied out okay from %s\n", __FUNCSIG__, user);
    }
    return r;
}

#if defined(__PROSPERO__)
static unsigned int get_fw_version(void)
{
    static int cache_fw_version = 0;
    if (cache_fw_version == 0)
    {
        cache_fw_version = kernel_get_fw_version() >> 16;
    }
    return cache_fw_version;
}

static unsigned long vmspace_pmap(unsigned long vmspace_kaddr)
{
    switch (get_fw_version())
    {
        case 0x100 ... 0x102:
            return vmspace_kaddr + 0x2C0;
        case 0x105 ... 0x550:
            return vmspace_kaddr + 0x2E0;
        case 0x600 ... 0x1200:
            return vmspace_kaddr + 0x2E8;
        default:
            return 0;  // unsupported fw version
    }
}

static unsigned long cache_kernel_dmap_base = 0;

static unsigned long kernel_get_proc_cr3(int pid)
{
    unsigned long proc = kernel_get_proc(pid);
    if (proc == 0)
    {
        return 0;
    }

    unsigned long vmspace = kernel_getlong(proc + KERNEL_OFFSET_PROC_P_VMSPACE);
    if (vmspace == 0)
    {
        return 0;
    }

    // read pm_pml4 + pm_cr3, its free to read the extra 8 bytes and this way we can populate the dmap base cache
    unsigned long pmap = vmspace_pmap(vmspace);
    if (pmap == 0)
    {
        return 0;
    }
    unsigned long data[2];
    if (kernel_copyout(pmap + 32, data, sizeof(data)))
    {
        return 0;
    }

    unsigned long pm_pml4 = data[0];  // KVA of level 4 page table - always within the direct map
    unsigned long pm_cr3 = data[1];

    cache_kernel_dmap_base = pm_pml4 - pm_cr3;
    return pm_cr3;
}

static unsigned long kernel_get_dmap_base(void)
{
    if (cache_kernel_dmap_base == 0)
    {
        kernel_get_proc_cr3(-1);  // implicitly sets cache_kernel_dmap_base
    }

    return cache_kernel_dmap_base;
}

// based on:
// https://github.com/sleirsgoevy/ps4jb-payloads/blob/6f2aad7c4773591913aecea405c54bb547d12822/ps5-kstuff/uelf/utils.c#L8
// https://github.com/sleirsgoevy/ps4jb-payloads/blob/6f2aad7c4773591913aecea405c54bb547d12822/ps5-kstuff/main.c#L235
static unsigned long virt2phys(unsigned long cr3, unsigned long va, unsigned long* phys_limit)
{
    unsigned long kernel_dmap_base = kernel_get_dmap_base();

    cr3 &= PG_FRAME;

    // PML4SHIFT (39) -> PDPSHIFT (30) -> PDRSHIFT (21) -> PAGE_SHIFT (12)
    for (int i = 39; i >= 12; i -= 9)
    {
        unsigned long index = (va >> i) & ((1ull << 9) - 1);
        unsigned long entry_offset = index * sizeof(unsigned long);  // pml4_entry_t is unsigned long
        cr3 = kernel_getlong(kernel_dmap_base + cr3 + entry_offset);

        if (!(cr3 & X86_PG_V))
        {  // not present/valid
            return -1;
        }
        if ((cr3 & X86_PG_PS) || i == 12)
        {                                       // large page or last level
            cr3 &= (1ull << 52) - (1ull << i);  // PG_PS_FRAME/PG_FRAME
            cr3 |= va & ((1ull << i) - 1);      // PDPMASK/PDRMASK/PAGE_MASK
            if (phys_limit)
            {
                *phys_limit = (cr3 | ((1ull << i) - 1)) + 1;
            }
            return cr3;
        }
        cr3 &= PG_FRAME;
    }

    assert_always(0 && "unreachable code reached");
    return 0;
}

static int phys_copy_to_remote(unsigned long cr3, const void* from_local_vaddr, unsigned long to_remote_vaddr, unsigned long len)
{
    const unsigned char* p_src = from_local_vaddr;
    unsigned long phys, phys_end;
    unsigned long kernel_dmap_base = kernel_get_dmap_base();
    if (kernel_dmap_base == 0)
    {
        return -1;
    }
    while (len)
    {
        phys = virt2phys(cr3, to_remote_vaddr, &phys_end);
        if (phys == -1)
        {
            return -1;
        }
        size_t chk = phys_end - phys;
        if (len < chk)
        {
            chk = len;
        }

        if (kernel_copyin(p_src, kernel_dmap_base + phys, chk))
        {
            return -1;
        }

        to_remote_vaddr += chk;
        p_src += chk;
        len -= chk;
    }
    return 0;
}
#endif

int userland_copyin_(pid_t pid, const void* buf, intptr_t addr, size_t len, const char* user)
{
#if defined(__ORBIS__)
    const int r = mdbg_copyin(pid, buf, addr, len);
    if (0 && r == 0)
    {
        printf("%s: copied in okay from %s\n", __FUNCSIG__, user);
    }
    return r;
#elif defined(__PROSPERO__)
    // mdbg copyin works up to 8.20
    if (get_fw_version() <= 0x820)
    {
        const int r = mdbg_copyin(pid, buf, addr, len);
        if (0 && r == 0)
        {
            printf("%s: copied in okay from %s\n", __FUNCSIG__, user);
        }
        return r;
    }
    // make sure the target has the addr faulted in
    void* tmp = malloc(len);
    if (tmp == NULL)
    {
        return -1;
    }

    int res = userland_copyout(pid, addr, tmp, len);
    printf("userland_copyout at 0x%lx is %d\n", addr, res);
    free(tmp);
    if (res)
    {
        return -1;
    }

    unsigned long cr3 = kernel_get_proc_cr3(pid);
    if (cr3 == 0)
    {
        return -1;
    }

    conet int r = phys_copy_to_remote(cr3, buf, addr, len);
    printf("phys_copy_to_remote at 0x%lx is %d\n", addr, r);
    return r;
#endif
}

int userland_setchar_(int pid, unsigned long addr, char val, const char* user)
{
    return userland_copyin_(pid, &val, addr, sizeof(val), user);
}

int userland_setshort_(int pid, unsigned long addr, short val, const char* user)
{
    return userland_copyin_(pid, &val, addr, sizeof(val), user);
}

int userland_setint_(int pid, unsigned long addr, int val, const char* user)
{
    return userland_copyin_(pid, &val, addr, sizeof(val), user);
}

int userland_setlong_(int pid, unsigned long addr, long val, const char* user)
{
    return userland_copyin_(pid, &val, addr, sizeof(val), user);
}

long userland_getlong_(int pid, unsigned long addr, const char* user)
{
    long val = 0;
    userland_copyout_(pid, addr, &val, sizeof(val), user);
    return val;
}

int userland_getint_(int pid, unsigned long addr, const char* user)
{
    int val = 0;
    userland_copyout_(pid, addr, &val, sizeof(val), user);
    return val;
}

short userland_getshort_(int pid, unsigned long addr, const char* user)
{
    short val = 0;
    userland_copyout_(pid, addr, &val, sizeof(val), user);
    return val;
}

char userland_getchar_(int pid, unsigned long addr, const char* user)
{
    char val = 0;
    userland_copyout_(pid, addr, &val, sizeof(val), user);
    return val;
}
