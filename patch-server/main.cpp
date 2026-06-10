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
#include "../shared/app_info.h"
#include "../shared/macro.h"
#include "../shared/memory.h"
#include "../shared/stringid.hpp"

#include "../shared/notify.h"
#include "../shared/proc_rw.h"
#include "../shared/patch_frame_context.h"
#include "patch_reg.hpp"

#include <libgen.h>
#include "patch_user_xml.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>

#include <pthread.h>
#include <pthread_np.h>

#include "../shared/my_assert.h"
#include "../shared/proc.h"

pid_t g_pid = 0;

void setup_server_thread(void);
void setup_main_thread(void);

static int run_main(void)
{
    setup_server_thread();
    setup_main_thread();
    notify(__FUNCSIG__ ": finalize\n");
    return 0;
}

static void __attribute__((constructor)) set_thread_name(void)
{
    syscall(SYS_thr_set_name, -1, ELF_NAME);
    puts("set thread name to " ELF_NAME);
}

int main(void)
{
    signal(SIGPIPE, SIG_IGN);
    notify(
        "%s start\n"
        "Built: " DATE_TIME "\n",
        ELF_NAME);
    kill_last_pid(ELF_NAME);
    g_pid = getpid();
    printf("this elf pid %d\n", g_pid);
    app_info inf = {};
    if (sceKernelGetAppInfo(g_pid, &inf) == 0)
    {
        printf("this elf appid 0x%x\n", inf.app_id);
    }
    return run_main();
}
