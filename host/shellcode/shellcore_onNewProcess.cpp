#include <errno.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/sysctl.h>
#include <sys/user.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <sys/signal.h>

#include "../../shared/stringid.hpp"

#include "../../shared/server_data.h"
#include "../../shared/app_info.h"
#include "../../shared/app_metadata.h"
#include "../../shared/notify.h"

#include "../../shared/debug.h"

#include <arpa/inet.h>
#include <netinet/in.h>

extern "C" int sceKernelGetProcessName(int pid, char* buf, const size_t nbuf);

static void send_signal_to_payload(const pid_t payload_pid, client_data& data)
{
    size_t retry = 0;
    const size_t max_retry = 25;

    for (; retry < max_retry; retry += 1)
    {
        int fd = 0;
        perror_on_less_zero(fd = socket(AF_INET, SOCK_STREAM, 0), fd);
        if (fd < 0)
        {
            ui_perror("socket", "failed to open socket, maybe not valid permission?");
            return;
        }
        struct sockaddr_in addr = {};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(SERVER_PORT);
        inet_pton(AF_INET, SERVER_HOST, &addr.sin_addr);
        int r = 0;
        perror_on_less_zero(r = connect(fd, (struct sockaddr*)&addr, sizeof(addr)), r);
        if (r < 0)
        {
            printf("[shellcore] server not up, retrying...\n");
            close(fd);
            usleep(1000 * 1000);
            continue;
        }

        send(fd, &data, sizeof(data), 0);
        data.res = client_data::res_none;
        recv(fd, &data, sizeof(data), 0);
        const uint32_t res = data.res;
        printf("[shellcore] received: %d\n", res);
        if (res == client_data::res_okay)
        {
            printf("[shellcore] server said okay!\n");
        }
        close(fd);
        break;
    }
    if (retry == max_retry)
    {
        unotify(FUNC_LINE ": timed out waiting for payload server.\nclient pid %d", data.clientPid);
    }
}

struct config
{
    pid_t clientPid;
};

#define MiB(x) (x ? ((x) / (1024 * 1024)) : 0)

static const char* state_abbrev(const size_t n)
{
    switch (n)
    {
        case 1:
        {
            return "START";
        }
        case 2:
        {
            return "RUN";
        }
        case 3:
        {
            return "SLEEP";
        }
        case 4:
        {
            return "STOP";
        }
        case 5:
        {
            return "ZA ZOMBIE";
        }
        case 6:
        {
            return "WAIT";
        }
        case 7:
        {
            return "LOCK";
        }
        default:
        {
            break;
        }
    }
    return "";
}

static int patch_apps(config* param_3)
{
    int mib[4] = {};
    mib[0] = CTL_KERN;
    mib[1] = KERN_PROC;
    mib[2] = KERN_PROC_PROC;
    // mib[3] = 0;
    size_t buf_size = 0;
    uint8_t* buf = 0;

    // determine size of query response
    if (sysctl(mib, _countof(mib), NULL, &buf_size, NULL, 0))
    {
        perror("sysctl");
        return -(__LINE__);
    }

    // allocate memory for query response
    if (!(buf = (uint8_t*)malloc(buf_size)))
    {
        perror("malloc");
        return -(__LINE__);
    }

    // query the kernel for proc info
    if (sysctl(mib, _countof(mib), buf, &buf_size, NULL, 0))
    {
        perror("sysctl");
        free(buf);
        return -(__LINE__);
    }
    printf(
        "     PID"
        "      PPID"
        "     PGID"
        "      SID"
        "      UID"
        "      State"
        "      AppId"
        "     TitleId"
        "         Memory (MiB)"
        "                                Command\n");
    for (uint8_t* ptr = buf; ptr < (buf + buf_size);)
    {
        struct kinfo_proc* ki = (struct kinfo_proc*)ptr;
        ptr += ki->ki_structsize;
        app_info_t appinfo2 = {};
        char cmd[32] = {};
        sceKernelGetProcessName(ki->ki_pid, cmd, sizeof(cmd));

        const int r = sceKernelGetAppInfo(ki->ki_pid, &appinfo2);
        if (r)
        {
            printf("sceKernelGetAppInfo 0x%x\n", r);
        }

        const char* state_buf = state_abbrev(ki->ki_stat);
        printf("%8u  %8u %8u %8u %8u %10s   %08x  %10s  %06ldmb / %06ldmb        %31s\n",
               ki->ki_pid,
               ki->ki_ppid,
               ki->ki_pgid,
               ki->ki_sid,
               ki->ki_uid,
               state_buf ? state_buf : "",
               appinfo2.app_id,
               appinfo2.title_id,
               MiB(ki->ki_rssize * PAGE_SIZE),
               MiB(ki->ki_size),
               cmd);
    }

    const pid_t clientPid = param_3->clientPid;
    for (uint8_t* ptr = buf; ptr < (buf + buf_size);)
    {
        app_info_t appinfo = {};
        struct kinfo_proc* ki = (struct kinfo_proc*)ptr;
        ptr += ki->ki_structsize;
        sceKernelGetAppInfo(clientPid, &appinfo);

        bool okay = false;
        switch (sid(ki->ki_comm))
        {
            case sid("payload.elf"):  // PS5
            case sid("Payload"):      // PS4, nyaaa, why the fuck is it different?
                                      // i'm not insane right? nya
                                      // https://github.com/ps5-payload-dev/elfldr/blob/699e8bcff03e91e8d6ca6eba281af25c5a58d8c2/socksrv.c#L219
                                      // sets by argv[0], https://github.com/ps5-payload-dev/elfldr/blob/699e8bcff03e91e8d6ca6eba281af25c5a58d8c2/elfldr.c#L704
                                      // https://github.com/ps4-payload-dev/elfldr/blob/b3ee49746cb9f3464e8dc75ec6387e8408558b8a/elfldr.c#L587
            {
                printf("ignoring payload process %d\n", clientPid);
                okay = true;
                break;
            }
            case sid("patch-server.elf"):
            {
                client_data data = {};
                data.clientPid = clientPid;
                data.appid = appinfo.app_id;
                printf("shellcore sending to server: pid %d appid 0x%x\n", data.clientPid, data.appid);
                send_signal_to_payload(ki->ki_pid, data);
                okay = true;
                break;
            }
        }
        if (okay)
        {
            break;
        }
    }

    free(buf);
    return 0;
}

extern "C"
{
int onNewProcess_original(void* param_1, void* param_2, config* param_3);

int __export_onNewProcess_hook(void* param_1, void* param_2, config* param_3)
{
    printf("onNewProcess_hook called! param_3->clientPid: %d\n", param_3 ? param_3->clientPid : -1);
    const int r = onNewProcess_original(param_1, param_2, param_3);
    if (param_3 && patch_apps(param_3) == 0)
    {
        puts("patch_apps(param_3) okay!");
    }
    return r;
}
}
