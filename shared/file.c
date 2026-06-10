#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <dirent.h>

#include <sys/file.h>
#include <fcntl.h>

#include "file.h"
#include "macro.h"

int file_exists(const char* path)
{
    if (access(path, F_OK) == 0)
    {
        return SUCCESS;
    }

    return FAILED;
}

int mkdirs(const char* dir)
{
    char path[MAX_PATH + 1] = {0};
    snprintf(path, _countof_1(path), "%s", dir);

    char* ptr = strrchr(path, '/');
    *ptr = 0;
    ptr = path;
    ptr++;
    while (*ptr)
    {
        while (*ptr && *ptr != '/')
        {
            ptr++;
        }

        char last = *ptr;
        *ptr = 0;

        if (file_exists(path) == FAILED)
        {
            if (mkdir(path, 0777) < 0)
            {
                return FAILED;
            }
            else
            {
                chmod(path, 0777);
            }
        }

        *ptr++ = last;
        if (last == 0)
        {
            break;
        }
    }

    return SUCCESS;
}

void touch_file(const char* path)
{
    close(open(path, O_CREAT | O_CLOEXEC, 0644));
}

int write_file(const char* file_path, const void* data, const size_t size)
{
    FILE* fp = 0;
    if (!file_path || !data)
    {
        return -__LINE__;
    }
    fp = fopen(file_path, "wb");
    if (!fp)
    {
        return -__LINE__;
    }

    if (fwrite(data, 1, size, fp) != size)
    {
        fclose(fp);
        return -__LINE__;
    }
    fclose(fp);
    return 0;
}

int read_file(const char* file_path, void* data, const size_t size)
{
    FILE* fp = 0;
    if (!file_path || !data)
    {
        return -__LINE__;
    }
    fp = fopen(file_path, "rb");
    if (!fp)
    {
        return -__LINE__;
    }
    if (fread(data, 1, size, fp) != size)
    {
        fclose(fp);
        return -__LINE__;
    }
    fclose(fp);
    return 0;
}
