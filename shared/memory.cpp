#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "macro.h"

#include <vector>
#include <string>
#include <stdexcept>
#include <cctype>

#include <sstream>

uint8_t hexCharVal(char c)
{
    if (c >= '0' && c <= '9')
    {
        return c - '0';
    }
    if (c >= 'a' && c <= 'f')
    {
        return c - 'a' + 10;
    }
    if (c >= 'A' && c <= 'F')
    {
        return c - 'A' + 10;
    }
    throw std::invalid_argument(std::string(FILE_FUNC_LINE ": Invalid hex character: ") + c);
}

std::vector<uint8_t> split_hex(const char* src)
{
    std::vector<uint8_t> result;
    const std::string s = src;

    size_t i = 0;
    while (i < s.size())
    {
        if (std::isspace((unsigned char)s[i]))
        {
            i += 1;
            continue;
        }

        if (i + 1 >= s.size())
        {
            throw std::invalid_argument(FILE_FUNC_LINE ": Odd number of hex digits");
        }

        result.push_back((hexCharVal(s[i]) << 4) | hexCharVal(s[i + 1]));
        i += 2;
    }

    return result;
}

static size_t pattern_to_bytes(const char* pattern, std::vector<int16_t>& bytes)
{
    const char* current = pattern;
    const char* end = pattern + strlen(pattern);

    while (current < end)
    {
        if (std::isspace((unsigned char)*current))
        {
            ++current;
            continue;
        }

        if (*current == '?')
        {
            bytes.push_back(-1);
            ++current;
            if (current < end && *current == '?')
            {
                ++current;
            }
            continue;
        }

        if (current + 1 >= end)
        {
            throw std::invalid_argument(FILE_FUNC_LINE ": Odd number of hex digits in pattern");
        }

        bytes.push_back((hexCharVal(current[0]) << 4) | hexCharVal(current[1]));
        current += 2;
    }

    return bytes.size();
}

extern "C" uintptr_t PatternScan(const void* module_base, const uint64_t module_size, const char* signature, const uint64_t offset)
{
    if (!module_base || !module_size)
    {
        return 0;
    }

    std::vector<int16_t> pattern;
    const size_t patternLength = pattern_to_bytes(signature, pattern);

    if (!patternLength || patternLength > module_size)
    {
        return 0;
    }

    const uint8_t* scanBytes = (const uint8_t*)module_base;

    for (uint64_t i = 0; i + patternLength <= module_size; ++i)
    {
        bool found = true;
        for (size_t j = 0; j < patternLength; ++j)
        {
            if (pattern[j] != -1 && scanBytes[i + j] != (uint8_t)pattern[j])
            {
                found = false;
                break;
            }
        }
        if (found)
        {
            return (uintptr_t)&scanBytes[i] + offset;
        }
    }

    return 0;
}

std::vector<std::string> split_ver(const char* s, const int delim)
{
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string token;
    const char cdelim = (char)delim;
    while (std::getline(ss, token, cdelim))
    {
        size_t start = token.find_first_not_of(" \t");
        size_t end = token.find_last_not_of(" \t");
        if (start != std::string::npos)
        {
            tokens.push_back(token.substr(start, end - start + 1));
        }
    }
    return tokens;
}
