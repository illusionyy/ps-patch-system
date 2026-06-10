#pragma once

#if defined(__PROSPERO__)

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
#include "../shared/shellcode_symbol.h"
#include "patch_reg.hpp"

#include <libgen.h>
#include "patch_user_xml.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>

#include <pthread.h>
#include <pthread_np.h>

#include "../shared/my_assert.h"

namespace patch_120hz
{
#include "videoout_120hz.h"
struct patch_120hz_context
{
    struct UCC_Context
    {
        bool isUCC_TitleID : 1;
        bool hasVideoOut : 1;
        bool hasVdec2 : 1;
    } ucc;
    int m_pid;
    dynlib_info* m_videoout;
    dynlib_info* m_vdec2;
    uint32_t m_vo_handle;
    uint32_t m_vdec2_handle;
    uintptr_t codebase;

    bool isUCC(const uint32_t name);
    void apply_videoout_hooks();
    void apply_vdec2_hooks();
    void upload_symbols();
    void run_120hz_videoout_patch();
};
}  // namespace patch_120hz

#endif
