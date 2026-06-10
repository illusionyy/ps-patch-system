#include <sys/types.h>
#include <sys/user.h>
#include <sys/sysctl.h>  // kinfo_proc and stuff
#include <unistd.h>
#include <sys/syscall.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

#include "../shared/debug.h"

#include "../shared/platform_kernel.h"
#include "../shared/dynlib.h"

#include "../shared/debug.h"
#include "../shared/memory.h"
#include "../shared/macro.h"
#include "../shared/notify.h"

#include "../shared/stringid.hpp"

extern "C"
{
bool is_ps4 = false;
void patchShellcore(const pid_t pid);
int hide_apps(const char* db_path);
}

static int patch_apps(void)
{
    int mib[4] = {CTL_KERN, KERN_PROC, KERN_PROC_PROC, 0};
    size_t buf_size = 0;

    if (sysctl(mib, _countof(mib), 0, &buf_size, 0, 0))
    {
        perror("sysctl size");
        return -(__LINE__);
    }

    uint8_t* buf = (uint8_t*)malloc(buf_size);
    if (!buf)
    {
        perror("malloc");
        return -(__LINE__);
    }

    if (sysctl(mib, _countof(mib), buf, &buf_size, 0, 0))
    {
        perror("sysctl query");
        free(buf);
        return -(__LINE__);
    }

    bool found = false;
    for (uint8_t* ptr = buf; ptr < (buf + buf_size);)
    {
        struct kinfo_proc* ki = (struct kinfo_proc*)ptr;
        switch (sid(ki->ki_comm))
        {
            case sid("SceShellCore"):
            {
                patchShellcore(ki->ki_pid);
                found = true;
                break;
            }
            default:
            {
                break;
            }
        }
        if (found)
        {
            break;
        }
        ptr = ptr + ki->ki_structsize;
    }

    free(buf);
    return 0;
}

static void __attribute__((constructor)) set_thread_name(void)
{
    syscall(SYS_thr_set_name, -1, ELF_NAME);
    puts("set thread name to " ELF_NAME);
}

int main(void)
{
#if defined(__ORBIS__)
    is_ps4 = true;
    puts("defined(__ORBIS__)");
#elif defined(__PROSPERO__)
    is_ps4 = false;
    puts("defined(__PROSPERO__)");
#endif
    notify(
        "%s start\n"
        "Built: " DATE_TIME "\n",
        ELF_NAME);
    func_ret(patch_apps());
    if (is_ps4)
    {
        func_ret(hide_apps("/system_data/priv/mms/app.db"));
    }
    return 0;
}
