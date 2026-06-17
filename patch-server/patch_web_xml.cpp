#include "patch_user_xml.hpp"
#include "patch_web_xml.hpp"

#define ROOT_XML_PATH USER_PATCH_PATH "/" XML_SETTINGS_FILE

StringId calc_hash(const PatchEntry& e)
{
    StringId h = sid(e.subdir.c_str());
    h ^= sid(e.file.c_str());
    h ^= sid(e.attr.name);
    h ^= sid(e.attr.title);
    h ^= sid(e.attr.appElf);
    h ^= sid(e.singleAppVer.c_str());
    return h;
}

std::string patch_hash_str(const PatchEntry& e)
{
    const auto h = calc_hash(e);
    char buf[16 + 1] = {};
    snprintf(buf, _countof_1(buf), "%08x", h);
    return buf;
}

bool patch_hash_enabled(const char* patches_root,
                        const char* folder,
                        const char* hash)
{
    char path[MAX_PATH + 1] = {};
    snprintf(path, _countof_1(path), "%s/%s/" XML_SETTINGS_FILE, patches_root, folder);

    pugi::xml_document doc;
    if (!doc.load_file(path))
    {
        return false;
    }

    const auto hash_h = sid(hash);
    for (auto node : doc.child("Settings").children("Patch"))
    {
        if (sid(node.attribute("hash").as_string()) == hash_h)
        {
            return node.attribute("enabled").as_bool();
        }
    }

    return false;
}

std::string get_settings_field_raw(const fs::path& settings_file,
                                   const std::string& field,
                                   const std::string& default_value)
{
    pugi::xml_document doc;
    if (!doc.load_file(settings_file.string().c_str()))
    {
        return default_value;
    }
    auto attr = doc.child("Settings").attribute(field.c_str());
    if (!attr)
    {
        return default_value;
    }
    return attr.as_string(default_value.c_str());
}

bool get_settings_field_bool(const fs::path& settings_file,
                             const std::string& field,
                             bool default_value)
{
    pugi::xml_document doc;
    if (!doc.load_file(settings_file.string().c_str()))
    {
        return default_value;
    }
    auto attr = doc.child("Settings").attribute(field.c_str());
    if (!attr)
    {
        return default_value;
    }
    return attr.as_bool();
}

int get_settings_field_int(const fs::path& settings_file,
                           const std::string& field,
                           int default_value)
{
    pugi::xml_document doc;
    if (!doc.load_file(settings_file.string().c_str()))
    {
        return default_value;
    }
    auto attr = doc.child("Settings").attribute(field.c_str());
    if (!attr)
    {
        return default_value;
    }
    return attr.as_int();
}

float get_settings_field_float(const fs::path& settings_file,
                               const std::string& field,
                               float default_value)
{
    pugi::xml_document doc;
    if (!doc.load_file(settings_file.string().c_str()))
    {
        return default_value;
    }
    auto attr = doc.child("Settings").attribute(field.c_str());
    if (!attr)
    {
        return default_value;
    }
    return attr.as_float();
}

std::string get_settings_field_string(const fs::path& settings_file,
                                      const std::string& field,
                                      const std::string& default_value)
{
    return get_settings_field_raw(settings_file, field, default_value);
}

static bool get_root_settings_bool(const char* name)
{
    return get_settings_field_bool(ROOT_XML_PATH, name);
}

bool apply_patch_diagnostic_enabled()
{
    return get_root_settings_bool("apply_patch_diagnostic");
}

bool apply_120hz_enabled()
{
    return get_root_settings_bool("apply_120hz_videoout");
}
