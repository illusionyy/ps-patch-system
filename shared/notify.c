#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "../shared/macro.h"
#include "notify.h"

void notify_fixed_(const bool display, const char* file_func_line, const char* message)
{
    extern int sceKernelSendNotificationRequest(int, SceNotificationRequest*, size_t, int);
    SceNotificationRequest req = {};
    req.Type = Message;
    req.TargetId = -1;
    strncpy(req.Message, message, _countof_1(req.Message));
    // memmove(req.Message, message, _countof_1(req.Message));
    const size_t len = strlen(req.Message);
    size_t len2 = len;
    // trim newline
    while (len2 > 0 && req.Message[len2 - 1] == '\n')
    {
        req.Message[--len2] = '\0';
    }
    printf("notify(%s) %s\n", file_func_line && *file_func_line ? file_func_line : "unknown", req.Message);
    display ? (void)sceKernelSendNotificationRequest(0, &req, sizeof(req), 0) : (void)0;
}

void notify_(const bool display, const char* file_func_line, const char* FMT, ...)
{
    char buf[NOTIFY_MSG_SIZE] = {};
    va_list args;
    va_start(args, FMT);
    vsnprintf(buf, _countof_1(buf), FMT, args);
    va_end(args);
    notify_fixed_(display, file_func_line, buf);
}

void ui_perror_(const char* str, const char* hint, const char* user)
{
    notify_(true, user, "%s: %s\n%s", str, strerror(*__error()), hint);
}
