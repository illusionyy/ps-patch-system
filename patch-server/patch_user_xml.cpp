#include "../shared/memory.h"
#include "../shared/memory.hpp"

#include "patch_user_xml.hpp"
#include "patch_web_xml.hpp"
#include "../shared/stringid.hpp"
#include "../shared/notify.h"
#include "../shared/proc_rw.h"
#include <stdio.h>
#include <pugixml.hpp>

#include <string>
#include <vector>
#include <stdexcept>
#include <cctype>
#include <sstream>

#include <string.h>

constexpr uintptr_t NO_ASLR_ADDR = 0x00400000;

static uintptr_t parse_addr(const char* s)
{
    return std::stoull(s, nullptr, 0);
}

static intptr_t parse_offset(const char* s)
{
    if (!s || s[0] == '\0')
    {
        return 0;
    }
    return std::stoll(s, nullptr, 0);
}

static std::string unescape_utf8(const char* src)
{
    std::string out;
    const char* p = src;
    while (*p)
    {
        if (*p == '\\')
        {
            ++p;
            switch (*p)
            {
                case 'n':
                    out += '\n';
                    break;
                case 't':
                    out += '\t';
                    break;
                case 'r':
                    out += '\r';
                    break;
                case '0':
                    out += '\0';
                    break;
                case '\\':
                    out += '\\';
                    break;
                case 'x':
                {
                    char hi = *++p, lo = *++p;
                    out += (char)((hexCharVal(hi) << 4) | hexCharVal(lo));
                    break;
                }
                default:
                    out += *p;
                    break;
            }
        }
        else
        {
            out += *p;
        }
        ++p;
    }
    return out;
}

static std::wstring unescape_utf16(const char* src)
{
    std::wstring out;
    const char* p = src;
    while (*p)
    {
        if (*p == '\\')
        {
            ++p;
            switch (*p)
            {
                case 'n':
                    out += L'\n';
                    break;
                case 't':
                    out += L'\t';
                    break;
                case 'r':
                    out += L'\r';
                    break;
                case '0':
                    out += L'\0';
                    break;
                case '\\':
                    out += L'\\';
                    break;
                case 'x':
                {
                    char hi = *++p, lo = *++p;
                    out += (wchar_t)((hexCharVal(hi) << 4) | hexCharVal(lo));
                    break;
                }
                default:
                    out += (wchar_t)(unsigned char)*p;
                    break;
            }
        }
        else
        {
            out += (wchar_t)(unsigned char)*p;
        }
        ++p;
    }
    return out;
}

static uintptr_t check_mapbase(const uintptr_t mapbase)
{
    return !mapbase ? NO_ASLR_ADDR : mapbase;
}

static uintptr_t scan_pattern(int pid, const dynlib_info& info, const char* pattern, size_t offset = 0)
{
    return pid_chunk_scan(pid, check_mapbase(info.obj.mapbase), info.obj.mapsize, pattern, offset);
}

static uintptr_t scan_bytes(int pid, const dynlib_info& info, const std::vector<uint8_t>& target)
{
    return pid_chunk_scan_data(pid, check_mapbase(info.obj.mapbase), info.obj.mapsize, target.data(), target.size(), 0);
}

static uintptr_t resolve_addr(const char* s, const dynlib_info& info, const char* imagebase)
{
    const uintptr_t addr = parse_addr(s);
    const uintptr_t base = imagebase && imagebase[0] != '\0'
                               ? parse_addr(imagebase)
                               : NO_ASLR_ADDR;
    const uintptr_t res = check_mapbase(info.obj.mapbase) + (addr - base);
    return res;
}

