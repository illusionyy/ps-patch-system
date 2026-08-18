#include "proc_rw.h"

#include "../shared/platform_kernel.h"
#include "../shared/platform_mdbg.h"

#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/user.h>

#if defined(__PROSPERO__)
#define COPYIN_LOCK_ATTEMPTS 1000000UL

static volatile int copyin_lock = 0;

static int lock_copyin(void)
{
    for (unsigned long attempt = 0; attempt < COPYIN_LOCK_ATTEMPTS; ++attempt)
    {
        if (__atomic_exchange_n(&copyin_lock, 1, __ATOMIC_ACQUIRE) == 0)
        {
            return 0;
        }
    }
    return -1;
}

static void unlock_copyin(void)
{
    __atomic_store_n(&copyin_lock, 0, __ATOMIC_RELEASE);
}
#endif

int userland_copyout_(pid_t pid, intptr_t addr, void* buf, size_t len, const char* user)
{
    // mdbg copyout is unpatched on 8.20+.
    const int result = mdbg_copyout(pid, addr, buf, len);
    if (0 && result == 0)
    {
        printf("%s: copied out okay from %s\n", __FUNCSIG__, user);
    }
    return result;
}

#if defined(__PROSPERO__)
static int checked_add_ulong(unsigned long left, unsigned long right, unsigned long* result)
{
    if (result == NULL || right > ULONG_MAX - left)
    {
        return -1;
    }
    *result = left + right;
    return 0;
}

static int valid_user_range(intptr_t addr, size_t len)
{
    if (addr < 0 || len == 0)
    {
        return len == 0;
    }

    const unsigned long start = (unsigned long)addr;
    unsigned long end = 0;
    if ((unsigned long)(len - 1) > ULONG_MAX ||
        checked_add_ulong(start, (unsigned long)(len - 1), &end) != 0)
    {
        return 0;
    }

    // Target process mappings used here must be in the low canonical half.
    return (start >> 47) == 0 && (end >> 47) == 0;
}

static int valid_local_range(const void* buffer, size_t len)
{
    if (len == 0)
    {
        return 1;
    }
    if (buffer == NULL)
    {
        return 0;
    }

    const uintptr_t start = (uintptr_t)buffer;
    return (len - 1) <= UINTPTR_MAX - start;
}

#define MAX_PROC_SCAN_COUNT 65536UL
#define PHYSICAL_ADDRESS_LIMIT (1UL << 52)

typedef struct remote_address_space
{
    pid_t pid;
    unsigned long proc;
    unsigned long vmspace;
    unsigned long pmap;
    unsigned long pml4;
    unsigned long cr3;
    unsigned long dmap;
} remote_address_space;

static int is_canonical_address(unsigned long address)
{
    const unsigned long high = address >> 47;
    return high == 0 || high == 0x1FFFF;
}

static int is_kernel_pointer(unsigned long address)
{
    return address != 0 && is_canonical_address(address) && (address & (1UL << 63)) != 0;
}

static unsigned int get_fw_version(void)
{
    static unsigned int cached_fw_version = 0;
    if (cached_fw_version == 0)
    {
        cached_fw_version = kernel_get_fw_version() >> 16;
    }
    return cached_fw_version;
}

static unsigned long vmspace_pmap_offset(void)
{
    switch (get_fw_version())
    {
        case 0x100 ... 0x102:
            return 0x2C0;
        case 0x105 ... 0x550:
            return 0x2E0;
        case 0x600 ... 0x1340:
            return 0x2E8;
        default:
            return 0;
    }
}

static int add_kernel_offset(unsigned long base, off_t offset, unsigned long* result)
{
    if (offset < 0)
    {
        return -1;
    }
    return checked_add_ulong(base, (unsigned long)offset, result);
}

static int read_proc_pid(unsigned long proc, pid_t* pid)
{
    unsigned long address = 0;
    if (!is_kernel_pointer(proc) || pid == NULL ||
        add_kernel_offset(proc, KERNEL_OFFSET_PROC_P_PID, &address) != 0 ||
        !is_kernel_pointer(address))
    {
        return -1;
    }

    unsigned int value = 0;
    if (kernel_copyout(address, &value, sizeof(value)) != 0)
    {
        return -1;
    }
    *pid = (pid_t)value;
    return 0;
}

static int find_proc_uncached(pid_t requested_pid, unsigned long* result)
{
    if (result == NULL)
    {
        return -1;
    }

    const pid_t pid = requested_pid > 0 ? requested_pid : getpid();
    const unsigned long allproc = (unsigned long)KERNEL_ADDRESS_ALLPROC;
    unsigned long proc = 0;
    if (!is_kernel_pointer(allproc) ||
        kernel_copyout(allproc, &proc, sizeof(proc)) != 0)
    {
        return -1;
    }

    for (unsigned long count = 0; count < MAX_PROC_SCAN_COUNT && proc != 0; ++count)
    {
        if (!is_kernel_pointer(proc))
        {
            return -1;
        }

        pid_t current_pid = 0;
        if (read_proc_pid(proc, &current_pid) != 0)
        {
            return -1;
        }
        if (current_pid == pid)
        {
            *result = proc;
            return 0;
        }

        unsigned long next = 0;
        if (kernel_copyout(proc, &next, sizeof(next)) != 0 || next == proc)
        {
            return -1;
        }
        proc = next;
    }

    return -1;
}

