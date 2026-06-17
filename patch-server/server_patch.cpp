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

#include "server_120hz_patch.hpp"
#include "patch_web_xml.hpp"

void patch_prx_load_for_signal(patch_frame_context& ctx, const client_data& read_client);
void upload_init_env_ptr(const uint32_t i, const client_data& read_client);

static bool is_forbidden_app(const uint32_t name)
{
    switch (name)
    {
        case sid("SceShellUI"):
        case sid("SceNKUIProcess"):           // ps4
        case sid("SceNKNetworkProcess"):      // ps4
        case sid("SceNKWebProcess"):          // ps5
        case sid("SceShellCore"):             // impossible unless you send sigusr1 with shellcore info
        case sid("SceCdlgApp"):               // common dialog
        case sid("coredump.elf"):             // crash reporter
        case sid("gpudump.elf"):              // crash reporter, ps5 only?
        case sid("orbis-jsc-compiler."):      // ps4
        case sid("orbis-jsc-compiler.self"):  // ps4
        case sid("SecureWebProcess.se"):      // ps4
        case sid("SecureWebProcess.self"):    // ps4
        case sid("SecureUIProcess.sel"):      // ps4
        case sid("SecureUIProcess.self"):     // ps4
        // ours, oops!
        case sid("patch.elf"):
        case sid("patch-server.elf"):
        case sid("patch-web.elf"):
        case sid("patch-bundle-orbis.elf"):
        case sid("patch-bundle-loader-orbis.elf"):
        case sid("patch-bundle-prospero.elf"):
        case sid("patch-bundle-loader-prospero.elf"):
        {
            printf("forbidden title '#%08x\n", name);
            return true;
        }
        default:
        {
            break;
        }
    }
    return false;
}

static bool is_forbidden_titleid(const uint32_t name)
{
    switch (name)
    {
        case sid("NPXS22010"):  // common dialog, SceCdlgApp ps4
        case sid("NPXS40093"):  // common dialog, SceCdlgApp ps5
        case sid("NPXS21016"):  // SceSpZeroConf ps4
        case sid("NPXS40112"):  // SceSpZeroConf ps5
            // case sid("CUSA00960"):  // Vue
            {
                return true;
            }
        default:
        {
            break;
        }
    }
    return false;
}

static PatchRegistry g_patch_registry;

struct dynlib_list
{
    dynlib_info first;
#if defined(__PROSPERO__)
    dynlib_info videoout;
    dynlib_info vdec2;
#endif
};
#if defined(__PROSPERO__)
static void do_120hz_patch(const pid_t pid, dynlib_list& info_list, patch_120hz::patch_120hz_context& hfr)
{
    if (apply_120hz_enabled() && !g_patch_registry.m_hfr_patched)
    {
        constexpr auto videoOut_h = sid("libSceVideoOut.sprx");
        constexpr auto vdec2_h = sid("libSceVideodec2.sprx");
        const uint32_t videoOut_m = g_patch_registry.find_module(pid, videoOut_h);
        const uint32_t vdec2_m = g_patch_registry.find_module(pid, vdec2_h);
        if (videoOut_m && !hfr.m_vo_handle)
        {
            kernel_dynlib_info2(pid, videoOut_m, &info_list.videoout);
            hfr.m_videoout = &info_list.videoout;
            hfr.m_vo_handle = (uint32_t)hfr.m_videoout->obj.handle;
        }
        if (vdec2_m && !hfr.m_vdec2_handle)
        {
            kernel_dynlib_info2(pid, vdec2_m, &info_list.vdec2);
            hfr.m_vdec2 = &info_list.vdec2;
            hfr.m_vdec2_handle = (uint32_t)hfr.m_vdec2->obj.handle;
        }
        if (hfr.ucc.isUCC_TitleID && hfr.m_vo_handle && hfr.m_vdec2_handle)
        {
            hfr.run_120hz_videoout_patch();
            g_patch_registry.m_hfr_patched = true;
        }
        else if (!hfr.ucc.isUCC_TitleID && hfr.m_videoout)
        {
            hfr.run_120hz_videoout_patch();
            g_patch_registry.m_hfr_patched = true;
        }
    }
}
#endif

