#pragma once

#if defined(__PROSPERO__)
#include <ps5/kernel.h>
#elif defined(__ORBIS__)
#include <ps4/kernel.h>
#include <sys/cdefs.h>
// not public yet
__BEGIN_DECLS
int kernel_get_vmem_protection(int pid, unsigned long addr, unsigned long len);
int kernel_set_vmem_protection(int pid, unsigned long addr, unsigned long len, int prot);
int kernel_mprotect(int pid, unsigned long addr, unsigned long len, int prot);
__END_DECLS
#endif