static int read_proc_vmspace(unsigned long proc, unsigned long* vmspace)
{
    unsigned long address = 0;
    if (!is_kernel_pointer(proc) || vmspace == NULL ||
        add_kernel_offset(proc, KERNEL_OFFSET_PROC_P_VMSPACE, &address) != 0 ||
        !is_kernel_pointer(address) ||
        kernel_copyout(address, vmspace, sizeof(*vmspace)) != 0 ||
        !is_kernel_pointer(*vmspace))
    {
        return -1;
    }
    return 0;
}

static int read_pmap_words(unsigned long pmap, unsigned long* pml4, unsigned long* cr3)
{
    unsigned long words_address = 0;
    unsigned long words[2] = {0};
    if (!is_kernel_pointer(pmap) || pml4 == NULL || cr3 == NULL ||
        checked_add_ulong(pmap, 0x20, &words_address) != 0 ||
        !is_kernel_pointer(words_address) ||
        kernel_copyout(words_address, words, sizeof(words)) != 0)
    {
        return -1;
    }

    *pml4 = words[0];
    *cr3 = words[1];
    return 0;
}

static int load_remote_address_space(pid_t requested_pid, remote_address_space* space)
{
    if (space == NULL)
    {
        return -1;
    }

    memset(space, 0, sizeof(*space));
    space->pid = requested_pid > 0 ? requested_pid : getpid();

    const unsigned long pmap_offset = vmspace_pmap_offset();
    if (pmap_offset == 0 || find_proc_uncached(space->pid, &space->proc) != 0)
    {
        return -1;
    }

    pid_t observed_pid = 0;
    if (read_proc_pid(space->proc, &observed_pid) != 0 || observed_pid != space->pid ||
        read_proc_vmspace(space->proc, &space->vmspace) != 0 ||
        checked_add_ulong(space->vmspace, pmap_offset, &space->pmap) != 0 ||
        !is_kernel_pointer(space->pmap) ||
        read_pmap_words(space->pmap, &space->pml4, &space->cr3) != 0)
    {
        return -1;
    }

    const unsigned long cr3_frame = space->cr3 & PG_FRAME;
    if (!is_kernel_pointer(space->pml4) || (space->pml4 & 0xFFF) != 0 ||
        cr3_frame == 0 || cr3_frame >= PHYSICAL_ADDRESS_LIMIT ||
        space->pml4 < cr3_frame)
    {
        return -1;
    }

    space->dmap = space->pml4 - cr3_frame;
    if (!is_kernel_pointer(space->dmap) ||
        read_proc_pid(space->proc, &observed_pid) != 0 || observed_pid != space->pid)
    {
        return -1;
    }

    unsigned long observed_vmspace = 0;
    if (read_proc_vmspace(space->proc, &observed_vmspace) != 0 ||
        observed_vmspace != space->vmspace)
    {
        return -1;
    }

    return 0;
}

static int revalidate_remote_address_space(const remote_address_space* space)
{
    unsigned long current_proc = 0;
    unsigned long current_vmspace = 0;
    unsigned long current_pml4 = 0;
    unsigned long current_cr3 = 0;
    pid_t observed_pid = 0;

    if (space == NULL || find_proc_uncached(space->pid, &current_proc) != 0 ||
        current_proc != space->proc ||
        read_proc_pid(space->proc, &observed_pid) != 0 || observed_pid != space->pid ||
        read_proc_vmspace(space->proc, &current_vmspace) != 0 ||
        current_vmspace != space->vmspace ||
        read_pmap_words(space->pmap, &current_pml4, &current_cr3) != 0 ||
        current_pml4 != space->pml4 || current_cr3 != space->cr3)
    {
        return -1;
    }
    return 0;
}

