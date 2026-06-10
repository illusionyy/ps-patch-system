#include "../../shared/path.h"
#include "../../shared/macro.h"
#include "../../shared/match.h"
#include "../../shared/notify.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "../../shared/stringid.hpp"

union ATTRIBUTE
{
    uint32_t v;
    struct
    {
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t kPS4Pro : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
    };
};

union ATTRIBUTE2
{
    uint32_t v;
    struct
    {
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t kLimitedGPUClock : 1;
        uint32_t kPS5 : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
    };
};

union ATTRIBUTE3
{
    uint32_t v;
    struct
    {
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t kHFR : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t kVRR : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
        uint32_t : 1;
    };
};

static_assert_expression(sizeof(ATTRIBUTE) == sizeof(uint32_t));
static_assert_expression(sizeof(ATTRIBUTE2) == sizeof(uint32_t));
static_assert_expression(sizeof(ATTRIBUTE3) == sizeof(uint32_t));

union shellcore_app_info
{
    struct
    {
        char pad[0x19c];  // since 5.05, this remains same (currently 12.52)
        ATTRIBUTE ATTRIBUTE;
        ATTRIBUTE2 ATTRIBUTE2;
    } ps4;

    struct
    {
        char pad[0x26c];  // only checked 4.50 and 8.00, seems same
        ATTRIBUTE ATTRIBUTE;
        ATTRIBUTE2 ATTRIBUTE2;
        ATTRIBUTE3 ATTRIBUTE3;
    } ps5;
};

extern "C" int __export_GetAppInfoHook(const char* title_id, const void* param_2, const void* param_3, const void* param_4 = 0);

enum
{
    is_ps4 = 1,
    is_ps5,
};

static bool disable_debug_print = true;

#define dprintf(...) (disable_debug_print ? (void)0 : (void)printf(__VA_ARGS__))

static void checkPS4Pro(const char* title_id, shellcore_app_info* pInfo, const int v)
{
    const int disable_ps4_pro = match_line_in_file(BASE_PATH "/disable_ps4_pro.txt", title_id);
    ATTRIBUTE& attr = (v == is_ps4) ? pInfo->ps4.ATTRIBUTE : pInfo->ps5.ATTRIBUTE;
    dprintf("attr.all: 0x%08x\n", attr.v);
    dprintf("attr.kPS4Pro: %d\n", attr.kPS4Pro);
    if (attr.kPS4Pro && disable_ps4_pro == 1)
    {
        unotify("%s wanted to disable PS4 Pro support\n", title_id);
        dprintf("before attr.all 0x%08x\n", attr.v);
        attr.kPS4Pro = false;
        dprintf("after attr.all 0x%08x\n", attr.v);
    }
}

static void enableHFR(const char* title_id, ATTRIBUTE3& a3)
{
    dprintf("before (%s) pInfo->ps5.ATTRIBUTE3.all 0x%08x\n", title_id, a3.v);
    dprintf("before (%s) pInfo->ps5.ATTRIBUTE3.kHFR %d\n", title_id, a3.kHFR);
    dprintf("before (%s) pInfo->ps5.ATTRIBUTE3.kVRR %d\n", title_id, a3.kVRR);
    a3.kHFR = a3.kVRR = true;
    dprintf("after (%s) pInfo->ps5.ATTRIBUTE3.kHFR %d\n", title_id, a3.kHFR);
    dprintf("after (%s) pInfo->ps5.ATTRIBUTE3.kVRR %d\n", title_id, a3.kVRR);
    dprintf("after (%s) pInfo->ps5.ATTRIBUTE3.all 0x%08x\n", title_id, a3.v);
}

extern "C"
{
int __export_PS4_GetAppInfoHook(const char* title_id, const void* param_2, shellcore_app_info* pInfo)
{
    printf(FILE_FUNC_LINE ": (%p,%p,%p)\n", title_id, param_2, pInfo);
    const int r = __export_GetAppInfoHook(title_id, param_2, (void*)pInfo);
    if (!pInfo || !title_id)
    {
        return r;
    }
    checkPS4Pro(title_id, pInfo, is_ps4);
    return r;
}

int __export_PS5_GetAppInfoHook(const char* title_id, const void* param_2, shellcore_app_info* pInfo)
{
    printf(FILE_FUNC_LINE ": (%p,%p,%p)\n", title_id, param_2, pInfo);
    const int r = __export_GetAppInfoHook(title_id, param_2, (void*)pInfo);
    if (!pInfo || !title_id)
    {
        return r;
    }
    checkPS4Pro(title_id, pInfo, is_ps5);
    enableHFR(title_id, pInfo->ps5.ATTRIBUTE3);
    return r;
}
int __export_PS5_GetAppInfoHook2(const char* title_id, const void* param_2, const void* param_3, shellcore_app_info* pInfo)
{
    printf(FILE_FUNC_LINE ": (%p,%p,%p,%p)\n", title_id, param_2, param_3, pInfo);
    const int r = __export_GetAppInfoHook(title_id, param_2, param_3, (void*)pInfo);
    if (!pInfo || !title_id)
    {
        return r;
    }
    checkPS4Pro(title_id, pInfo, is_ps5);
    enableHFR(title_id, pInfo->ps5.ATTRIBUTE3);
    return r;
}
}
