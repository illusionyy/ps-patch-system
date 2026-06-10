#pragma once

#include <stdint.h>
#include <string>
#include <vector>

uint8_t hexCharVal(char c);
std::vector<uint8_t> split_hex(const char* src);
std::vector<std::string> split_ver(const char* s, const int delim = ',');
