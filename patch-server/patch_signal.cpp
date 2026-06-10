#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include <sys/sysctl.h>
#include <sys/user.h>
#include <sys/syscall.h>
#include <sys/mman.h>

#include "../shared/platform_kernel.h"

#include "../shared/dynlib.h"
#include "../shared/debug.h"
#include "../shared/server_data.h"
#include "../shared/app_metadata.h"
#include "../shared/macro.h"
#include "../shared/memory.h"

#include "../shared/proc_rw.h"
#include "../shared/notify.h"

#include "../shared/patch_frame_context.h"
#include "../shared/shellcode_symbol.h"

extern pid_t g_pid;
static const int O_WRONLY_ = 2;
extern "C" int open(const char* path, int mode);

#include "usermode_prx_load.h"

void patch_prx_load_for_signal(patch_frame_context& ctx, const client_data& read_client)
{
    {
        dump_dynlib_obj(&ctx.frame_info.obj);
        {
            uintptr_t start = ctx.frame_info.obj.mapbase, start_size = ctx.frame_info.obj.mapsize;
            int pid = read_client.clientPid;
            int prot = 0;
            func_ret(prot = kernel_get_vmem_protection(pid, start, start_size));
            debugf("before prot %d\n", prot);
            func_ret(kernel_mprotect(pid, start, start_size, PROT_READ | PROT_EXEC | PROT_WRITE));
            func_ret(prot = kernel_get_vmem_protection(pid, start, start_size));
            debugf("after prot %d\n", prot);
        }
        const uintptr_t post_prx_load_append_module_call = pid_chunk_scan(read_client.clientPid, ctx.frame_info.obj.mapbase, ctx.frame_info.obj.mapsize, "48 c7 c0 52 02 00 00 49 89 ca 0f 05", 0);
        printf("post_prx_load_append_module_call %lx\n", post_prx_load_append_module_call);
        if (post_prx_load_append_module_call)
        {
            const uintptr_t codebase = ctx.frame_start;
            uint8_t code_copy[sizeof(shellcode)];
            memcpy(code_copy, shellcode, sizeof(shellcode));
            const uintptr_t original_newproc = post_prx_load_append_module_call;
            if (original_newproc)
            {
                const size_t pid_big = g_pid;
                memcpy(code_copy + payload_pid_offset, &pid_big, sizeof(pid_big));
                static ssize_t console_fd = 0;
                if (!console_fd)
                {
                    console_fd = open("/dev/console", O_WRONLY_);
                }
                debugf("console_fd: %ld\n", console_fd);
                memcpy(code_copy + payload_console_fd_offset, &console_fd, sizeof(console_fd));
                static const struct sym_entry symlist[] = {
                    sym(getpid, 0x2001),
                    sym(kill, 0x2001),
                    sym(__sys_rdup, 0x2001),
                    sym(dup2, 0x2001),
                    sym(printf, 0x2),
                };
                if (write_syms(read_client.clientPid, code_copy, symlist, _countof(symlist)) == 0)
                {
                    printf("wrote %ld symbols okay!\n", _countof(symlist));
                }
                else
                {
                    return;
                }

                userland_copyin(read_client.clientPid, code_copy, codebase, sizeof(code_copy));
                const bool use_jmp = true;
                pid_write_call(read_client.clientPid, original_newproc, codebase + __export_post_prx_load_append_module_hook_offset, use_jmp);
                pid_write_call(read_client.clientPid, codebase + __export_post_prx_load_append_module_offset + 10, original_newproc + 7, use_jmp);
                const size_t nbytes = sizeof(code_copy);
                ctx.frame_consumed += nbytes;
                ctx.frame_start += nbytes;
                ctx.frame_size -= nbytes;
            }
        }
    }
}

void upload_init_env_ptr(const uint32_t i, const client_data& read_client)
{
    debugf("here\n");
    dynlib_info f = {};
    kernel_dynlib_obj(read_client.clientPid, 0x2001, &f.obj);
    const uintptr_t init_env_ptr = kernel_dynlib_dlsym(read_client.clientPid, i, "_init_env");
    printf("init_env_ptr(%u): %lx\n", i, init_env_ptr);
    if (init_env_ptr)
    {
        const uintptr_t target_ptr = f.obj.eh_frame + __export_init_env_hook_offset;

        printf("init_env_ptr: %lx\n", init_env_ptr);
        printf("target_ptr: %lx\n", target_ptr);
        const bool is_jmp = true;
        pid_write_call2(read_client.clientPid, init_env_ptr, target_ptr, is_jmp);
    }
}
