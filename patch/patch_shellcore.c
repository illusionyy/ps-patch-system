#include <sys/types.h>
#include <sys/user.h>
#include <sys/sysctl.h>  // kinfo_proc and stuff
#include <sys/file.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

#include "../shared/file.h"
#include "../shared/debug.h"

#include <sys/mman.h>
#include "../shared/platform_kernel.h"
#include "../shared/dynlib.h"

#include "../shared/debug.h"
#include "../shared/memory.h"
#include "../shared/macro.h"
#include "../shared/notify.h"
#include "../shared/proc_rw.h"

#include "../shared/patch_frame_context.h"
#include "../shared/shellcode_symbol.h"

#include "shellcore_onNewProcess.h"

#include <signal.h>
#include <sys/signal.h>

extern bool is_ps4;

struct pattern_entry
{
    const char* pattern;
    size_t offset;
};

static void patchAppTimeoutForMonitoredProcs(const dynlib_info* obj, const pid_t pid)
{
#if defined(__PROSPERO__)
    const uintptr_t mapbase = obj->obj.mapbase;
    const size_t mapsize = obj->obj.mapsize;

    uintptr_t timeout_branch = 0;
    char patch_buf[MAX_PATH + 1] = {};
    const char* patten = "83 f8 0b 0f 82 ? ? ? ? 42 8b 44 23 f0";
    const size_t patten_offset = 3;
    gen_backup_path(FUNC_N(0), patten, patten_offset, patch_buf, _countof_1(patch_buf));
    make_backup_buf(sfo_backup, 2);
    const bool is_ptr = false;
    if (restore_cached_backup(pid, mapbase, patch_buf, &sfo_backup, sizeof(sfo_backup), sizeof(sfo_backup.bytes), &timeout_branch, is_ptr) == 1)
    {
        debugf("timeout_branch cached %lx\n", timeout_branch);
    }
    else
    {
        timeout_branch = pid_chunk_scan(pid, mapbase, mapsize, patten, patten_offset);
    }
    debugf("timeout_branch: %lx\n", timeout_branch);
    if (timeout_branch)
    {
        if (file_exists(patch_buf) == FAILED)
        {
            debugf("cache file not exist %s\n", patch_buf);
            userland_copyout(pid, timeout_branch, sfo_backup.bytes, sizeof(sfo_backup.bytes));
            sfo_backup.addr = timeout_branch - mapbase;
            write_backup_to_disk(patch_buf, &sfo_backup, sizeof(sfo_backup));
        }
        static const uint8_t p[] = {0x48, 0xe9};
        userland_copyin2(pid, timeout_branch, p, sizeof(p));
    }
#endif
}

