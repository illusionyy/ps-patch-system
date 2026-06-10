#include <stdio.h>   // printf and snprintf
#include <stdlib.h>  // abort
#include <unistd.h>  // getpid
#include <stdarg.h>
#include "notify.h"
#include "macro.h"
#include "my_assert.h"
#include "proc.h"

constexpr size_t msg_buf = 2048 + 1;

__BEGIN_DECLS
_Noreturn void my_assert_(const assert_info* pInfo, const char* extra_msg)
{
    struct buf
    {
        char program[MAX_PATH + 1];
        char msg[(msg_buf * 2) + sizeof(program)];  // yes, actually need cpp mode for this. nyaa. just to avoid inline memset.
    } buf = {};

    sys_thr_get_name(getpid(), buf.program, sizeof_1(buf.program));
    snprintf(buf.msg, _countof_1(buf.msg), "Assertion failed: (%s), program %s, function %s, file %s, line %ld.%s", pInfo->failedexpr, buf.program, pInfo->func, pInfo->file, pInfo->line, extra_msg ? extra_msg : "");
    printf("%s\n", buf.msg);
    notify_(true, pInfo->user, buf.msg);
    abort();
}

_Noreturn void my_assertf_(const assert_info* pInfo, const char* fmt, ...)
{
    char buf[msg_buf] = {};
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, _countof_1(buf), fmt, args);
    va_end(args);
    my_assert_(pInfo, buf);
}

bool is_true = true;

void is_false_check()
{
    assert_on_false(is_true);
    assertf_on_false(is_true, "is_true %d %d", is_true, 1234);
}

void is_true_check()
{
    assert_on_true(is_true);
    assertf_on_true(is_true, "is_true %d %d", is_true, 1234);
}

void always_check()
{
    assert_always(0 && "this will always hit");
}

void alwaysf_check()
{
    assertf_always(0 && "this will always hit", "testing format %s %d", "nya", 9999);
}

void dummy()
{
    is_true_check();
    is_false_check();
    always_check();
    alwaysf_check();
}
__END_DECLS
