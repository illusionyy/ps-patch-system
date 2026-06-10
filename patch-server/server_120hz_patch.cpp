#include "server_120hz_patch.hpp"
#include "../shared/memory.h"
#include "../shared/proc_rw.h"
#include "../shared/notify.h"

#if defined(__PROSPERO__)
bool patch_120hz::patch_120hz_context::isUCC(const uint32_t name)
{
    switch (name)
    {
        case sid("CUSA01399"):
        case sid("CUSA02320"):
        case sid("CUSA02343"):
        case sid("CUSA02344"):
        case sid("CUSA02826"):
        // PSN
        case sid("CUSA03280"):
        case sid("CUSA03281"):
        case sid("CUSA03282"):
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

static void relocate_symbol_to_hook_(const int pid, const uintptr_t dst, const char* symbol, const dynlib_info& scan_mod, const uint32_t symbol_mod, const char* user)
{
    const uintptr_t symbol_addr = kernel_dynlib_dlsym(pid, symbol_mod, symbol);
    if (symbol_addr)
    {
        // this is gonna hurt, nyaaa
        const uintptr_t source_sym = pid_chunk_scan_data(pid, scan_mod.obj.mapbase, scan_mod.obj.mapsize, &symbol_addr, sizeof(symbol_addr), 0);
        printf("module %s source_sym 0x%lx for %s\n", scan_mod.path, source_sym, symbol);
        if (source_sym)
        {
            userland_copyin(pid, &dst, source_sym, sizeof(dst));
        }
    }
    else
    {
        printf("can't find symbol %s with handle 0x%08x for %s\n", symbol, symbol_mod, user);
    }
}

#define relocate_symbol_to_hook(p, s, m, h) relocate_symbol_to_hook_(p, (codebase + sym_hook(s)), #s, m, h, FILE_FUNC_LINE)

void patch_120hz::patch_120hz_context::apply_videoout_hooks()
{
    notify(__FUNCSIG__ ": searching for functions, this will take sometime...");
    uint32_t modules[256] = {};
    ssize_t mod_n = 0;
    dynlib_info info_first = {};
    const int rf = kernel_dynlib_info2(m_pid, 0, &info_first);
    const ssize_t module_count = (ssize_t)kernel_dynlib_handle_list(m_pid, modules, _countof(modules));
    if (rf == 0 && module_count > 0)
    {
        for (ssize_t i = mod_n; i < module_count; i++)
        {
            const uint32_t module_handle = modules[i];
            if (module_handle == m_vo_handle)
            {
                continue;
            }
            dynlib_info info = {};
            int r = kernel_dynlib_info2(m_pid, module_handle, &info);
            if (r == 0)
            {
                relocate_symbol_to_hook(m_pid, sceVideoOutOpen, info, m_vo_handle);
                relocate_symbol_to_hook(m_pid, sceVideoOutRegisterBuffers, info, m_vo_handle);
                if (m_vdec2)
                {
                    relocate_symbol_to_hook(m_pid, sceVideodec2CreateDecoder, info, m_vdec2_handle);
                    relocate_symbol_to_hook(m_pid, sceVideodec2DeleteDecoder, info, m_vdec2_handle);
                }
            }
        }
    }
    notify(__FUNCSIG__ ": finalize");
}

void patch_120hz::patch_120hz_context::apply_vdec2_hooks()
{
    notify(__FUNCSIG__ ": searching for functions, this will take sometime...");
    notify(__FUNCSIG__ ": finalize");
}

void patch_120hz::patch_120hz_context::upload_symbols()
{
    codebase = m_videoout->obj.eh_frame;
    uint8_t code_copy[sizeof(shellcode)];
    memcpy(code_copy, shellcode, sizeof(shellcode));

    static struct sym_entry usual_symlist[] = {
        sym(sceVideoOutOpen, 0),
        sym(sceVideoOutSetFlipRate, 0),
        sym(sceVideoOutConfigureOutput, 0),
        sym(sceVideoOutIsOutputSupported, 0),
        sym(sceVideoOutRegisterBuffers, 0),
        sym(memset, 0x2),
        sym(vsnprintf, 0x2),
        sym(strlen, 0x2),
        sym(printf, 0x2),
        sym(sceKernelSendNotificationRequest, 0x2001),
        sym(strerror, 0x2),
        sym(strncpy, 0x2),
    };

    for (size_t i = 0; i < _countof(usual_symlist); i++)
    {
        switch (sid(usual_symlist[i].name))
        {
            case sid("sceVideoOutOpen"):
            case sid("sceVideoOutSetFlipRate"):
            case sid("sceVideoOutConfigureOutput"):
            case sid("sceVideoOutIsOutputSupported"):
            case sid("sceVideoOutRegisterBuffers"):
            {
                usual_symlist[i].handle = (uint32_t)m_videoout->obj.handle;
                break;
            }
            default:
            {
                break;
            }
        }
    }
    int sym_r = 0;
    sym_r = write_syms(m_pid, code_copy, usual_symlist, _countof(usual_symlist));
    if (sym_r == 0)
    {
        printf("upload %ld usual_symlist okay!\n", _countof(usual_symlist));
    }
    else
    {
        printf("failed to upload %d usual_symlist\n", sym_r);
        return;
    }
    if (m_vdec2)
    {
        static struct sym_entry vdec2_symlist[] = {
            sym(sceVideodec2CreateDecoder, 0),
            sym(sceVideodec2DeleteDecoder, 0),
        };
        for (size_t i = 0; i < _countof(vdec2_symlist); i++)
        {
            switch (sid(vdec2_symlist[i].name))
            {
                case sid("sceVideodec2CreateDecoder"):
                case sid("sceVideodec2DeleteDecoder"):
                {
                    const uint32_t h = (uint32_t)m_vdec2->obj.handle;
                    vdec2_symlist[i].handle = h;
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
        sym_r = write_syms(m_pid, code_copy, vdec2_symlist, _countof(vdec2_symlist));
        if (sym_r == 0)
        {
            printf("upload %ld vdec2_symlist okay!\n", _countof(vdec2_symlist));
        }
        else
        {
            printf("failed to upload %d vdec2_symlist\n", sym_r);
            return;
        }
    }
    debugf("codebase: %lx\n", codebase);
    userland_copyin(m_pid, code_copy, codebase, sizeof(code_copy));
}

void patch_120hz::patch_120hz_context::run_120hz_videoout_patch()
{
    const int pid = m_pid;
    const int kMaxProt = (PROT_EXEC | PROT_READ | PROT_WRITE);
    func_ret(kernel_mprotect(pid, m_videoout->obj.mapbase, m_videoout->obj.mapsize, kMaxProt));
    if (m_vdec2)
    {
        func_ret(kernel_mprotect(pid, m_vdec2->obj.mapbase, m_vdec2->obj.mapsize, kMaxProt));
    }
    upload_symbols();
    apply_videoout_hooks();
    if (ucc.isUCC_TitleID && m_vdec2)
    {
        apply_vdec2_hooks();
    }
}
#endif
