#pragma once

#include "stringid.hpp"

__attribute__((noinline)) static size_t ConstStrLen(const char* str)
{
    size_t len = 0;
    while (str[len])
    {
        len++;
    }
    return len;
}

// runtime function to strstr by sid
__attribute__((noinline)) static const char*
StrStrSid(const char* haystackStr, const StringId needleSid, const size_t needleLen)
{
    const size_t hlen = ConstStrLen(haystackStr);
    const size_t nlen = needleLen;

    if (nlen > hlen)
    {
        return 0;
    }

    for (size_t i = 0; i <= hlen - nlen; i++)
    {
        const char* r = haystackStr + i;
        if (stringid_l(r, nlen) == needleSid)
        {
            return r;
        }
    }

    return 0;
}

// yes, this uses a lambda, but can't be avoided it seems
// compile time sid of needle
#define ConstStrStrId(haystack, needle)                                            \
    StrStrSid((haystack),                                                          \
              []() {                                                               \
                  constexpr StringId Sid = stringid_l(needle, _countof_1(needle)); \
                  return Sid;                                                      \
              }(),                                                                 \
              _countof_1(needle))