static void patchOnNewProcess(patch_frame_context* frame, const dynlib_info* obj, const pid_t pid)
{
    if (!is_ps4)
    {
        patchAppTimeoutForMonitoredProcs(obj, pid);
    }
    const uintptr_t codebase = frame->frame_start;
    const uintptr_t mapbase = obj->obj.mapbase;
    const size_t mapsize = obj->obj.mapsize;

    static const struct pattern_entry ps4_patterns[] = {
        {"55 48 89 e5 41 57 41 56 41 55 41 54 53 48 83 ec 18 49 89 ? bf 18 00 00 00"},
    };

    static const struct pattern_entry ps5_patterns[] = {
        {"55 48 89 e5 41 57 41 56 41 55 41 54 53 48 83 ec 18 49 89 ? bf 18 00 00 00 49 89 d6 49 89 f5"},              // 4.00 - 10.00
        {"55 48 89 e5 41 57 41 56 41 55 41 54 53 50 49 89 fd bf 18 00 00 00 49 89 d7 49 89 f4 e8 ? ? ? ?"},           // 11.00+
        {"55 48 89 e5 41 57 41 56 41 55 41 54 53 48 83 ec 18 49 89 fd bf 18 00 00 00 49 89 d6 49 89 f4 e8 ? ? ? ?"},  // 12.00+
    };

    const struct pattern_entry* patterns = is_ps4 ? ps4_patterns : ps5_patterns;
    const size_t pattern_count = is_ps4 ? _countof(ps4_patterns) : _countof(ps5_patterns);

    uintptr_t original_newproc = 0;
    for (size_t idx = 0; idx < pattern_count; idx++)
    {
        const char* patten = patterns[idx].pattern;
        const size_t patoffset = patterns[idx].offset;

        original_newproc = pid_chunk_scan(pid, mapbase, mapsize, patten, patoffset);
        printf("original_newproc %lx\n", original_newproc);
        if (original_newproc)
        {
            break;
        }
    }

    if (original_newproc)
    {
        printf("codebase %lx\n", codebase);
        char patch_buf[MAX_PATH + 1] = {};
        char hexbuf[32 + 1] = {};
        const size_t patoffset = 0;
        snprintf(hexbuf, _countof_1(hexbuf), "%016lx", original_newproc - mapbase);
        gen_backup_path(FUNC_N(0), hexbuf, patoffset, patch_buf, _countof_1(patch_buf));
        uintptr_t pOriginalNewProc = 0;
        make_backup_buf(OnNewProcPtrBackup, sizeof(pOriginalNewProc));
        const bool is_ptr = true;
        if (restore_cached_backup(pid, mapbase, patch_buf, &OnNewProcPtrBackup, sizeof(OnNewProcPtrBackup), sizeof(OnNewProcPtrBackup.bytes), &pOriginalNewProc, is_ptr) == 1)
        {
            debugf("pOriginalNewProc cached %lx\n", pOriginalNewProc);
        }
        else
        {
            pOriginalNewProc = pid_chunk_scan_data(pid, is_ps4 ? obj->obj.unknown1 : mapbase, is_ps4 ? obj->obj.unknown1size : mapsize, &original_newproc, sizeof(original_newproc), patoffset);
        }
        debugf("pOriginalNewProc %lx\n", pOriginalNewProc);
        uint8_t code_copy[sizeof(shellcode)];
        memset(code_copy, 0xcc, sizeof(code_copy));
        // just to invalidate pattern scan
        userland_copyin(pid, code_copy, codebase, sizeof(code_copy));
        memcpy(code_copy, shellcode, sizeof(shellcode));
        memcpy(code_copy + onNewProcess_original_offset, &original_newproc, sizeof(original_newproc));
        static const struct sym_entry symlist[] =
            {
                sym(sysctl, 0x2001),
                sym(malloc, 0x2),
                sym(free, 0x2),
                sym(sceKernelGetAppInfo, 0x2001),
                sym(getpid, 0x2001),
                sym(kill, 0x2001),
                sym(puts, 0x2),
                sym(usleep, 0x2001),
                sym(fopen, 0x2),
                sym(fclose, 0x2),
                sym(fwrite, 0x2),
                sym(stat, 0x2001),
                sym(memset, 0x2),
                sym(vsnprintf, 0x2),
                sym(printf, 0x2),
                sym(strlen, 0x2),
                sym(sceKernelSendNotificationRequest, 0x2001),
                sym(perror, 0x2),
                sym(sceKernelGetProcessName, 0x2001),
                sym(socket, 0x2001),
                sym(htons, 0x2001),
                sym(connect, 0x2001),
                sym(close, 0x2001),
                sym(recv, 0x2001),
                sym(send, 0x2001),
                sym(__inet_pton, 0x2001),
                sym(fgets, 0x2),  // for getAppInfo patch
                sym(__error, 0x2001),
                sym(strerror, 0x2),
                sym(strncpy, 0x2),
            };
        if (write_syms(pid, code_copy, symlist, _countof(symlist)) == 0)
        {
            printf("wrote %ld symbols okay!\n", _countof(symlist));
        }
        else
        {
            return;
        }
        if (pOriginalNewProc)
        {
            debugf("pOriginalNewProc found okay at %lx\n", pOriginalNewProc);
            if (file_exists(patch_buf) == FAILED)
            {
                debugf("cache file not exist %s\n", patch_buf);
                OnNewProcPtrBackup.addr = original_newproc - mapbase;
                *(uintptr_t*)OnNewProcPtrBackup.bytes = pOriginalNewProc - mapbase;
                write_backup_to_disk(patch_buf, &OnNewProcPtrBackup, sizeof(OnNewProcPtrBackup));
            }
            const uintptr_t entry_point = codebase + __export_onNewProcess_hook_offset;
            userland_copyin(pid, &entry_point, pOriginalNewProc, sizeof(entry_point));
        }
        else
        {
            notify(__FUNCTION__ ": failed to find pOriginalNewProc nya");
            return;
        }
        const size_t nbytes = sizeof(code_copy);
        userland_copyin(pid, code_copy, codebase, nbytes);
        frame->frame_start += nbytes;
        frame->frame_consumed += nbytes;
        frame->frame_size -= nbytes;
    }
    else
    {
        notify(__FUNCTION__ ": failed to find original_newproc nya");
        return;
    }
}

