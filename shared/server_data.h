#pragma once

#include <stdint.h>
#include <unistd.h>
#define WEB_PORT 23900
#define SERVER_PORT WEB_PORT + 16
#define SERVER_HOST "0.0.0.0"

typedef struct client_data
{
    pid_t clientPid;
    uint32_t appid;
    enum res_code : uint32_t
    {
        res_none = 0,
        res_okay = 1,
    } res;
} client_data;