void patch_xml_context::apply_patch(const patch_line& pline)
{
    const int pid = read_client.clientPid;

    switch (sid(pline.type))
    {
        case sid("byte"):
        {
            const uintptr_t addr = resolve_addr(pline.address, info, pline.imagebase);
            const uint8_t v = (uint8_t)std::stoull(pline.value, nullptr, 0);
            userland_copyin2(pid, addr, &v, sizeof(v));
            break;
        }
        case sid("bytes16"):
        {
            const uintptr_t addr = resolve_addr(pline.address, info, pline.imagebase);
            const uint16_t v = (uint16_t)std::stoull(pline.value, nullptr, 0);
            userland_copyin2(pid, addr, &v, sizeof(v));
            break;
        }
        case sid("bytes32"):
        {
            const uintptr_t addr = resolve_addr(pline.address, info, pline.imagebase);
            const uint32_t v = (uint32_t)std::stoull(pline.value, nullptr, 0);
            userland_copyin2(pid, addr, &v, sizeof(v));
            break;
        }
        case sid("bytes64"):
        {
            const uintptr_t addr = resolve_addr(pline.address, info, pline.imagebase);
            const int64_t v = std::stoll(pline.value, nullptr, 0);
            userland_copyin2(pid, addr, &v, sizeof(v));
            break;
        }

        case sid("float32"):
        {
            const uintptr_t addr = resolve_addr(pline.address, info, pline.imagebase);
            const float val = std::stof(pline.value);
            userland_copyin2(pid, addr, &val, sizeof(val));
            break;
        }
        case sid("float64"):
        {
            const uintptr_t addr = resolve_addr(pline.address, info, pline.imagebase);
            const double val = std::stod(pline.value);
            userland_copyin2(pid, addr, &val, sizeof(val));
            break;
        }

        case sid("utf8"):
        {
            const uintptr_t addr = resolve_addr(pline.address, info, pline.imagebase);
            const std::string s = unescape_utf8(pline.value);
            userland_copyin2(pid, addr, s.data(), s.size());
            break;
        }
        case sid("utf16"):
        {
            const uintptr_t addr = resolve_addr(pline.address, info, pline.imagebase);
            const std::wstring ws = unescape_utf16(pline.value);
            userland_copyin2(pid, addr, ws.data(), ws.size() * sizeof(wchar_t));
            break;
        }

        case sid("bytes"):
        {
            const uintptr_t addr = resolve_addr(pline.address, info, pline.imagebase);
            const auto payload = split_hex(pline.value);
            userland_copyin2(pid, addr, payload.data(), payload.size());
            break;
        }
        case sid("mask"):
        {
            const intptr_t offset = parse_offset(pline.address_offset);
            const uintptr_t found = scan_pattern(pid, info, pline.address, offset > 0 ? offset : 0);
            if (!found)
            {
                throw std::runtime_error(std::string(FILE_FUNC_LINE ": Pattern not found: ") + pline.address);
            }

            const uintptr_t patch_addr = found + (offset < 0 ? offset : 0);
            const auto payload = split_hex(pline.value);
            userland_copyin2(pid, patch_addr, payload.data(), payload.size());
            break;
        }
        case sid("mask_jump32"):
        {
            const intptr_t offset32 = parse_offset(pline.address_offset);
            const uintptr_t found = scan_pattern(pid, info, pline.address, offset32 > 0 ? offset32 : 0);
            if (!found)
            {
                throw std::runtime_error(std::string(FILE_FUNC_LINE ": Pattern not found: ") + pline.address);
            }

            const size_t jump_size = pline.pad_jump_size ? (size_t)std::stoul(pline.pad_jump_size) : 5;
            if (jump_size < 5)
            {
                throw std::invalid_argument(FILE_FUNC_LINE ": mask_jump32 Size must be >= 5");
            }

            const uintptr_t patch_addr = found + (offset32 < 0 ? offset32 : 0);
            const auto target_bytes = split_hex(pline.cave_jump_target);
            const uintptr_t cave_addr = scan_bytes(pid, info, target_bytes);
            if (!cave_addr)
            {
                throw std::runtime_error(FILE_FUNC_LINE ": mask_jump32 Target not found");
            }

            const auto cave_payload = split_hex(pline.value);
            const uintptr_t cave_end = cave_addr + cave_payload.size();
            userland_copyin2(pid, cave_addr, cave_payload.data(), cave_payload.size());

            {
                uint8_t jmp[5];
                const int32_t rel = (int32_t)((intptr_t)(patch_addr + jump_size) - (intptr_t)(cave_end + 5));
                jmp[0] = 0xE9;
                std::memcpy(&jmp[1], &rel, 4);
                userland_copyin2(pid, cave_end, jmp, sizeof(jmp));
            }

            {
                std::vector<uint8_t> jmp_in(jump_size, 0x90);
                const int32_t rel = (int32_t)((intptr_t)cave_addr - (intptr_t)(patch_addr + 5));
                jmp_in[0] = 0xE9;
                std::memcpy(&jmp_in[1], &rel, 4);
                userland_copyin2(pid, patch_addr, jmp_in.data(), jmp_in.size());
            }
            break;
        }
        default:
        {
            throw std::invalid_argument(std::string(FILE_FUNC_LINE ": Unknown patch type: ") + pline.type);
        }
    }
}

