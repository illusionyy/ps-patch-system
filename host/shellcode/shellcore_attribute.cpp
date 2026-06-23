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

struct attr
{
    ATTRIBUTE ATTRIBUTE;
    ATTRIBUTE2 ATTRIBUTE2;
    ATTRIBUTE3 ATTRIBUTE3;  // not valid on ps4
    uint32_t ATTRIBUTE4;    // only valid on ps5 11.00
};

extern "C" int __export_GetAppInfoHook(const char* title_id, const void* param_2, const void* param_3, const void* param_4 = 0);

enum
{
    is_ps4 = 1,
    is_ps5,
};

static bool disable_debug_print = true;

#define dprintf(...) (disable_debug_print ? (void)0 : (void)printf(__VA_ARGS__))

extern "C" int dynlib_get_obj_member(const uint32_t module_id, const size_t which, void** out);

static int get_firmware_version_from_disk(const int platform, uint32_t& out)
{
    static uint32_t cached = 0;
    if (!cached)
    {
        void* sce_proc_param = 0;
        int r = 0;
        const uint32_t libkernel_sys = 0x2001;
        perror_on_non_zero(r = dynlib_get_obj_member(libkernel_sys, 8, &sce_proc_param), r);
        if (r || !sce_proc_param)
        {
            return -__LINE__;
        }
        const uint32_t* spp = (uint32_t*)sce_proc_param;
        // on ps5, 4 is ps4 ver, 5 is ps5 ver
        const size_t fw_idx = platform == is_ps4 ? 4 : 5;
        out = cached = spp[fw_idx];
        return 0;
    }
    else
    {
        out = cached;
        return 0;
    }
    return -__LINE__;
}

static uint32_t get_fw(const int p)
{
    uint32_t c = 0;
    get_firmware_version_from_disk(p, c);
    return c >> 16;
}

static uintptr_t get_ps5_attr()
{
    switch (get_fw(is_ps5))
    {
        case 0x100 ... 0x1060:
        {
            return 0x26c;
        }
        // 11.00 or newer
        default:
        {
            return 0x1f4;
        }
    }
}

static uintptr_t get_ps4_attr()
{
    return 0x19c;
}

static attr& get_attr_field(const uintptr_t pInfoAddr, const int platform)
{
    return platform == is_ps4 ? *(attr*)(pInfoAddr + get_ps4_attr()) : *(attr*)(pInfoAddr + get_ps5_attr());
}

static void checkPS4Pro(const char* title_id, ATTRIBUTE& attr, const int v)
{
    const int disable_ps4_pro = match_line_in_file(BASE_PATH "/disable_ps4_pro.txt", title_id);
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
int __export_PS4_GetAppInfoHook(const char* title_id, const void* param_2, uintptr_t pInfo)
{
    printf(FILE_FUNC_LINE ": (%p,%p,%lx)\n", title_id, param_2, pInfo);
    const int r = __export_GetAppInfoHook(title_id, param_2, (void*)pInfo);
    if (!pInfo || !title_id)
    {
        return r;
    }
    attr& attr_f = get_attr_field(pInfo, is_ps4);
    checkPS4Pro(title_id, attr_f.ATTRIBUTE, is_ps4);
    return r;
}

int __export_PS5_GetAppInfoHook(const char* title_id, const void* param_2, uintptr_t pInfo)
{
    unotify(FILE_FUNC_LINE ": (%p,%p,%lx)\n", title_id, param_2, pInfo);
    const int r = __export_GetAppInfoHook(title_id, param_2, (void*)pInfo);
    if (!pInfo || !title_id)
    {
        return r;
    }
    attr& attr_f = get_attr_field(pInfo, is_ps5);
    checkPS4Pro(title_id, attr_f.ATTRIBUTE, is_ps5);
    enableHFR(title_id, attr_f.ATTRIBUTE3);
    return r;
}
int __export_PS5_GetAppInfoHook2(const char* title_id, const void* param_2, const void* param_3, uintptr_t pInfo)
{
    unotify(FILE_FUNC_LINE ": (%p,%p,%p,%lx)\n", title_id, param_2, param_3, pInfo);
    const int r = __export_GetAppInfoHook(title_id, param_2, param_3, (void*)pInfo);
    if (!pInfo || !title_id)
    {
        return r;
    }
    attr& attr_f = get_attr_field(pInfo, is_ps5);
    checkPS4Pro(title_id, attr_f.ATTRIBUTE, is_ps5);
    enableHFR(title_id, attr_f.ATTRIBUTE3);
    return r;
}
}