static void run_prx_load_patch(const client_data& read_client, const uint32_t id_h)
{
    uint32_t modules[256] = {};
    ssize_t mod_n = 0;
    dynlib_list info_list = {};
    const int rf = kernel_dynlib_info2(read_client.clientPid, 0, &info_list.first);
    const ssize_t module_count = (ssize_t)kernel_dynlib_handle_list(read_client.clientPid, modules, _countof(modules));
    if (rf == 0 && module_count > 0)
    {
#if defined(__PROSPERO__)
        patch_120hz::patch_120hz_context hfr = {};
        hfr.m_pid = read_client.clientPid;
        hfr.ucc.isUCC_TitleID = hfr.isUCC(id_h);
#endif
        for (ssize_t i = mod_n; i < module_count; i++)  // 1 because already used by info_first
        {
            const uint32_t module_handle = modules[i];
            dynlib_info info = {};
            const int r = kernel_dynlib_info2(read_client.clientPid, module_handle, &info);
            if (r == 0)
            {
                const char* prx_base = basename(info.path);
                if (prx_base && prx_base[0])
                {
                    const auto prx_base_h = sid(prx_base);
                    if (!g_patch_registry.is_patched(read_client.clientPid, module_handle, prx_base_h))
                    {
#if defined(__PROSPERO__)
                        do_120hz_patch(read_client.clientPid, info_list, hfr);
#endif
                        {
                            switch (prx_base_h)
                            {
                                case sid("libc.prx"):
                                {
                                    // upload_init_env_ptr(module_handle, read_client);
                                    break;
                                }
                                case sid("libSceVideoOut.sprx"):
                                {
                                    break;
                                }
                            }
                        }
                        g_patch_registry.mark_patched(read_client.clientPid, module_handle, prx_base_h);
                        // if (0)
                        {
                            printf("proc=%s pid=%d module=%s handle=0x%x\n",
                                   info_list.first.path,
                                   read_client.clientPid,
                                   info.path,
                                   module_handle);
                        }
                    }
                }
                else
                {
                    if (0)
                    {
                        printf("Already patched, skipping: proc=%s pid=%d module=%s handle=0x%x\n",
                               info_list.first.path,
                               read_client.clientPid,
                               info.path,
                               module_handle);
                    }
                }
            }
            else
            {
                printf("kernel_dynlib_info2 failed %d\n", r);
            }
        }
    }
}

void run_elf_user_patch(client_data& read_client)
{
    const bool enable_prx_patch = true;
    app_info appinfo = {};
    dynlib_info info = {};
    const int r = kernel_dynlib_info2(read_client.clientPid, 0, &info);
    if (r == 0 && sceKernelGetAppInfo(read_client.clientPid, &appinfo) == 0)
    {
        // dump_dynlib_obj(&info.obj);
        read_client.appid = appinfo.app_id;
        printf("%d %s %s %s\n", read_client.clientPid, info.name, appinfo.title_id, info.path);

        const auto name_hash = sid(info.name);
        const auto id_hash = sid(appinfo.title_id);

        if (g_patch_registry.clear_if_exists(read_client.clientPid))
        {
            printf("Cleared stale patch state for pid %d\n", read_client.clientPid);
        }

        if (!is_forbidden_app(name_hash) && !is_forbidden_titleid(id_hash))
        {
            patch_frame_context libkernel_eh_frame_patch = {};
            kernel_dynlib_obj(read_client.clientPid, 0x2001, &libkernel_eh_frame_patch.frame_info.obj);
            if (!libkernel_eh_frame_patch.frame_info.obj.eh_frame || !libkernel_eh_frame_patch.frame_info.obj.eh_frame_size)
            {
                memset(&libkernel_eh_frame_patch, 0, sizeof(libkernel_eh_frame_patch));
                kernel_dynlib_obj(read_client.clientPid, 0x1, &libkernel_eh_frame_patch.frame_info.obj);
            }
            if (enable_prx_patch && libkernel_eh_frame_patch.frame_info.obj.eh_frame && libkernel_eh_frame_patch.frame_info.obj.eh_frame_size)
            {
                libkernel_eh_frame_patch.frame_start = libkernel_eh_frame_patch.frame_info.obj.eh_frame;
                libkernel_eh_frame_patch.frame_size = libkernel_eh_frame_patch.frame_info.obj.eh_frame_size;
                func_ret(kill(read_client.clientPid, SIGSTOP));
                patch_prx_load_for_signal(libkernel_eh_frame_patch, read_client);
                func_ret(kill(read_client.clientPid, SIGCONT));
            }
            metadata_buf meta = {};
            const int mr = get_app_metadata(read_client.clientPid, &meta);
            const char* mod_base = basename(info.path);
            if (mr == 0 && mod_base && *mod_base)
            {
                patch_xml_context patch = {
                    .appinfo = appinfo,          //
                    .info = info,                //
                    .read_client = read_client,  //
                    .metadata = meta,            //
                    .exec_name = mod_base        //
                };
                patch.read_xml();
                run_prx_load_patch(read_client, id_hash);
            }
        }
    }
}

void run_patch(int signo, pid_t pid)
{
    client_data read_client = {};
    app_info appinfo = {};
    if (signo == SIGUSR2 && sceKernelGetAppInfo(pid, &appinfo) == 0)
    {
        read_client.clientPid = pid;
        read_client.appid = appinfo.app_id;
        const auto id_hash = sid(appinfo.title_id);
        run_prx_load_patch(read_client, id_hash);
        {
            int r = 0;
            perror_on_non_zero(r = kill(read_client.clientPid, SIGCONT), r);
            if (r)
            {
                notify(
                    "failed to send sigcont to pid %d\n"
                    "trying one more time.",
                    read_client.clientPid);
                perror_on_non_zero(r = kill(read_client.clientPid, SIGCONT), r);
                if (r)
                {
                    notify("somehow failed to sigcont pid %d again, killing it now!\n", read_client.clientPid);
                    perror_on_non_zero(r = kill(read_client.clientPid, SIGKILL), r);
                    // perror_on_non_zero(r = kill(read_client.clientPid, SIGTERM), r);
                }
            }
        }
    }
}
