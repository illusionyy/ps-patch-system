#pragma once

#include <stdint.h>
#include <unistd.h>
#include <sys/cdefs.h>

__BEGIN_DECLS
int userland_setchar_(int pid, unsigned long addr, char val, const char* user);
int userland_setshort_(int pid, unsigned long addr, short val, const char* user);
int userland_setint_(int pid, unsigned long addr, int val, const char* user);
int userland_setlong_(int pid, unsigned long addr, long val, const char* user);
long userland_getlong_(int pid, unsigned long addr, const char* user);
int userland_getint_(int pid, unsigned long addr, const char* user);
short userland_getshort_(int pid, unsigned long addr, const char* user);
char userland_getchar_(int pid, unsigned long addr, const char* user);

int userland_copyout_(pid_t pid, intptr_t addr, void* buf, size_t len, const char* user);
int userland_copyin_(pid_t pid, const void* buf, intptr_t addr, size_t len, const char* user);
__END_DECLS

#define userland_setchar(p, a, v) userland_setchar_(p, a, v, FILE_FUNC_LINE)
#define userland_setshort(p, a, v) userland_setshort_(p, a, v, FILE_FUNC_LINE)
#define userland_setint(p, a, v) userland_setint_(p, a, v, FILE_FUNC_LINE)
#define userland_setlong(p, a, v) userland_setlong_(p, a, v, FILE_FUNC_LINE)
#define userland_getlong(p, a) userland_getlong_(p, a, FILE_FUNC_LINE)
#define userland_getint(p, a) userland_getint_(p, a, FILE_FUNC_LINE)
#define userland_getshort(p, a) userland_getshort_(p, a, FILE_FUNC_LINE)
#define userland_getchar(p, a) userland_getchar_(p, a, FILE_FUNC_LINE)

#define userland_copyout(p, a, b, l) userland_copyout_(p, a, b, l, FILE_FUNC_LINE)
#define userland_copyin2(p, a, b, l) userland_copyin(p, b, a, l)
#define userland_copyin(p, b, a, l) userland_copyin_(p, b, a, l, FILE_FUNC_LINE)
