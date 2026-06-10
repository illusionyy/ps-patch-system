#include "dynlib.h"
#include "macro.h"
#include <stdio.h>
#include "memory.h"
#include "proc.h"

void dump_dynlib_obj_(const dynlib_obj_t* obj, const char* user)
{
    return;
    const unsigned long base = obj->mapbase;

#define PRINT_HEX(field) printf("%s: %-20s [+0x%04lx] 0x%lx\n", user, #field, __builtin_offsetof(dynlib_obj_t, field), (unsigned long)obj->field)
#define PRINT_DEC(field) printf("%s: %-20s [+0x%04lx] %d\n", user, #field, __builtin_offsetof(dynlib_obj_t, field), obj->field)
#define PRINT_UINT(field) printf("%s: %-20s [+0x%04lx] %u\n", user, #field, __builtin_offsetof(dynlib_obj_t, field), obj->field)
#define PRINT_ADDR(field) printf("%s: %-20s [+0x%04lx] 0x%lx (base+0x%lx)\n", user, #field, __builtin_offsetof(dynlib_obj_t, field), obj->field, obj->field - base)
#define PRINT_ARR(field, i) printf("%s: %-20s [+0x%04lx] 0x%lx\n", user, #field "[" #i "]", __builtin_offsetof(dynlib_obj_t, field), obj->field[i])

    PRINT_HEX(next);
    PRINT_HEX(path);
    {
        char path[1024] = {};
        if (kernel_dynlib_path_from_obj(obj, path, sizeof(path), _countof(path)) == 0)
        {
            printf("%s: %-20s %s\n", user, "path", path);
        }
    }
    PRINT_UINT(refcount);
    PRINT_HEX(handle);

    PRINT_HEX(mapbase);
    PRINT_HEX(mapsize);
    PRINT_HEX(textsize);

    PRINT_HEX(database);
    PRINT_HEX(datasize);

    PRINT_HEX(unknown1);
    PRINT_HEX(unknown1size);

    PRINT_ADDR(entry);
    PRINT_HEX(vaddrbase);

    PRINT_UINT(tlsindex);
    PRINT_ADDR(tlsinit);
    PRINT_HEX(tlsinitsize);
    PRINT_HEX(tlssize);
    PRINT_HEX(tlsoffset);
    PRINT_HEX(tlsalign);

    PRINT_ADDR(pltgot);

    PRINT_ADDR(init);
    PRINT_ADDR(fini);

    PRINT_ADDR(eh_frame_hdr);
    PRINT_HEX(eh_frame_hdr_size);
    PRINT_ADDR(eh_frame);
    PRINT_HEX(eh_frame_size);

    PRINT_DEC(status);
    PRINT_HEX(flags);

    hex_dump(obj, sizeof(*obj), 0);

#undef PRINT_HEX
#undef PRINT_DEC
#undef PRINT_UINT
#undef PRINT_ADDR
#undef PRINT_ARR
}

static size_t offset_proc(void)
{
    return
#if defined(__ORBIS__)
        0x340
#elif defined(__PROSPERO__)
        0x3e8
#else
#error "not support"
#endif
        ;
}

int kernel_dynlib_handle_list(const int pid, unsigned int* handles_out, const unsigned long max_count)
{
    unsigned long kproc;
    unsigned long kaddr;
    long temphandle;
    unsigned int count = 0;

    if (!(kproc = kernel_get_proc(pid)))
    {
        return -__LINE__;
    }

    if (kernel_copyout(kproc + offset_proc(), &kaddr, sizeof(kaddr)) < 0)
    {
        return -__LINE__;
    }

    do
    {
        if (kernel_copyout(kaddr, &kaddr, sizeof(kaddr)) < 0)
        {
            return -__LINE__;
        }
        if (!kaddr)
        {
            break;
        }
        if (kernel_copyout(kaddr + __builtin_offsetof(dynlib_obj_t, handle),
                           &temphandle,
                           sizeof(temphandle)) < 0)
        {
            return -__LINE__;
        }

        if (count < max_count)
        {
            handles_out[count++] = (unsigned int)temphandle;
        }

    } while (1);

    return (int)count;
}

int kernel_dynlib_info2(const int pid, const unsigned int handle, dynlib_info* info)
{
    unsigned long kproc;
    unsigned long kaddr;
    unsigned long kpath;
    long temphandle;

    if (!(kproc = kernel_get_proc(pid)))
    {
        return -__LINE__;
    }

    if (kernel_copyout(kproc + offset_proc(), &kaddr, sizeof(kaddr)) < 0)
    {
        return -__LINE__;
    }

    do
    {
        if (kernel_copyout(kaddr, &kaddr, sizeof(kaddr)) < 0)
        {
            return -__LINE__;
        }
        if (!kaddr)
        {
            return -__LINE__;
        }
        if (kernel_copyout(kaddr + __builtin_offsetof(dynlib_obj_t, handle),
                           &temphandle,
                           sizeof(temphandle)) < 0)
        {
            return -__LINE__;
        }

        if ((unsigned int)temphandle != handle)
        {
            continue;
        }

        if (kernel_copyout(kaddr, &info->obj, sizeof(info->obj)) < 0)
        {
            return -__LINE__;
        }

        if (kernel_copyout(kaddr + __builtin_offsetof(dynlib_obj_t, path),
                           &kpath,
                           sizeof(kpath)) < 0)
        {
            return -__LINE__;
        }
        if (kernel_copyout(kpath, info->path, sizeof(info->path)) < 0)
        {
            return -__LINE__;
        }
        info->path[_countof_1(info->path)] = '\0';
        if (sys_thr_get_name(pid, info->name, sizeof(info->name)))
        {
            perror("sys_thr_get_name");
            return -__LINE__;
        }
        info->name[_countof_1(info->name)] = '\0';

        return 0;

    } while (1);
    return -__LINE__;
}

int kernel_dynlib_path_from_obj(const dynlib_obj_t* obj, char* path, const unsigned long path_sz, const unsigned long path_cnt)
{
    if (!obj || !path)
    {
        return -__LINE__;
    }
    if (kernel_copyout(obj->path, path, path_sz) < 0)
    {
        return -__LINE__;
    }
    path[path_cnt - 1] = '\0';
    return 0;
}
