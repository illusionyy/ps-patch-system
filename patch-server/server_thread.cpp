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
#include "../shared/sys_state.h"

void run_elf_user_patch(client_data& read_client);

static void run_server(void)
{
    while (1)
    {
        int lfd = 0;
        perror_on_less_zero(lfd = socket(AF_INET, SOCK_STREAM, 0), lfd);
        if (lfd < 0)
        {
            ui_perror("socket", "failed to open socket, maybe not valid permission?");
            assert_always((lfd < 0));
            return;
        }
        int opt = 1;
        int r = 0;
        perror_on_less_zero(r = setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)), r);
        struct sockaddr_in addr = {};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(SERVER_PORT);
        addr.sin_addr.s_addr = INADDR_ANY;
        perror_on_less_zero(r = bind(lfd, (struct sockaddr*)&addr, sizeof(addr)), r);
        if (r < 0)
        {
            ui_perror("bind", ELF_NAME ": Try starting server again.");
            assert_always((r < 0));
            return;
        }
        perror_on_less_zero(r = listen(lfd, 1), r);
        if (r < 0)
        {
            ui_perror("listen", ELF_NAME ": Try starting server again.");
            assert_always((r < 0));
            return;
        }
        notify(ELF_NAME ": waiting for connection from shellcore...\n");

        while (1)
        {
            int cfd = 0;
            perror_on_less_zero(cfd = accept(lfd, NULL, NULL), cfd);
            if (cfd < 0)
            {
                break;
            }
            client_data client = {};
            recv(cfd, &client, sizeof(client), 0);
            printf("[" ELF_NAME "] received pid: %d\n", client.clientPid);
            if (isPatchAllowed())
            {
                run_elf_user_patch(client);
            }
            client.res = client_data::res_okay;
            send(cfd, &client, sizeof(client), 0);
            close(cfd);
        }
        close(lfd);
        sleep(1);
    }
}

static void* server_thread(void* arg)
{
    pthread_set_name_np(pthread_self(), __FUNCSIG__);
    (void)arg;
    run_server();
    pthread_exit(0);
    return 0;
}

void setup_server_thread(void)
{
    pthread_t tid = 0;
    pthread_create(&tid, 0, server_thread, 0);
    printf("tid 0x%p\n", tid);
}
