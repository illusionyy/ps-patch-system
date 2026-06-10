#pragma once

#include <sys/cdefs.h>
#include <unistd.h>

__BEGIN_DECLS
int sys_thr_get_name(const int pid, char* buf, const unsigned long nbuf);
/**
 * Find the pid of a process with the given name. If multiple processes share
 * the same name, the one with the lowest pid is returned.
 **/
pid_t findpid(const char* procname);
void kill_last_pid(const char* procname);
__END_DECLS
