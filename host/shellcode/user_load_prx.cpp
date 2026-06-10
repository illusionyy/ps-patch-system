#include <unistd.h>
#include <signal.h>
#include <sys/signal.h>

extern "C"
{
int printf(const char* fmt, ...);
int __sys_rdup(int other_pid, int fd);
int dup2(int fd, int fd2);
pid_t payload_pid();
int payload_console_fd();
int __export_post_prx_load_append_module(const void* param_1, const void* param_2, const void* param_3, const void* param_4, const void* param_5);
int __export_post_prx_load_append_module_hook(const void* param_1, const void* param_2, const void* param_3, const void* param_4, const void* param_5)
{
    const int r = __export_post_prx_load_append_module(param_1, param_2, param_3, param_4, param_5);
    kill(payload_pid(), SIGUSR2);
    kill(getpid(), SIGSTOP);
    return r;
}

static void open_console_from_existing_fd()
{
    return;  // not enough perms, maybe jailbreak first?
    const int target_fd = __sys_rdup(payload_pid(), payload_console_fd());
    dup2(target_fd, STDOUT_FILENO);
    dup2(target_fd, STDERR_FILENO);
    printf("hello from %s!\n", __FUNCSIG__);
}

void __export_init_env_hook()
{
    open_console_from_existing_fd();
}
}