static const char* folder_for_title_id(const char* title_id)
{
    switch (sid_len(title_id, _countof_1("xxxx")))
    {
        case sid_startswith("CUSA"):
        case sid_startswith("CUSB"):
        {
            return XML_PS4_PATH;
        }
        case sid_startswith("PPSA"):
        case sid_startswith("PPSB"):
        {
            return XML_PS5_PATH;
        }
        case sid_startswith("NPXS"):
        {
            return XML_SYSTEM_PATH;
        }
        default:
        {
            break;
        }
    }
    return 0;
}

static size_t count_num(const char* s)
{
    size_t count = 0;
    while (*s)
    {
        if (*s++ == '.')
        {
            count++;
        }
    }
    return count;
}

static uint32_t ver_to_num(const char* v)
{
    int a = 0, b = 0, c = 0;
    const size_t dots = count_num(v);

    if (dots == 1)
    {
        sscanf(v, "%d.%d", &a, &b);
        return ((a / 10) << 12) | ((a % 10) << 8) |
               ((b / 10) << 4) | ((b % 10));
    }
    else if (dots == 3)
    {
        sscanf(v, "%d.%d.%d", &a, &b, &c);
        return ((a / 10) << 20) | ((a % 10) << 16) |
               ((b / 10) << 12) | ((b % 10) << 8) |
               ((c / 100) << 4) | ((c / 10 % 10) << 4) | (c % 10);
    }
    return UINT32_MAX;
}

static const char* ver_high(const char* v1, const char* v2)
{
    const uint32_t ver1 = ver_to_num(v1);
    const uint32_t ver2 = ver_to_num(v2);

    if (ver1 != UINT32_MAX && ver2 != UINT32_MAX)
    {
        printf("v1: 0x%08x (%s)\n", ver1, v1);
        printf("v2: 0x%08x (%s)\n", ver2, v2);
        const char* d = (ver1 >= ver2) ? v1 : v2;
        printf("decided: %s\n", d);
        return d;
    }
    return v1;
}