static int virt2phys(const remote_address_space* space,
                     unsigned long virtual_address,
                     unsigned long* physical_address,
                     size_t* page_remaining)
{
    if (space == NULL || physical_address == NULL || page_remaining == NULL ||
        (virtual_address >> 47) != 0)
    {
        return -1;
    }

    unsigned long table = space->cr3 & PG_FRAME;
    for (int shift = 39; shift >= 12; shift -= 9)
    {
        if (table == 0 || table >= PHYSICAL_ADDRESS_LIMIT)
        {
            return -1;
        }

        const unsigned long index = (virtual_address >> shift) & 0x1FF;
        const unsigned long entry_offset = index * sizeof(unsigned long);
        unsigned long table_address = 0;
        unsigned long entry_address = 0;
        unsigned long entry = 0;
        if (checked_add_ulong(space->dmap, table, &table_address) != 0 ||
            checked_add_ulong(table_address, entry_offset, &entry_address) != 0 ||
            !is_kernel_pointer(entry_address) ||
            kernel_copyout(entry_address, &entry, sizeof(entry)) != 0 ||
            (entry & X86_PG_V) == 0)
        {
            return -1;
        }

        const int is_large_page = (entry & X86_PG_PS) != 0;
        if (is_large_page && shift != 30 && shift != 21)
        {
            return -1;
        }

        if (is_large_page || shift == 12)
        {
            const unsigned long page_size = 1UL << shift;
            const unsigned long page_offset = virtual_address & (page_size - 1);
            const unsigned long frame_mask = (1UL << 52) - page_size;
            const unsigned long frame = entry & frame_mask;
            const unsigned long physical = frame | page_offset;
            if (frame >= PHYSICAL_ADDRESS_LIMIT || physical >= PHYSICAL_ADDRESS_LIMIT)
            {
                return -1;
            }

            *physical_address = physical;
            *page_remaining = (size_t)(page_size - page_offset);
            return *page_remaining != 0 ? 0 : -1;
        }

        table = entry & PG_FRAME;
    }

    return -1;
}

static int phys_copy_to_remote(const remote_address_space* space,
                               const void* source_buffer,
                               unsigned long remote_address,
                               size_t len)
{
    if (space == NULL || (source_buffer == NULL && len != 0))
    {
        return -1;
    }

    const unsigned char* source = source_buffer;
    size_t remaining = len;
    while (remaining != 0)
    {
        unsigned long physical = 0;
        size_t page_remaining = 0;
        if (revalidate_remote_address_space(space) != 0 ||
            virt2phys(space, remote_address, &physical, &page_remaining) != 0)
        {
            return -1;
        }

        const size_t chunk = remaining < page_remaining ? remaining : page_remaining;
        unsigned long kernel_target = 0;
        unsigned long kernel_target_end = 0;
        if (chunk == 0 ||
            checked_add_ulong(space->dmap, physical, &kernel_target) != 0 ||
            checked_add_ulong(kernel_target, (unsigned long)(chunk - 1), &kernel_target_end) != 0 ||
            !is_kernel_pointer(kernel_target) || !is_kernel_pointer(kernel_target_end))
        {
            return -1;
        }

        unsigned long verified_physical = 0;
        size_t verified_page_remaining = 0;
        if (revalidate_remote_address_space(space) != 0 ||
            virt2phys(space, remote_address, &verified_physical, &verified_page_remaining) != 0 ||
            verified_physical != physical || verified_page_remaining < chunk)
        {
            return -1;
        }

        if (kernel_copyin(source, kernel_target, chunk) != 0)
        {
            return -1;
        }

        unsigned long next_remote = 0;
        if (checked_add_ulong(remote_address, (unsigned long)chunk, &next_remote) != 0)
        {
            return -1;
        }
        remote_address = next_remote;
        source += chunk;
        remaining -= chunk;
    }

    return 0;
}

static int validate_copyin_arguments(const void* buffer, intptr_t addr, size_t len)
{
    return valid_local_range(buffer, len) && valid_user_range(addr, len) ? 0 : -1;
}

static int copyin_physical(pid_t pid, const void* buf, intptr_t addr, size_t len)
{
    // Fault every target page in before walking its page tables.
    unsigned char* original = malloc(len);
    if (original == NULL)
    {
        return -1;
    }
    if (mdbg_copyout(pid, addr, original, len) != 0)
    {
        free(original);
        return -1;
    }
    free(original);

    remote_address_space space;
    if (load_remote_address_space(pid, &space) != 0)
    {
        return -1;
    }
    return phys_copy_to_remote(&space, buf, (unsigned long)addr, len);
}
#endif

int userland_copyin_(pid_t pid, const void* buf, intptr_t addr, size_t len, const char* user)
{
#if defined(__ORBIS__)
    const int result = mdbg_copyin(pid, buf, addr, len);
    if (0 && result == 0)
    {
        printf("%s: copied in okay from %s\n", __FUNCSIG__, user);
    }
    return result;
#elif defined(__PROSPERO__)
    // mdbg copyin works up to 8.20.
    if (get_fw_version() <= 0x820)
    {
        const int result = mdbg_copyin(pid, buf, addr, len);
        if (0 && result == 0)
        {
            printf("%s: copied in okay from %s\n", __FUNCSIG__, user);
        }
        return result;
    }

    if (len == 0)
    {
        return 0;
    }
    if (validate_copyin_arguments(buf, addr, len) != 0)
    {
        return -1;
    }

    if (lock_copyin() != 0)
    {
        return -1;
    }
    const int result = copyin_physical(pid, buf, addr, len);
    unlock_copyin();
    return result;
#else
    (void)pid;
    (void)buf;
    (void)addr;
    (void)len;
    (void)user;
    return -1;
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
