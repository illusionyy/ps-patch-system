#include <sys/types.h>
#include <sys/user.h>
#include <sys/sysctl.h>

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../shared/file.h"
#include "../shared/path.h"
#include "../shared/debug.h"
#include "../shared/macro.h"
#include "../shared/proc_rw.h"
#include "../shared/stringid.h"
#include "memory.h"

#define BACKUP_VER 22761  // not random nya, 22761 does mean something.
int sceKernelUsleep(int);

uintptr_t pid_chunk_scan_data(const int pid, const uintptr_t mem_start, const uintptr_t mem_sz, const void* data, const size_t data_sz, const size_t pattern_offset)
{
#define chunk_size (8ul * 1024)
    uintptr_t found = 0;
    for (size_t i = 0; i < (mem_sz - chunk_size); i += chunk_size)
    {
        uint8_t mem[chunk_size];
        const uintptr_t chunk_start = mem_start + i;
        userland_copyout(pid, chunk_start, mem, sizeof(mem));
        for (size_t j = 0; j < (chunk_size - data_sz); j++)
        {
            if (memcmp((void*)(mem + j), data, data_sz) == 0)
            {
                const uintptr_t chunk_offset = (uintptr_t)(mem + j) - (uintptr_t)mem;
                const uintptr_t chunk_found = chunk_start + chunk_offset + pattern_offset;
                found = chunk_found;
                debugf("found data at chunk loc 0x%lx offset 0x%lx result 0x%lx\n",
                       chunk_start,
                       chunk_offset,
                       chunk_found);
                break;
            }
        }
        sceKernelUsleep(1);
    }
    if (!found)
    {
        debugf(
            "couldn't find data\n"
            "pid %d\n",
            pid);
    }
    return found;
#undef chunk_size
}

uintptr_t pid_chunk_scan(const int pid, const uintptr_t mem_start, const uintptr_t mem_sz, const char* pattern, const size_t pattern_offset)
{
#define chunk_size (8ul * 1024)
    uintptr_t found = 0;
    for (size_t i = 0; i < (mem_sz - chunk_size); i += chunk_size)
    {
        uint8_t mem[chunk_size];
        const uintptr_t chunk_start = mem_start + i;
        userland_copyout(pid, chunk_start, mem, sizeof(mem));
        // sys_proc_rw(pid, chunk_start, (const void*)mem, chunk_size, 0);
        const uintptr_t mem_start = (uintptr_t)mem;
        const uintptr_t found_pattern = PatternScan(mem, chunk_size, pattern, pattern_offset);
        if (found_pattern)
        {
            const uintptr_t chunk_offset = found_pattern - mem_start;
            const uintptr_t chunk_found = chunk_start + chunk_offset;
            found = chunk_found;
            debugf("found data at 0x%lx 0x%lx chunk loc 0x%lx offset 0x%lx\n", found_pattern, chunk_offset, chunk_start, chunk_found);
            break;
        }
        sceKernelUsleep(10);
    }
    if (!found)
    {
        debugf(
            "couldn't find pattern\n"
            "%s\n"
            "pid %d\n",
            pattern,
            pid);
    }
    return found;
#undef chunk_size
}

static int isprint(int c)
{
    return ((unsigned)c - 0x20) < 0x5f;
}

void hex_dump(const void* data, const size_t size, const uintptr_t real)
{
#define kMaxBytesPrint 16
#define kMaxFormatChars " xx | ..\n"  // hex bytes and ascii
    const unsigned char* p = (const unsigned char*)data;
    for (uintptr_t i = 0; i < size; i += kMaxBytesPrint)
    {
        char buf[(_countof(kMaxFormatChars) * kMaxBytesPrint) + 1] = {0};
        char* cur = buf;
        char* end = buf + _countof_1(buf);
        cur += snprintf(cur, end - cur, "0x%016lx[+0x%04lx]:  ", real + i, i);
        for (uintptr_t j = 0; j < kMaxBytesPrint; j++)
        {
            if (i + j < size)
            {
                cur += snprintf(cur, end - cur, "%02x ", p[i + j]);
            }
            else
            {
                cur += snprintf(cur, end - cur, "   ");
            }
        }
        cur += snprintf(cur, end - cur, " |");
        for (uintptr_t j = 0; j < kMaxBytesPrint && i + j < size; j++)
        {
            if (cur < end)
            {
                *cur++ = isprint(p[i + j]) ? (char)p[i + j] : '.';
            }
        }
        cur += snprintf(cur, end - cur, "|");
        puts(buf);
    }
#undef kMaxBytesPrint
#undef kMaxFormatChars
}

int32_t pid_write_call_(const pid_t pid, const uintptr_t addr, const uintptr_t dest, const bool jmp, const char* user)
{
    int32_t offset = dest - addr - 5;
    userland_setchar_(pid, addr, !jmp ? 0xe8 : 0xe9, user);
    userland_setint_(pid, addr + 1, offset, user);
    printf("%s: write %s from %lx to %lx\n", user, jmp ? "jmp" : "call", addr, dest);
    return 0;
}

