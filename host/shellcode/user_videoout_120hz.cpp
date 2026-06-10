#include <stdint.h>
#include <stdbool.h>

extern "C"
{
#include "../../shared/notify.c"
int sceVideoOutOpen(const uint32_t param_1, const uint32_t param_2, const uint32_t param_3, const void* param_4);
int sceVideoOutSetFlipRate(const uint32_t param_1, const uint32_t param_2);
int sceVideoOutConfigureOutput(const uint32_t handle, const uint32_t request_type, const void* param_3, const void* param_4, const void* param_5);
int sceVideoOutIsOutputSupported(const uint32_t handle, const uint32_t request_type, const void* param_3, const void* param_4, const void* param_5);
int sceVideoOutRegisterBuffers(const uint32_t param_1, const uint32_t param_2, const void* param_3, const uint32_t param_4, const void* param_5);
// vdec for big collection
// but really, i should fix the game and not the module
int sceVideodec2CreateDecoder(const void* param_1, const void* param_2, const void* param_3);
int sceVideodec2DeleteDecoder(const void* param_1);
}

// wrapper for unused fields
static int sceVideoOutIsOutputSupported2(const uint32_t h, const uint32_t r)
{
    return sceVideoOutIsOutputSupported(h, r, 0, 0, 0);
}
static int sceVideoOutConfigureOutput2(const uint32_t h, const uint32_t r)
{
    return sceVideoOutConfigureOutput(h, r, 0, 0, 0);
}

bool g_isBigCollection = false;
static bool print_debug_notify = true;
static uint32_t g_handle = 0;

static void set120hz(const uint32_t handle)
{
    const uint32_t m120hz = 15;
    const int supported = sceVideoOutIsOutputSupported2(handle, m120hz);
    const bool is_supported = supported > 0;
    if (is_supported)
    {
        unotify(
            "sceVideoOutIsOutputSupported 0x%x\n"
            "sceVideoOutConfigureOutput 0x%x\n",
            supported,
            sceVideoOutConfigureOutput2(handle, m120hz));
        return;
    }
    unotify(
        "sceVideoOutIsOutputSupported returned 0x%x\n"
        "%s",
        supported,
        is_supported ? "maybe your display is supported." : "is shellcore patched or display supported?");
}

extern "C"
{
int __export_sceVideoOutOpen_hook(const uint32_t param_1, const uint32_t param_2, const uint32_t param_3, const void* param_4)
{
    const int r = sceVideoOutOpen(param_1, param_2, param_3, param_4);
    if (print_debug_notify)
    {
        unotify("sceVideoOutOpen 0x%x", r);
    }
    g_handle = r;
    set120hz(r);
    return r;
}

int __export_sceVideoOutRegisterBuffers_hook(const uint32_t param_1, const uint32_t param_2, const void* param_3, const uint32_t param_4, const void* param_5)
{
    if (print_debug_notify)
    {
        notify(FILE_FUNC_LINE);
    }
    set120hz(param_1);
    return sceVideoOutRegisterBuffers(param_1, param_2, param_3, param_4, param_5);
}

// Big collection fix, it should be possible to run video decoder at arbitrary rate
// but I didn't bother looking into it
int __export_sceVideodec2CreateDecoder_hook(const void* param_1, const void* param_2, const void* param_3)
{
    if (print_debug_notify)
    {
        notify(FILE_FUNC_LINE);
    }
    // half rate vsync, 120hz to 60hz
    if (g_isBigCollection)
    {
        sceVideoOutSetFlipRate(g_handle, 1);
    }
    return sceVideodec2CreateDecoder(param_1, param_2, param_3);
}

int __export_sceVideodec2DeleteDecoder_hook(const void* param_1)
{
    if (print_debug_notify)
    {
        notify(FILE_FUNC_LINE);
    }
    if (g_isBigCollection)
    {
        // reset to full rate vsync
        sceVideoOutSetFlipRate(g_handle, 0);
    }
    return sceVideodec2DeleteDecoder(param_1);
}
}
