// based on https://github.com/ps5-payload-dev/websrv/commit/f2e030daadcccc42caa326505a0cdc92de10ef13
// adjust to hardcode titleid, and path by macro

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <ps5/kernel.h>

#include <sys/stat.h>
#include <stdint.h>
#include <stddef.h>

#include "../shared/server_data.h"
#include "../shared/macro.h"
#include "../shared/file.h"

#include <generated/icon0_png.inc.h>

// must be in this order
#pragma comment(lib, "SceIpmi")
#pragma comment(lib, "SceAppInstUtil")

extern int sceAppInstUtilInitialize(void);
extern int sceAppInstUtilAppInstallAll(void*);

static int install_app(const char* title_id, const char* dir)
{
    int (*sceAppInstUtilAppInstallTitleDir)(const char*, const char*, void*) = 0;
    uint32_t handle = 0;

    if (!kernel_dynlib_handle(-1, "libSceAppInstUtil.sprx", &handle))
    {
        static const char nid[] = "Wudg3Xe3heE";
        sceAppInstUtilAppInstallTitleDir = (void*)kernel_dynlib_resolve(-1, handle, nid);
    }

    if (sceAppInstUtilAppInstallTitleDir)
    {
        return sceAppInstUtilAppInstallTitleDir(title_id, dir, 0);
    }

    return sceAppInstUtilAppInstallAll(0);
}

static int install_file(const char* path, const void* data, const size_t size)
{
    return write_file(path, data, size);
}

static int install_shortcut(const char* title_id, const char* url, const char* friendly_name)
{
    struct d
    {
        char path[MAX_PATH + 1];
        char json[1024 + 1];
        struct stat st;
    } d = {};

    if (sceAppInstUtilInitialize())
    {
        return 0;
    }

    snprintf_clear(d.path, _countof_1(d.path), "/user/appmeta/%s/param.json", title_id);
    if (!stat(d.path, &d.st))
    {
        return 0;
    }

    snprintf_clear(d.path, _countof_1(d.path), "/user/app/%s/param.json", title_id);

    if (!stat(d.path, &d.st))
    {
        return 0;
    }

    snprintf_clear(d.path, _countof_1(d.path), "/user/app/%s", title_id);

    if (mkdir(d.path, 0755))
    {
        perror("mkdir");
        return -1;
    }

    snprintf_clear(d.path, _countof_1(d.path), "/user/app/%s/sce_sys", title_id);

    if (mkdir(d.path, 0755))
    {
        perror("mkdir");
        return -1;
    }

    snprintf_clear(d.json, _countof_1(d.json),
                   "{\n"
                   "    \"titleId\": \"%s\",\n"
                   "    \"deeplinkUri\": \"%s\",\n"
                   "    \"localizedParameters\": {\n"
                   "        \"defaultLanguage\": \"en-US\",\n"
                   "        \"en-US\": {\n"
                   "            \"titleName\": \"%s\"\n"
                   "        }\n"
                   "    }\n"
                   "}\n",
                   title_id,
                   url,
                   friendly_name);
    snprintf_clear(d.path, _countof_1(d.path), "/user/app/%s/sce_sys/param.json", title_id);

    if (install_file(d.path, d.json, strlen(d.json)))
    {
        return -1;
    }
    snprintf_clear(d.path, _countof_1(d.path), "/user/app/%s/sce_sys/icon0.png", title_id);
    if (install_file(d.path, icon0_png_data, sizeof(icon0_png_data)))
    {
        return -1;
    }
    return install_app(title_id, "/user/app/");
}

#if !defined(WEB_PORT)
#pragma error "WEB_PORT not defined"
#endif

int install_launcher(void)
{
    install_shortcut("ILNY26228", "http://127.0.0.1:" xstr(WEB_PORT), "Patch Manager");
    install_shortcut("ILNY26229", "https://idlesauce.github.io/umtx2", "idlesauce umtx2 host");
    return 0;
}
