#pragma once

#include "macro.h"
#include <sys/cdefs.h>

#include <stdbool.h>
#include <stdint.h>

#define NOTIFY_MSG_SIZE 0x400  // 1024

enum SceNotificationRequestType
{
    Message = 0,
    MessageMsgId = 1,
    MessageUserId = 2,
    MessageDeviceId = 3,
    MessageDeviceIdRelatedToUser = 4,
    MessageText = 5,
    MessageTextRelatedToUser = 6,
    MessageErrorCode = 7,
    MessageAppId = 8,
    MessageAppInfo = 9,
    MessageTitleRelatedToUser = 10,
    MessageParams = 11,
    MessageUserName = 12,
    MessageUserNameInfo = 13,
    MessageAddressing = 14,
    MessageAddressingDeviceId = 15,
    MessageAddressingUserName = 16,
    MessageAddressingUserId = 17,

    DebugMessage = 100,
    TrcCheckMessage = 101,
    NpDebugMessage = 102,
    WebDebugMessage = 102,
    UNK_103 = 103,
};

enum SystemNotificationPriority
{
    Default = 0,
    High = 1,
    Special = -1
};

enum SystemNotificationAttribute
{
    ShowVideoPlayback = 1,
    ShowVR = 2,
    ShowKratos = 4,
    All = 7,
};

enum NotificationAPI
{
    ToastPopup = 0,
    NotifyDatabase = 1,
};

#pragma pack(push, 1)
typedef struct SceNotificationRequest
{
    enum SceNotificationRequestType Type;  // 0x00
    uint32_t ReqId;                        // 0x04
    uint32_t Priority;                     // 0x08
    uint32_t MsgId;                        // 0x0C
    uint32_t TargetId;                     // 0x10
    uint32_t UserId;                       // 0x14
    uint32_t DeviceId;                     // 0x18
    uint32_t AddressingUserId;             // 0x1C
    uint32_t AppId;                        // 0x20
    uint32_t ErrorNumber;                  // 0x24
    uint32_t Attribute;                    // 0x28
    uint8_t HasIcon;                       // 0x2C
    union
    {
        struct
        {
            char Message[NOTIFY_MSG_SIZE];  // 0x2D
            char IconImageUri[0x800];       // 0x42D
        };

        struct
        {
            char arg1[180];  // 0x2D
            char arg2[180];  // 0xE1
            char arg3[180];  // 0x195
        };

        struct  // Ensure proper size.
        {
            char buffer[0xC03];  // 0x2D
        };
    };
} SceNotificationRequest;
#pragma pack(pop)

__BEGIN_DECLS
void notify_fixed_(const bool display, const char* file_func_line, const char* message);
void notify_(const bool display, const char* file_func_line, const char* FMT, ...);
void ui_perror_(const char* str, const char* hint, const char* user);
__END_DECLS

#define _SRC_YES FILE_FUNC_LINE
#define _SRC_NONE 0
#define _SRC(u) _SRC_##u

#define HAS_SRC YES
#define HAS_NO_SRC NONE

#define _notify_static(u, a) notify_fixed_(true, _SRC(u), a)
#define _notify(u, ...) notify_(true, _SRC(u), __VA_ARGS__)
#define _notify_static_cond(u, c, a) notify_fixed_(c, _SRC(u), a)
#define _notify_cond(u, c, ...) notify_(c, _SRC(u), __VA_ARGS__)

#define notify_static(a) _notify_static(HAS_SRC, a)
#define notify(...) _notify(HAS_SRC, __VA_ARGS__)
#define notify_static_cond(c, a) _notify_static_cond(HAS_SRC, c, a)
#define notify_cond(c, ...) _notify_cond(HAS_SRC, c, __VA_ARGS__)

#define unotify_static(a) _notify_static(HAS_NO_SRC, a)
#define unotify(...) _notify(HAS_NO_SRC, __VA_ARGS__)
#define unotify_static_cond(c, a) _notify_static_cond(HAS_NO_SRC, c, a)
#define unotify_cond(c, ...) _notify_cond(HAS_NO_SRC, c, __VA_ARGS__)

#define ui_perror(s, h) ui_perror_(s, h, FILE_FUNC_LINE)