int32_t pid_write_call2_(const pid_t pid, const uintptr_t addr, const uintptr_t dest, const bool jmp, const char* user)
{
    // beacuse of xom, i need to use `a` register, reading memory is not an option
    userland_setchar_(pid, addr + 0, 0x48, user);
    userland_setchar_(pid, addr + 1, 0xB8, user);  // rax
    userland_setlong_(pid, addr + 2, dest, user);
    userland_setchar_(pid, addr + 10, 0xFF, user);  // rax
    userland_setchar_(pid, addr + 11, jmp ? 0xE0 : 0xD0, user);
    printf("%s: write %s from %lx to %lx\n", user, jmp ? "jmp" : "call", addr, dest);
    return 0;
}

uintptr_t pid_read_call_(const pid_t pid, const uintptr_t addr, const char* user)
{
    const int32_t r = userland_getint_(pid, addr + 1, user);
    if (r)
    {
        return (addr + r) + 5;
    }
    return 0;
}

// https://github.com/sleirsgoevy/ps4-hamachi/blob/d41f328fb587cc17e567845ed314f89a2255976c/app/app/getfw.c#L6
int dynlib_get_obj_member(const uint32_t module_id, const size_t which, void** out);

int get_firmware_version_from_disk(uint32_t* out)
{
    static uint32_t cached = 0;
    if (!cached)
    {
        void* sce_proc_param = 0;
        int r = 0;
        const uint32_t libkernel_sys = 0x1;
        perror_on_non_zero(r = dynlib_get_obj_member(libkernel_sys, 8, &sce_proc_param), r);
        if (r || !sce_proc_param)
        {
            return -__LINE__;
        }
        const uint32_t* spp = sce_proc_param;
// on ps5, 4 is ps4 ver, 5 is ps5 ver
#if defined(__ORBIS__)
        const size_t fw_idx = 4;
#elif defined(__PROSPERO__)
        const size_t fw_idx = 5;
#endif
        *out = cached = spp[fw_idx];
        return 0;
    }
    else
    {
        *out = cached;
        return 0;
    }
    return -__LINE__;
}

static void make_backup_path(void)
{
    mkdirs(BACKUP_DATA_PATCH_PATH "/");
}

void gen_backup_path(const char* file, const char* pattn, const size_t pattn_offset, char* buf, const size_t len)
{
    make_backup_path();
    static uint32_t fw = 0;
    int r = 0;
    if (!fw)
    {
        func_ret(r = get_firmware_version_from_disk(&fw));
    }
    if (r == 0)
    {
        char pattern_buf[1024 + 1] = {};
        snprintf(pattern_buf, _countof_1(pattern_buf), "%s:%ld", pattn, pattn_offset);
        static StringId bn = 0xff;
        bn = bn != 0xff ?: stringid_l("BACKUP_VER: " dstr(BACKUP_VER), -1);
        const StringId fn = stringid_l(pattn, -1) ^ stringid_l(file, -1) ^ bn;
        snprintf(buf, len, BACKUP_DATA_PATCH_PATH "/fw_0x%08x_0x%08x_v%d.bin", fw, fn, BACKUP_VER);
    }
}

int write_backup_to_disk_(const char* path, const void* data, const size_t len, const char* user)
{
    return write_file(path, data, len);
}

int read_backup_from_disk_(const char* path, void* data, const size_t len, const char* user)
{
    return read_file(path, data, len);
}

int restore_cached_backup_(const pid_t pid, const uintptr_t mapbase, const char* patch_buf, void* backup, const size_t backup_size, const size_t backup_bytes_size, uintptr_t* out_addr, const bool is_ptr, const char* user)
{
    if (file_exists(patch_buf) != SUCCESS)
    {
        return -__LINE__;
    }
    int r = 0;
    func_ret(r = read_backup_from_disk_(patch_buf, backup, backup_size, user));
    printf("read %s for %s = %d\n", patch_buf, user, r);
    if (r == 0)
    {
        uintptr_t* pBackup = backup;
        if (!pBackup[0])
        {
            return -__LINE__;
        }
        pBackup[0] += mapbase;
        pBackup[1] += is_ptr ? mapbase : 0;
        *out_addr = pBackup[is_ptr ? 1 : 0];
        if (is_ptr)
        {
            printf("base pBackup[0]: %lx\n", pBackup[0] - mapbase);
            printf("base pBackup[1]: %lx\n", pBackup[1] - mapbase);

            printf("absolute pBackup[0]: %lx\n", pBackup[0]);
            printf("absolute pBackup[1]: %lx\n", pBackup[1]);
            func_ret(userland_copyin(pid, &pBackup[0], pBackup[1], sizeof(pBackup[0])));
            // func_ret(userland_copyin(pid, &pBackup[1], pBackup[0], sizeof(pBackup[1])));
        }
        else
        {
            const uint8_t* bytes = (const uint8_t*)backup + sizeof(uintptr_t);
            printf("bytes size %ld\n", backup_bytes_size);
            hex_dump(bytes, backup_bytes_size, 0);
            func_ret(userland_copyin(pid, bytes, pBackup[0], backup_bytes_size));
        }
        return 1;
    }
    return -__LINE__;
}
