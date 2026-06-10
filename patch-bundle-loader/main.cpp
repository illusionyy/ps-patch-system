#include <stdio.h>
#include <stdint.h>  // need this order nya
#include <unistd.h>  // need this order nya
#include <sys/sysctl.h>
#include <sys/user.h>
#include <sys/syscall.h>

extern "C"
{
#include "../shared/debug.h"
#include "../shared/file.h"
#include "../shared/macro.h"
#include "../shared/path.h"
#include "../shared/patch-common.h"
#include <stdlib.h>
#include <elfldr.h>
#include <fast-lzma2.h>
const char* basename(const char*);
}

#include "../shared/notify.h"
#include "../patch-bundle/utils.hpp"

#if defined(__PROSPERO__)
#include <generated/patch_bundle_prospero_elf.inc.h>
#elif defined(__ORBIS__)
#include <generated/patch_bundle_orbis_elf.inc.h>
#else
#error "unsupport platform nya"
#endif

#define MAP_FAILED (void*)(-1)
static const int O_WRONLY_ = 2;
extern "C" int open(const char* path, int mode);

static void __attribute__((constructor)) set_thread_name(void)
{
    syscall(SYS_thr_set_name, -1, ELF_NAME);
    puts("set thread name to " ELF_NAME);
}

#define LOCAL_PATCH BIN_DIR "/patch-bundle.elf"

int main()
{
    signal(SIGPIPE, SIG_IGN);
    notify(
        "%s start\n"
        "Built: " DATE_TIME "\n",
        ELF_NAME);
#if defined(__PROSPERO__)
    const char* patch_bundle_elf_path = patch_bundle_prospero_elf_path;
    const uint8_t* patch_bundle_elf_data = patch_bundle_prospero_elf_data;
    const size_t patch_bundle_elf_len = sizeof(patch_bundle_prospero_elf_data);
#elif defined(__ORBIS__)
    const char* patch_bundle_elf_path = patch_bundle_orbis_elf_path;
    const uint8_t* patch_bundle_elf_data = patch_bundle_orbis_elf_data;
    const size_t patch_bundle_elf_len = sizeof(patch_bundle_orbis_elf_data);
#endif
    const char* bpp = basename(patch_bundle_elf_path);
    const char* bp = bpp && bpp[0] != 0 ? bpp : "unknown";
    // trust me nya, this will work
    const size_t decompress_size = FL2_findDecompressedSize(patch_bundle_elf_data, patch_bundle_elf_len);
    if (FL2_isError(decompress_size))
    {
        notify(
            "failed to decompress %s!\n"
            "FL2_getErrorName() says: %s\n",
            bp,
            FL2_getErrorName(decompress_size));
        return -(__LINE__);
    }
    printf("decompress_size for %s is %ld bytes\n", bp, decompress_size);
    void* decompress_mem = 0;
    func_ret_fmt(decompress_mem = malloc(decompress_size), "%p");
    if (decompress_mem && decompress_mem != MAP_FAILED)
    {
        size_t r = 0;
        func_ret_fmt(r = FL2_decompress(decompress_mem, decompress_size, patch_bundle_elf_data, patch_bundle_elf_len), "%ld");
        if (r == decompress_size)
        {
            create_default_folders();
            const int write_res = write_file(LOCAL_PATCH, decompress_mem, decompress_size);
            const bool write_okay = write_res == 0;
            char write_buf[32 + 1] = {};
            if (!write_okay)
            {
                snprintf(write_buf, _countof_1(write_buf), " (returned %d)", write_res);
            }
            notify("write local copy to %s is %s%s\n", LOCAL_PATCH, write_okay ? "okay" : "failed", write_buf);
            printf("FL2_decompress for %s okay!\n", bp);
            const int console_fd = open("/dev/console", O_WRONLY_);  // not enough permission yet, so open it for them
            const char* const argv[] = {bp, 0};
            int pid = 0;
            func_ret(pid = elfldr_spawn(console_fd, (char* const*)argv, (uint8_t*)decompress_mem, decompress_size));
            print_load_const(notify, bp, pid, decompress_size);
            free(decompress_mem);
            return 0;
        }
        return -(__LINE__);
    }
    else
    {
        // maybe webkit or other low memory envoriemnt, warn them nya!
        notify(
            "failed to allocate memory for %s!\n"
            "try loading uncompressed version.\n"
            "decompress_mem %p",
            bp,
            decompress_mem);
        printf("nyahuhuhu, bailing now\n");
        return -(__LINE__);
    }
    return 0;
}