static void patchGetAppInfoSfo(patch_frame_context* frame, const dynlib_info* obj, const pid_t pid)
{
    const uintptr_t mapbase = obj->obj.mapbase;
    const size_t mapsize = obj->obj.mapsize;
    uintptr_t call_to_getappinfo = 0;
    bool is_new_fw_ps5 = false;
    char patch_buf[MAX_PATH + 1] = {};
    make_backup_buf(sfo_backup, 5);

    static const struct pattern_entry ps4_patterns[] = {
        {"49 8b 76 10 48 8b 55 a0 4c 89 ff e8 ? ? ? ? 85 c0", 11},
    };

    static const struct pattern_entry ps5_patterns[] = {
        {"e8 ? ? ? ? 48 8b 85 ? ? ? ? 4c 89 ? ? 89 ? 48 8b 70 10 e8 ? ? ? ?", 22},                   // 4.00-8.00
        {"48 8b 85 40 ff ff ff 48 8d 75 a0 4c 89 ef 4c 89 f9 48 8b 50 10 e8 ? ? ? ? 41 89 c4", 21},  // 10.00+
        {"49 8b 55 10 48 8b bd ? ? ? ? 48 8d 75 ? 4c 89 e1 e8 ? ? ? ? 41 89 c6", 18},                // 11.00+
        {"49 8b 55 10 48 8b bd ? ? ? ? 48 8b 8d ? ? ? ? 48 8d 75 ? e8 ? ? ? ? 41 89 c6", 22},        // 12.00+
    };

    const struct pattern_entry* patterns = is_ps4 ? ps4_patterns : ps5_patterns;
    const size_t pattern_count = is_ps4 ? _countof(ps4_patterns) : _countof(ps5_patterns);

    for (size_t idx = 0; idx < pattern_count; idx++)
    {
        const char* patten = patterns[idx].pattern;
        const size_t patten_offset = patterns[idx].offset;

        if (idx > 0)
        {
            // some new firmware, this function chnaged, nyaa
            debugf("couldn't find first call_to_getappinfo, trying (\"%s\" + %ld)!\n", patten, patten_offset);
            memset(patch_buf, 0, sizeof(patch_buf));
            memset(&sfo_backup, 0, sizeof(sfo_backup));
        }

        gen_backup_path(FUNC_N(0), patten, patten_offset, patch_buf, _countof_1(patch_buf));

        const bool is_ptr = false;
        if (restore_cached_backup(pid, mapbase, patch_buf, &sfo_backup, sizeof(sfo_backup), sizeof(sfo_backup.bytes), &call_to_getappinfo, is_ptr) == 1)
        {
            debugf("call_to_getappinfo cached %lx\n", call_to_getappinfo);
        }
        else
        {
            call_to_getappinfo = pid_chunk_scan(pid, mapbase, mapsize, patten, patten_offset);
        }

        if (call_to_getappinfo)
        {
            is_new_fw_ps5 = idx > 0;
            break;
        }
    }

    printf("call_to_getappinfo: %lx is_new_fw_ps5 %d\n", call_to_getappinfo, is_new_fw_ps5);

    if (call_to_getappinfo)
    {
        if (file_exists(patch_buf) == FAILED)
        {
            debugf("cache file not exist %s\n", patch_buf);
            userland_copyout(pid, call_to_getappinfo, sfo_backup.bytes, sizeof(sfo_backup.bytes));
            sfo_backup.addr = call_to_getappinfo - mapbase;
            write_backup_to_disk(patch_buf, &sfo_backup, sizeof(sfo_backup));
        }
        const uintptr_t original_func = pid_read_call(pid, call_to_getappinfo);
        pid_write_call(pid, frame->frame_base + __export_GetAppInfoHook_offset, original_func, true);
        const uintptr_t export_offset = frame->frame_base + (is_ps4 ? __export_PS4_GetAppInfoHook_offset : is_new_fw_ps5 ? __export_PS5_GetAppInfoHook2_offset
                                                                                                                         : __export_PS5_GetAppInfoHook_offset);
        pid_write_call(pid, call_to_getappinfo, export_offset, false);
        // no need to modify frame
    }
    else
    {
        notify("failed to find getAppInfoFromDB, patching of runtime sfo will not be possible.");
    }
}