int patch_xml_context::read_xml()
{
    // dump_dynlib_obj(&info.obj);

    pugi::xml_document doc;

    char path[MAX_PATH + 1] = {};
    const char* fn_base = folder_for_title_id(metadata.title_id);
    if (!fn_base)
    {
        printf("unrecognized title id %s\n", metadata.title_id);
        return 0;
    }
    snprintf(path, _countof_1(path), USER_PATCH_PATH "/%s/%s.xml", fn_base, metadata.title_id);
    printf("path %s\n", path);
    pugi::xml_parse_result result = doc.load_file(path);
    if (!result)
    {
        printf("Failed to parse XML: %s\n", result.description());
        return 1;
    }

    pugi::xml_node patch = doc.child("Patch");

    const bool printStats = false;
    // const auto hash_titleid = sid(metadata.title_id);
    const char* decided_ver = ver_high(metadata.app_version, metadata.main_version);
    const auto hash_appver = sid(decided_ver);
    struct patch_info
    {
        size_t patch_applied;
        size_t line_applied;
    };

    patch_info p = {};
    diag = apply_patch_diagnostic_enabled();

    for (pugi::xml_node meta : patch.children("Metadata"))
    {
        patch_attr pattr;
        pattr.title = meta.attribute("Title").as_string();
        pattr.name = meta.attribute("Name").as_string();
        pattr.note = meta.attribute("Note").as_string();
        pattr.author = meta.attribute("Author").as_string();
        pattr.patchVer = meta.attribute("PatchVer").as_string();
        pattr.appVer = meta.attribute("AppVer").as_string();
        pattr.appElf = meta.attribute("AppElf").as_string();
        pattr.ImageBase = meta.attribute("ImageBase").as_string();

        // check app ver
        bool matched = false;
        std::string matched_ver = "unknown";
        {
            auto app_ver_list = split_ver(pattr.appVer);
            for (const auto& p : app_ver_list)
            {
                const auto hash_ver = sid(p.c_str());
                constexpr auto hash_mask = sid("mask");
                if (!matched && (hash_ver == hash_appver || hash_ver == hash_mask))
                {
                    matched = true;
                    matched_ver = p;
                    break;
                }
            }

            char xml_fname[MAX_PATH + 1] = {};
            snprintf(xml_fname, _countof_1(xml_fname), "%s.xml", metadata.title_id);
            PatchEntry e = {};
            e.subdir = fn_base;
            e.file = xml_fname;
            e.attr.name = pattr.name;
            e.attr.title = pattr.title;
            e.singleAppVer = matched_ver.c_str();
            e.attr.appElf = pattr.appElf;
            std::string hash = patch_hash_str(e);
            const size_t info_len = strlen(info.name);
            const size_t elf_len = strlen(pattr.appElf);
            // apparently thread name is only 31 chars
            // so only check based on shortest buf length uwu
            const size_t check_len = elf_len >= info_len ? info_len : elf_len;
            const bool elf_match = sid_len(info.name, check_len) == sid_len(pattr.appElf, check_len);
            const bool enabled = patch_hash_enabled(USER_PATCH_PATH, fn_base, hash.c_str());
            notify_cond(diag,
                        "info.name %s pattr.appElf %s elf_match %d\n"
                        "hash %s enabled %d matched %d\n",
                        info.name,
                        pattr.appElf,
                        elf_match,
                        hash.c_str(),
                        enabled,
                        matched);
            notify_cond(diag,
                        "title_id %s\n"
                        "decided_ver %s matched_ver %s\n",
                        metadata.title_id,
                        decided_ver,
                        matched_ver.c_str());
            printf("info_len %ld elf_len %ld check_len %ld\n", info_len, elf_len, check_len);
            if (!elf_match || !matched || !enabled)
            {
                continue;
            }
        }

        if (printStats)
        {
            printf("Title:    %s\n", pattr.title);
            printf("Name:     %s\n", pattr.name);
            printf("Note:     %s\n", pattr.note);
            printf("Author:   %s\n", pattr.author);
            printf("PatchVer: %s\n", pattr.patchVer);
            printf("AppVer:   %s\n", pattr.appVer);
            printf("AppElf:   %s\n", pattr.appElf);
            printf("ImageBase:   %s\n", pattr.ImageBase);
            printf("  PatchList:\n");
        }
        p.patch_applied += 1;

        for (pugi::xml_node line : meta.child("PatchList").children("Line"))
        {
            patch_line pline;
            pline.imagebase = pattr.ImageBase;
            pline.type = line.attribute("Type").as_string();
            pline.address = line.attribute("Address").as_string();
            pline.value = line.attribute("Value").as_string();
            // masks
            pline.address_offset = line.attribute("Offset").as_string();
            // jump32
            pline.pad_jump_size = line.attribute("Size").as_string();
            pline.cave_jump_target = line.attribute("Target").as_string();
            if (printStats)
            {
                printf("Type: %s\n", pline.type);
                printf("Address: %s\n", pline.address);
                printf("Value: %s\n", pline.value);
                printf("Offset: %s\n", pline.address_offset);
                printf("Size: %s\n", pline.pad_jump_size);
                printf("Target: %s\n", pline.cave_jump_target);
            }
            try
            {
                apply_patch(pline);
                p.line_applied += 1;
            }
            catch (const std::exception& e)
            {
                notify_static(e.what());
                continue;
            }
        }

        if (printStats)
        {
            printf("\n");
        }
    }
    if (p.patch_applied && p.line_applied)
    {
        notify(ELF_NAME
               ":\n"
               "%ld %s Applied\n"
               "%ld %s Applied",
               p.patch_applied,
               (p.patch_applied == 1) ? "Patch" : "Patches",
               p.line_applied,
               (p.line_applied == 1) ? "Patch Line" : "Patch Lines");
    }

    return 0;
}

void patch_xml_context::run_user_patch()
{
}
