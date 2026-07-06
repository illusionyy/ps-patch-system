#pragma once

#include <sys/cdefs.h>
#include <unistd.h>

#define PROC_BUFF_N 32

__BEGIN_DECLS
/**
 * Retrieves thread name of given pid, max size for buf is 32 bytes,
 * any larger buf will be truncated to 31 characters + null term.
 **/
int sys_thr_get_name(const int pid, char buf[PROC_BUFF_N]);
/**
 * Find the pid of a process with the given name. If multiple processes share
 * the same name, the one with the lowest pid is returned.
 **/
pid_t findpid(const char* procname);
void kill_last_pid(const char* procname);
__END_DECLS