static void patch_shellcore_ps4_phone_home(const int pid, const uintptr_t start, const uintptr_t start_size)
{
#if defined(__ORBIS__)
    char write_buf[MAX_PATH + 1];
    snprintf_clear(write_buf, _countof_1(write_buf), "/user/temp/patch_%s", "target_patch_game_update");
    if (0 && file_exists(write_buf) == FAILED)
    {
        const uintptr_t target_patch_game_update = pid_chunk_scan(pid, start, start_size, "41 bf 14 26 18 80 41 80 7c 24 10 00 0f", 12);
        debugf("target_patch_game_update %lx\n", target_patch_game_update);
        if (target_patch_game_update)
        {
            static const uint8_t patch[] = {0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
            func_ret(userland_copyin(pid, patch, target_patch_game_update, sizeof(patch)));
            touch_file(write_buf);
            notify("patched game updates");
        }
        else
        {
            touch_file(write_buf);
            notify("can't find target_patch_game_update! maybe already patched?\n");
        }
    }
    else
    {
        debugf("check file %s exists!\n", write_buf);
    }
    snprintf_clear(write_buf, _countof_1(write_buf), "/user/temp/patch_%s", "target_patch_system_update");
    if (file_exists(write_buf) == FAILED)
    {
        const uintptr_t target_patch_system_update = pid_chunk_scan(pid, start, start_size, "80 7f 58 00 0f 84 ? ? ? ? 49 89 f7 48 85 f6", 4);
        debugf("target_patch_system_update %lx\n", target_patch_system_update);
        if (target_patch_system_update)
        {
            static const uint8_t patch[] = {0x48, 0xe9};
            func_ret(userland_copyin(pid, patch, target_patch_system_update, sizeof(patch)));
            touch_file(write_buf);
            notify("patched system update");
        }
        else
        {
            touch_file(write_buf);
            notify("can't find target_patch_system_update! maybe already patched?\n");
        }
    }
    else
    {
        debugf("check file %s exists!\n", write_buf);
    }
    snprintf_clear(write_buf, _countof_1(write_buf), "/user/temp/patch_%s", "phone_home_user_signed_in_target");
    if (file_exists(write_buf) == FAILED)
    {
        const uintptr_t phone_home_user_signed_in_target = pid_chunk_scan(pid, start, start_size, "55 48 89 e5 41 57 41 56 41 55 41 54 53 48 83 e4 ? 48 81 ec ? ? ? ? 48 8b 0d ? ? ? ? 48 85 ff 41 be 01 00 de 80", 0);
        debugf("phone_home_user_signed_in_target %lx\n", phone_home_user_signed_in_target);
        if (phone_home_user_signed_in_target)
        {
            static const uint8_t patch[] = {0x31, 0xc0, 0xc3};
            func_ret(userland_copyin(pid, patch, phone_home_user_signed_in_target, sizeof(patch)));
            touch_file(write_buf);
            notify("patched phoning home of user signed in");
        }
        else
        {
            touch_file(write_buf);
            notify("can't find phone_home_user_signed_in_target! maybe already patched?\n");
        }
    }
    else
    {
        debugf("check file %s exists!\n", write_buf);
    }
#endif
}

void patchShellcore(const pid_t pid)
{
    debugf("shellcore pid %d\n", pid);
    func_ret(kill(pid, SIGSTOP));
    uintptr_t start = 0;
    uint64_t start_size = 0;
    dynlib_obj_t temp_obj;
    const int r = kernel_dynlib_obj(pid, 0, &temp_obj);
    start = temp_obj.mapbase;
    start_size = temp_obj.mapsize;

    {
        int prot = 0;
        func_ret(prot = kernel_get_vmem_protection(pid, start, start_size));
        debugf("before prot %d\n", prot);
        func_ret(kernel_mprotect(pid, start, start_size, PROT_READ | PROT_EXEC | PROT_WRITE));
        func_ret(prot = kernel_get_vmem_protection(pid, start, start_size));
        debugf("after prot %d\n", prot);
    }

    debugf("ret %d start %lx size %ld bytes\n", r, start, start_size);
    const bool shellcore_okay = r == 0 && start && start_size;
    if (is_ps4 && shellcore_okay)
    {
        patch_shellcore_ps4_phone_home(pid, start, start_size);
    }
    if (shellcore_okay)
    {
        dynlib_info obj = {.obj = temp_obj};
        dump_dynlib_obj(&obj.obj);
        const uintptr_t fb = obj.obj.eh_frame;
        const uintptr_t fs = obj.obj.eh_frame_size;
        patch_frame_context f = {.frame_base = fb, .frame_start = fb, .frame_size = fs};
        patchOnNewProcess(&f, &obj, pid);
        patchGetAppInfoSfo(&f, &obj, pid);
    }
    func_ret(kill(pid, SIGCONT));
}
