#pragma once

#include "../shared/app_info.h"
#include "../shared/app_metadata.h"
#include "../shared/server_data.h"
#include "../shared/dynlib.h"
#include "../shared/macro.h"
#include "../shared/path.h"

struct patch_attr
{
    const char* title;
    const char* name;
    const char* note;
    const char* author;
    const char* patchVer;
    const char* appVer;
    const char* appElf;
    const char* ImageBase;
};

struct patch_line
{
    const char* type;
    const char* address;
    const char* value;
    const char* address_offset;
    const char* pad_jump_size;
    const char* cave_jump_target;
    const char* imagebase;
};

struct patch_xml_context
{
    const app_info& appinfo;
    const dynlib_info& info;
    const client_data& read_client;
    const metadata_buf& metadata;
    const char* exec_name;
    bool diag;

    int read_xml();
    void run_user_patch();
    void apply_patch(const patch_line& pline);
};
