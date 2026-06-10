#pragma once

#if !defined(_countof)
#define _countof(a) (sizeof(a) / sizeof(*a))
#endif

#if !defined(_countof_1)
#define _countof_1(a) (_countof(a) - 1)
#endif

#if !defined(add_1)
#define add_1(n) (n + 1)
#endif

#if !defined(sizeof_1)
#define sizeof_1(n) (sizeof(n) - 1)
#endif

#if !defined(dstr)
#define dstr(s) #s
#endif
#if !defined(xstr)
#define xstr(s) dstr(s)
#endif

#if !defined(FUNC_LINE)
#define FUNC_LINE __FUNCSIG__ ":" xstr(__LINE__)
#endif

#if !defined(FILE_FUNC_LINE)
#define FILE_FUNC_LINE __FILE__ ":" xstr(__LINE__) " " __FUNCSIG__
#endif

#if !defined(FILE_LINE)
#define FILE_LINE __FILE__ ":" xstr(__LINE__)
#endif

#if !defined(FUNC_N)
#define FUNC_N(n) __FUNCTION__ ": " #n
#endif

#if !defined(DATE_TIME)
#define DATE_TIME __DATE__ " @ " __TIME__
#endif

#if !defined(static_size_assert)
#define static_size_assert(x, n) static_assert(x == n, #x " is not " #n "!")
#endif

#if !defined(static_assert_expression)
#define static_assert_expression(x) static_assert(x, #x)
#endif

#if !defined(MAX_PATH)
#define MAX_PATH 260
#endif

#if !defined(PRINT_CHAR_FIELD)
#define PRINT_CHAR_FIELD(pn, s) pn(#s ": %s\n", s)
#endif

#if !defined(snprintf_clear)
#define snprintf_clear(b, s, f, ...) \
    memset(b, 0, sizeof(b));         \
    snprintf(b, s, f, __VA_ARGS__)
#endif
