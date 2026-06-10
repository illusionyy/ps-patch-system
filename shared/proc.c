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

#include "macro.h"
#include "stringid.h"

#include "notify.h"

int sys_thr_get_name(const int pid, char* buf, const unsigned long nbuf)
{
    const int SYS_thr_get_name = 0x268;
    return syscall(SYS_thr_get_name, pid, buf, nbuf);
}

pid_t findpid(const char* procname)
{
    int mib[4] = {CTL_KERN, KERN_PROC, KERN_PROC_PROC, 0};
    pid_t mypid = getpid();
    size_t buf_size;
    pid_t pid = -1;
    uint8_t* buf;

    if (sysctl(mib, _countof(mib), 0, &buf_size, 0, 0))
    {
        return -__LINE__;
    }

    if (!(buf = (uint8_t*)malloc(buf_size)))
    {
        return -__LINE__;
    }

    if (sysctl(mib, _countof(mib), buf, &buf_size, 0, 0))
    {
        free(buf);
        return -__LINE__;
    }

    const auto proc_sid = sid_len(procname, -1);
    for (uint8_t* ptr = buf; ptr < (buf + buf_size);)
    {
        struct kinfo_proc* ki = (struct kinfo_proc*)ptr;
        const pid_t ki_pid = ki->ki_pid;
        if (proc_sid == sid_len(ki->ki_comm, -1) && ki_pid != mypid && (pid == -1 || ki_pid < pid))
        {
            pid = ki_pid;
        }
        ptr = ptr + ki->ki_structsize;
    }

    free(buf);

    if (pid == -1)
    {
        errno = ESRCH;
    }

    return pid;
}

void kill_last_pid(const char* procname)
{
    pid_t pid = 0;
    while ((pid = findpid(procname)) > 0)
    {
        const int kr = kill(pid, SIGKILL);
        if (kr)
        {
            ui_perror("kill", "not enough permissions?");
            return;
        }
        printf("killed %d for %s\n", pid, procname);
        size_t retry = 0;
        const size_t max_retry = 10;
        pid_t curr_pid;
        while ((curr_pid = findpid(procname)) == pid)
        {
            if (retry >= max_retry)
            {
                printf("gave up waiting for pid %d to exit\n", pid);
                return;
            }
            retry++;
            printf("%lu/%lu %s still active at %d\n", retry, max_retry, procname, curr_pid);
            sleep(1);
        }
        printf("pid now %d\n", curr_pid);
    }
}
