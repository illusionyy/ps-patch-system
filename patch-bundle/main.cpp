#include <stdio.h>
#include <stdint.h>  // need this order nya
#include <unistd.h>  // need this order nya
#include <sys/sysctl.h>
#include <sys/user.h>
#include <sys/syscall.h>

extern "C"
{
#include "../shared/debug.h"
#include "../shared/macro.h"
#include <elfldr.h>
}

#include "../shared/notify.h"
#include "../shared/patch-common.h"
#include "utils.hpp"

#include <generated/patch_elf.inc.h>
#include <generated/patch_server_elf.inc.h>
#include <generated/patch_web_elf.inc.h>

static const int O_WRONLY_ = 2;
extern "C" int open(const char* path, int mode);
extern "C" const char* basename(const char* path);

static void __attribute__((constructor)) set_thread_name(void)
{
    syscall(SYS_thr_set_name, -1, ELF_NAME);
    puts("set thread name to " ELF_NAME);
}

int main()
{
    signal(SIGPIPE, SIG_IGN);
    notify(
        "%s start\n"
        "Built: " DATE_TIME "\n",
        ELF_NAME);
    create_default_folders();
    const char* argv[] = {"payload.elf", 0};
    const int console_fd = open("/dev/console", O_WRONLY_);  // not enough permission yet, so open it for them
    int pid = 0;
    argv[0] = basename(patch_server_elf_path);
    func_ret(pid = elfldr_spawn(console_fd, (char* const*)argv, (uint8_t*)patch_server_elf_data, sizeof(patch_server_elf_data)));
    print_load_constexpr(notify, argv[0], pid, sizeof(patch_server_elf_data));
    argv[0] = basename(patch_elf_path);
    func_ret(pid = elfldr_spawn(console_fd, (char* const*)argv, (uint8_t*)patch_elf_data, sizeof(patch_elf_data)));
    print_load_constexpr(notify, argv[0], pid, sizeof(patch_elf_data));
    argv[0] = basename(patch_web_elf_path);
    func_ret(pid = elfldr_spawn(console_fd, (char* const*)argv, (uint8_t*)patch_web_elf_data, sizeof(patch_web_elf_data)));
    print_load_constexpr(notify, argv[0], pid, sizeof(patch_web_elf_data));
    return 0;
}
