#include "app_metadata.h"
#include "app_info.h"
#include "macro.h"
#include "debug.h"
#include "memory.h"
#include <stdint.h>
#include <string.h>

int get_app_metadata_(const pid_t pid, metadata_buf* buf, const char* user)
{
    printf("%s called from %s\n", __PRETTY_FUNCTION__, user);
    app_info_t app;
    memset(&app, 0, sizeof(app));
    extern int sceShellCoreUtilGetAppLaunchedParamString(const uint32_t appid, const char* key, char* out, size_t out_sz);
    int s = 0;
    s |= sceKernelGetAppInfo(pid, &app);
    const uint32_t u = app.app_id;
    // bc and native share some of these fields
    s |= sceShellCoreUtilGetAppLaunchedParamString(u, "TITLE_ID", buf->title_id, sizeof(buf->title_id));
    s |= sceShellCoreUtilGetAppLaunchedParamString(u, "VERSION", buf->main_version, sizeof(buf->main_version));
    s |= sceShellCoreUtilGetAppLaunchedParamString(u, "APP_VER", buf->app_version, sizeof(buf->app_version));
    s |= sceShellCoreUtilGetAppLaunchedParamString(u, "TITLE", buf->title, sizeof(buf->title));
    s |= sceShellCoreUtilGetAppLaunchedParamString(u, "CONTENT_ID", buf->content_id, sizeof(buf->content_id));
    // fallback for native app, this is the only version field available
    if (!*buf->app_version)
    {
        s |= sceShellCoreUtilGetAppLaunchedParamString(u, "CONTENT_VERSION", buf->app_version, sizeof(buf->app_version));
    }
    if (s < 0)
    {
        return s;
    }
#define fun printf
    PRINT_CHAR_FIELD(fun, buf->title_id);
    PRINT_CHAR_FIELD(fun, buf->main_version);
    PRINT_CHAR_FIELD(fun, buf->app_version);
    PRINT_CHAR_FIELD(fun, buf->title);
    PRINT_CHAR_FIELD(fun, buf->content_id);
#undef fun
    return s;
}
