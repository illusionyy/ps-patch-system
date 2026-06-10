#pragma once

#include <unistd.h>
#include "macro.h"
#include <sys/cdefs.h>

typedef struct metadata_buf
{
    char title_id[16];
    char main_version[16];
    char app_version[16];
    char title[128];
    char content_id[64];
} metadata_buf;

__BEGIN_DECLS
int get_app_metadata_(const pid_t pid, metadata_buf* buf, const char* user);
__END_DECLS
#define get_app_metadata(p, b) get_app_metadata_(p, b, FILE_FUNC_LINE)
