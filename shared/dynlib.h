#pragma once

#include <sys/cdefs.h>

#if defined(__PROSPERO__)
/**
 * an ELF loaded in kernel memory.
 **/
typedef struct dynlib_obj
{
    unsigned long next;
    unsigned long path;
    unsigned long unknown0[2];
    unsigned int refcount;
    unsigned long handle;

    unsigned long mapbase;
    unsigned long mapsize;
    unsigned long textsize;

    unsigned long database;
    unsigned long datasize;

    unsigned long unknown1;
    unsigned long unknown1size;

    unsigned long entry;
    unsigned long unknown2;
    unsigned long vaddrbase;

    unsigned int tlsindex;
    unsigned long tlsinit;
    unsigned long tlsinitsize;
    unsigned long tlssize;
    unsigned long tlsoffset;
    unsigned long tlsalign;

    unsigned long pltgot;

    unsigned long unknown3[6];

    unsigned long init;
    unsigned long fini;

    unsigned long eh_frame_hdr;
    unsigned long eh_frame_hdr_size;

    unsigned long eh_frame;
    unsigned long eh_frame_size;

    int status;
    int flags;

    unsigned long unknown4[5];
    unsigned long dynsec;
    unsigned long unknown5[6];  // fingerprint?
} dynlib_obj_t;

__BEGIN_DECLS
int kernel_dynlib_obj(int pid, unsigned int handle, dynlib_obj_t* obj);
__END_DECLS
#include <ps5/kernel.h>
#elif defined(__ORBIS__)
#include <ps4/kernel.h>
#endif

typedef struct dynlib_info
{
    dynlib_obj_t obj;
    char path[1024];
    char name[1024];
} dynlib_info;

__BEGIN_DECLS
void dump_dynlib_obj_(const dynlib_obj_t* obj, const char* user);

int kernel_dynlib_handle_list(const int pid, unsigned int* handles_out, const unsigned long max_count);
int kernel_dynlib_info2(const int pid, const unsigned int handle, dynlib_info* info);
int kernel_dynlib_path_from_obj(const dynlib_obj_t* obj, char* path, const unsigned long path_sz, const unsigned long path_cnt);
__END_DECLS

#define dump_dynlib_obj(o) dump_dynlib_obj_(o, FILE_FUNC_LINE)
