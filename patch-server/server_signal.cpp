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
#include "../shared/sigqueue.h"

void run_patch(int signo, pid_t pid);

static bool quit = false;

/**
 * Callback function invoked by the sigqueue when signals are consumed.
 **/
static void on_signal(const siginfo_t* si)
{
    switch (si->si_signo)
    {
        case SIGUSR1:
        case SIGUSR2:
        {
            run_patch(si->si_signo, si->si_pid);
            break;
        }
        case SIGTERM:
        {  // TODO
            notify("SIGTERM was raised, what do i do now? nya\n");
            quit = true;
            // exit(0);
            break;
        }
    }
}

void setup_main_thread(void)
{
    // initialize the global signal queue
    sigqueue_init();
    sigqueue_subscribe(SIGUSR1);
    sigqueue_subscribe(SIGUSR2);
    sigqueue_subscribe(SIGTERM);

    while (!quit)
    {
        int r = 0;
        perror_on_less_zero(r = sigqueue_consume(&on_signal, 5 * 1000), r);
        if (r < 0)
        {
            break;
        }
    }
    sigqueue_fini();
}

