#include "../../shared/macro.h"
#include "../../shared/strstrid.hpp"
#include <stdint.h>

static bool valid_str(const char* p)
{
    return p && *p;
}

static const char* check_str(const char* p)
{
    return valid_str(p) ? p : "";
}

static bool is_avContents(const char* src, const char* dst)
{
    return valid_str(src) && valid_str(dst) &&
           ConstStrStrId(src, "/user/av_contents/content_tmp") && ConstStrStrId(dst, "/av_contents/content_tmp");
}

extern "C" int __export_mount_root_original(void* pThis, const char* fs, const char* src, const char* dst, void* param_5, uint32_t* param_6);
extern "C" int __export_mount_root_hook(void* pThis, const char* fs, const char* src, const char* dst, void* param_5, uint32_t* param_6)
{
    printf(FILE_FUNC_LINE ": fs: \"%s\" src \"%s\" -> dst \"%s\"\n", check_str(fs), check_str(src), check_str(dst));
    if (is_avContents(src, dst))
    {
        static const char src_p[] = "/user/data";
        static const char dst_p[] = "/data";
        printf("allocating \"%s\" -> \"%s\" path\n", src_p, dst_p);
        __export_mount_root_original(pThis, "nullfs", src_p, dst_p, param_5, param_6);
    }
    return __export_mount_root_original(pThis, fs, src, dst, param_5, param_6);
}
