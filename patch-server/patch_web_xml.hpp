#pragma once

#include "patch_user_xml.hpp"

#include <string>
#include <vector>

#include <pugixml.hpp>
#include "../shared/macro.h"
#include "../shared/stringid.hpp"

#include <filesystem>

namespace fs = std::filesystem;

struct PatchEntry
{
    std::string subdir;
    std::string file;
    std::string hash;
    std::string singleAppVer;
    patch_attr attr;
    std::vector<patch_line> lines;
};

StringId calc_hash(const PatchEntry& e);
std::string patch_hash_str(const PatchEntry& e);
bool patch_hash_enabled(const char* patches_root,
                        const char* folder,
                        const char* hash);
std::string get_settings_field_raw(const fs::path& settings_file,
                                   const std::string& field,
                                   const std::string& default_value = "");
bool get_settings_field_bool(const fs::path& settings_file,
                             const std::string& field,
                             bool default_value = false);
int get_settings_field_int(const fs::path& settings_file,
                           const std::string& field,
                           int default_value = 0);
float get_settings_field_float(const fs::path& settings_file,
                               const std::string& field,
                               float default_value = 0.0f);
std::string get_settings_field_string(const fs::path& settings_file,
                                      const std::string& field,
                                      const std::string& default_value = "");
bool apply_patch_diagnostic_enabled();
bool apply_120hz_enabled();
