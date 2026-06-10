#pragma once

#include <stdint.h>
#include <sys/cdefs.h>
#include <unistd.h>

typedef struct app_info
{
    uint32_t app_id;
    uint64_t unknown1;
    char title_id[10];
    uint32_t apptype;
    char unknown2[0x36];
} app_info_t;

__BEGIN_DECLS
int sceKernelGetAppInfo(const pid_t pid, app_info_t* info);
__END_DECLS
