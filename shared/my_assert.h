#pragma once

#include "macro.h"  // FILE_FUNC_LINE
#include <sys/cdefs.h>

typedef struct assert_info
{
    const char* user;
    const char* func;
    const char* file;
    const char* failedexpr;
    const unsigned long line;
} assert_info;

__BEGIN_DECLS
_Noreturn void my_assert_(const assert_info* pInfo, const char* extra_msg);
_Noreturn void my_assertf_(const assert_info* pInfo, const char* fmt, ...);
__END_DECLS

#define my_assert__(h, e, es, ...)                                                            \
    {                                                                                         \
        static const assert_info tmp = {FILE_FUNC_LINE, __FUNCSIG__, __FILE__, es, __LINE__}; \
        ((e) ? (void)0 : h(&tmp, __VA_ARGS__));                                               \
    }

// i think these work?
#define assert_on_false(e) my_assert__(my_assert_, !e, #e, 0)
#define assert_on_true(e) my_assert__(my_assert_, e, #e, 0)
// this will always hit, useful for unreachable code
#define assert_always(e) my_assert__(my_assert_, 0, #e, 0)

// format ver
#define assertf_on_false(e, ...) my_assert__(my_assertf_, !e, #e, __VA_ARGS__)
#define assertf_on_true(e, ...) my_assert__(my_assertf_, e, #e, __VA_ARGS__)
#define assertf_always(e, ...) my_assert__(my_assertf_, 0, #e, __VA_ARGS__)
