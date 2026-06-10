#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

#include "path.h"
#include "file.h"
#include "macro.h"

#define DIR(s) s "/"

void create_default_folders(void)
{
    static const char paths[][64 + 1] = {
        DIR(USER_PATCH_PATH "/" XML_PS4_PATH),
        DIR(USER_PATCH_PATH "/" XML_PS5_PATH),
        DIR(USER_PATCH_PATH "/" XML_SYSTEM_PATH),
        DIR(BIN_DIR),
    };
    const size_t num_paths = _countof(paths);
    for (size_t i = 0; i < num_paths; i++)
    {
        const char* c_path = paths[i];
        const bool okay = mkdirs(c_path) == SUCCESS;
        printf("create (%ld/%ld) %s %s\n", i + 1, num_paths, c_path, okay ? "okay!" : "failed");
    }
}
