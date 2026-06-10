// C don't seem to support `static_assert`

#include "macro.h"
#include "notify.h"
static_size_assert(sizeof(SceNotificationRequest), 0xc30);
#include "app_info.h"
// `__ORBIS__` is 0x48
// `__PROSPERO__` is 0x58
static_assert_expression(sizeof(app_info) == 0x58);  // so that its sure that `__ORBIS__` leaves about 16 bytes of padded space
static_assert_expression(sizeof(app_info) == 0x48 || sizeof(app_info) == 0x58);

#if defined(__ORBIS__)
#pragma message("defined(__ORBIS__)")
#elif defined(__PROSPERO__)
#pragma message("defined(__PROSPERO__)")
#else
#error "Unsupported platform"
#endif

#pragma message("__cplusplus " xstr(__cplusplus))
