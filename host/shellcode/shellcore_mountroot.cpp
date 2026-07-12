#include "../../shared/macro.h"
#include "../../shared/debug.h"
#include "../../shared/strstrid.hpp"
#include <stdint.h>

// exports
extern "C" int __export_mount_root_original(void* pThis, const char* fs, const char* src, const char* dst, void* param_5, uint32_t* param_6);

static bool valid_str(const char* p)
{
    return p && *p;
}

static const char* check_str(const char* p)
{
    return valid_str(p) ? p : "";
}

static bool is_valid_src_dst(const char* src, const char* dst)
{
    return valid_str(src) && valid_str(dst);
}

static bool is_avContents(const char* src, const char* dst)
{
    return is_valid_src_dst(src, dst) &&
           ConstStrStrId(src, "/user/av_contents/content_tmp") && ConstStrStrId(dst, "/av_contents/content_tmp");
}

static bool is_userData(const char* src, const char* dst)
{
    return is_valid_src_dst(src, dst) &&
           ConstStrStrId(src, "/user/data") && ConstStrStrId(dst, "/data");
}

static int mountUserData(void* pThis, void* param_5, uint32_t* param_6)
{
    static const char src_p[] = "/user/data";
    static const char dst_p[] = "/data";
    debugf("allocating \"%s\" -> \"%s\" path\n", src_p, dst_p);
    return __export_mount_root_original(pThis, "nullfs", src_p, dst_p, param_5, param_6);
}

extern "C" int __export_mount_root_hook(void* pThis, const char* fs, const char* src, const char* dst, void* param_5, uint32_t* param_6)
{
    debugf("fs: \"%s\" src \"%s\" -> dst \"%s\"\n", check_str(fs), check_str(src), check_str(dst));
    // check for pre patched environment
    if (is_userData(src, dst))
    {
        printf("attempting to mount \"%s\" to \"%s\"! this is handled by \"mountUserData\" already!\n", src, dst);
        return 0;
    }
    // not patched, mount data folder
    if (is_avContents(src, dst))
    {
        mountUserData(pThis, param_5, param_6);
    }
    return __export_mount_root_original(pThis, fs, src, dst, param_5, param_6);
}
