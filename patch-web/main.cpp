#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include <algorithm>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "pugixml.hpp"

#include "../shared/macro.h"
#include "../shared/file.h"
#include "../shared/stringid.hpp"

#include "../patch-server/patch_web_xml.hpp"

#include "../shared/patch-common.h"

#include <signal.h>
#include <unistd.h>
#include <sys/sysctl.h>
#include <sys/user.h>
#include <sys/syscall.h>

#include "../shared/notify.h"
#include "../shared/debug.h"
#include "../shared/memory.hpp"

#include "../shared/server_data.h"
#include "../shared/proc.h"

#include <generated/materialize_min_css.inc.h>
#include <generated/materialize_min_js.inc.h>

#include <generated/roboto_v51_latin_regular_woff2.inc.h>
#include <generated/roboto_v51_latin_500_woff2.inc.h>
#include <generated/roboto_mono_v31_latin_regular_woff2.inc.h>

static const std::map<std::string, std::string> g_friendly_names = {
    {XML_PS4_PATH, "PS4"},
    {XML_PS5_PATH, "PS5"},
    {XML_SYSTEM_PATH, "System"},
};

static const char SETTINGS_TAB[] = "__settings__";
static const constexpr auto xml_hash = sid(".xml");

static std::string friendly_tab_name(const std::string& dirname)
{
    auto it = g_friendly_names.find(dirname);
    return it != g_friendly_names.end() ? it->second : dirname;
}

static fs::path settings_path_for(const fs::path& subdir)
{
    return subdir / XML_SETTINGS_FILE;
}

struct ParsedFile
{
    pugi::xml_document doc;
    std::vector<PatchEntry> entries;
};

static ParsedFile parse_xml(const fs::path& path, const std::string& subdir)
{
    ParsedFile pf;
    if (!pf.doc.load_file(path.string().c_str()))
    {
        return pf;
    }

    for (auto meta : pf.doc.child("Patch").children("Metadata"))
    {
        PatchEntry tmpl;
        tmpl.subdir = subdir;
        tmpl.file = path.filename().string();
        tmpl.attr.title = meta.attribute("Title").as_string();
        tmpl.attr.name = meta.attribute("Name").as_string();
        tmpl.attr.note = meta.attribute("Note").as_string();
        tmpl.attr.author = meta.attribute("Author").as_string();
        tmpl.attr.patchVer = meta.attribute("PatchVer").as_string();
        tmpl.attr.appVer = meta.attribute("AppVer").as_string();
        tmpl.attr.appElf = meta.attribute("AppElf").as_string();
        tmpl.attr.ImageBase = meta.attribute("ImageBase").as_string();

        std::vector<patch_line> lines;
        for (auto line : meta.child("PatchList").children("Line"))
        {
            patch_line pl{};
            pl.type = line.attribute("Type").as_string();
            pl.address = line.attribute("Address").as_string();
            pl.value = line.attribute("Value").as_string();
            pl.address_offset = line.attribute("Offset").as_string();
            pl.pad_jump_size = line.attribute("Size").as_string();
            pl.cave_jump_target = line.attribute("Target").as_string();
            lines.push_back(pl);
        }

        auto versions = split_ver(tmpl.attr.appVer);
        if (versions.empty())
        {
            versions.push_back("");
        }

        for (const auto& ver : versions)
        {
            PatchEntry e = tmpl;
            e.singleAppVer = ver;
            e.lines = lines;
            e.hash = patch_hash_str(e);
            pf.entries.push_back(std::move(e));
        }
    }
    return pf;
}

static std::string he(const std::string& s)
{
    std::string o;
    for (unsigned char c : s)
    {
        if (c > 127)
        {
            o += '?';
            continue;
        }
        switch (c)
        {
            case '&':
                o += "&amp;";
                break;
            case '<':
                o += "&lt;";
                break;
            case '>':
                o += "&gt;";
                break;
            case '"':
                o += "&quot;";
                break;
            default:
                o += (char)c;
        }
    }
    return o;
}

static std::string title_id_from_filename(const std::string& fname)
{
    auto dot = fname.rfind('.');
    return dot != std::string::npos ? fname.substr(0, dot) : fname;
}

extern "C"
{
int sceUserServiceInitialize(void*);
int sceUserServiceTerminate(void);
int sceSystemServiceLaunchWebBrowser(const char* uri, void*);
}

struct patch_web_context
{
    std::string patches_root;
    int port{WEB_PORT};

    std::mutex mtx;
    std::map<std::string, bool> state;

    std::atomic<int> active_client{-1};
    std::atomic<int> sse_client{-1};

    enum class SettingType
    {
        Bool,
        Int,
        Float,
        String
    };

    struct SettingEntry
    {
        std::string field;
        std::string name;
        std::string description;
        SettingType type;
        std::string default_value;
    };
    std::vector<SettingEntry> settings_registry;
    using TitleVerKey = std::pair<std::string, std::string>;

    void add_setting(const std::string& field,
                     const std::string& name,
                     const std::string& description,
                     SettingType type,
                     const std::string& default_value = "")
    {
        settings_registry.push_back({field, name, description, type, default_value});
    }

    fs::path global_settings_path() const
    {
        return fs::path(patches_root) / XML_SETTINGS_FILE;
    }

    void load_all_settings()
    {
        state.clear();
        for (auto& de : fs::directory_iterator(patches_root))
        {
            if (!de.is_directory())
            {
                continue;
            }
            fs::path sp = settings_path_for(de.path());
            pugi::xml_document doc;
            if (!doc.load_file(sp.string().c_str()))
            {
                continue;
            }
            for (auto node : doc.child("Settings").children("Patch"))
            {
                state[node.attribute("hash").as_string()] = node.attribute("enabled").as_bool();
            }
        }
    }

    void save_settings_for(const fs::path& subdir,
                           const std::vector<std::string>& hashes)
    {
        pugi::xml_document doc;
        auto root = doc.append_child("Settings");
        for (auto& hash : hashes)
        {
            auto it = state.find(hash);
            if (it == state.end())
            {
                continue;
            }
            auto n = root.append_child("Patch");
            n.append_attribute("hash") = hash.c_str();
            n.append_attribute("enabled") = it->second;
        }
        doc.save_file(settings_path_for(subdir).string().c_str(), "  ");
    }

    std::string get_global_field_raw(const std::string& field,
                                     const std::string& default_value = "") const
    {
        return get_settings_field_raw(global_settings_path(), field, default_value);
    }

    void set_global_field(const std::string& field, const std::string& value)
    {
        fs::path sp = global_settings_path();
        pugi::xml_document doc;
        doc.load_file(sp.string().c_str());

        auto settings = doc.child("Settings");
        if (!settings)
        {
            settings = doc.append_child("Settings");
        }

        auto attr = settings.attribute(field.c_str());
        if (!attr)
        {
            attr = settings.append_attribute(field.c_str());
        }
        attr.set_value(value.c_str());

        doc.save_file(sp.string().c_str(), "  ");
    }

    static std::string http_resp(int code, const char* ctype, const std::string& body)
    {
        const char* status = code == 200   ? "OK"
                             : code == 204 ? "No Content"
                             : code == 302 ? "Found"
                             : code == 503 ? "Service Unavailable"
                                           : "Not Found";
        std::ostringstream r;
        r << "HTTP/1.1 " << code << " " << status << "\r\n"
          << "Content-Type: " << ctype << "\r\n"
          << "Content-Length: " << body.size() << "\r\n"
          << "Connection: close\r\n\r\n"
          << body;
        return r.str();
    }

    static std::string url_decode(const std::string& s)
    {
        std::string out;
        out.reserve(s.size());
        for (size_t i = 0; i < s.size(); ++i)
        {
            if (s[i] == '+')
            {
                out += ' ';
            }
            else if (s[i] == '%' && i + 2 < s.size())
            {
                char hex[3] = {s[i + 1], s[i + 2], '\0'};
                char* end = nullptr;
                long val = std::strtol(hex, &end, 16);
                if (end == hex + 2)
                {
                    out += static_cast<char>(val);
                    i += 2;
                }
                else
                {
                    out += s[i];
                }
            }
            else
            {
                out += s[i];
            }
        }
        return out;
    }

    static std::map<std::string, std::string> parse_qs(const std::string& qs)
    {
        std::map<std::string, std::string> m;
        std::istringstream ss(qs);
        std::string tok;
        while (std::getline(ss, tok, '&'))
        {
            auto eq = tok.find('=');
            if (eq != std::string::npos)
            {
                m[url_decode(tok.substr(0, eq))] = url_decode(tok.substr(eq + 1));
            }
        }
        return m;
    }

    std::vector<fs::path> get_tab_dirs() const
    {
        std::vector<fs::path> dirs;
        for (auto& de : fs::directory_iterator(patches_root))
        {
            if (de.is_directory())
            {
                dirs.push_back(de.path());
            }
        }
        std::sort(dirs.begin(), dirs.end());
        return dirs;
    }

    static std::string common_head(const char* title)
    {
        std::ostringstream o;
        o << "<!DOCTYPE html><html lang='en'><head><meta charset=UTF-8>"
             "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
             "<title>"
          << title << "</title>"
                      "<link rel='stylesheet' href='/materialize.min.css'>"
                      "<style>"
                      "@font-face{font-family:'Roboto';font-weight:400;font-style:normal;"
                      "src:url('/roboto-regular.woff2') format('woff2')}"
                      "@font-face{font-family:'Roboto';font-weight:500;font-style:normal;"
                      "src:url('/roboto-medium.woff2') format('woff2')}"
                      "@font-face{font-family:'Roboto Mono';font-weight:400;font-style:normal;"
                      "src:url('/roboto-mono.woff2') format('woff2')}"
                      ":root{"
                      "--bg:#121212;--surface:#1a1a1a;--surface2:#1e1e1e;--surface3:#263238;"
                      "--border:#333;--border2:#2a2a2a;--txt:#e0e0e0;--txt-dim:#aaa;"
                      "--accent:#64b5f6;--accent2:#90a4ae;"
                      "}"
                      "body{background:var(--bg);color:var(--txt);"
                      "font-family:'Roboto',sans-serif;font-size:14px}"
                      "h1,h2,h3,h4,h5,h6{font-family:'Roboto',sans-serif;color:var(--txt)}"
                      "code,pre,.code-font{font-family:'Roboto Mono',monospace;font-size:13px}"
                      ".card{background:var(--surface);color:var(--txt)}"
                      ".chip{background:var(--surface3);color:var(--txt-dim)}"
                      ".tabs{background:var(--bg) !important;border-bottom:2px solid var(--border)}"
                      ".tabs .tab a{color:var(--txt-dim) !important;font-size:14px}"
                      ".tabs .tab a:hover,.tabs .tab a.active{color:var(--accent) !important}"
                      ".tabs .indicator{background-color:var(--accent) !important}"
                      "[type='checkbox']+span:not(.lever)::before,"
                      "[type='checkbox']:checked+span:not(.lever)::before{border-color:var(--accent)}"
                      "[type='checkbox']:checked+span:not(.lever)::before{background-color:var(--accent)}"
                      ".search-wrap{margin-bottom:16px}"
                      ".search-wrap input{background:var(--surface2);color:var(--txt);"
                      "border:1px solid #444;border-radius:4px;"
                      "padding:6px 10px;width:320px;font-size:14px;font-family:'Roboto',sans-serif}"
                      ".search-wrap input::placeholder{color:#777}"
                      ".file-row{display:flex;gap:16px;padding:8px 10px;"
                      "border-bottom:1px solid var(--border2);align-items:baseline}"
                      ".file-row:hover{background:var(--surface2)}"
                      ".file-row a{font-weight:500;text-decoration:none;color:var(--accent);"
                      "min-width:160px;font-family:'Roboto Mono',monospace}"
                      ".file-row a:hover{text-decoration:underline}"
                      ".file-meta{color:var(--txt-dim);font-size:13px}"
                      ".section-header{background:var(--surface2);border-left:4px solid var(--accent);"
                      "padding:6px 12px;margin:14px 0 6px;font-weight:500;color:var(--txt)}"
                      ".patch-card{background:var(--surface);border:1px solid var(--border);"
                      "border-radius:4px;padding:10px 14px;margin-bottom:8px}"
                      ".patch-card label{cursor:pointer;display:flex;align-items:center;gap:8px}"
                      ".patch-card label b{color:var(--txt);font-weight:500}"
                      ".patch-note{color:var(--txt-dim);font-size:13px;margin-top:4px;font-style:italic}"
                      "details summary{cursor:pointer;color:var(--accent2);font-size:13px;margin-top:6px}"
                      "details summary:hover{color:var(--accent)}"
                      "table{border-collapse:collapse;margin-top:6px;width:100%}"
                      "th{background:var(--surface3);color:#b0bec5;padding:4px 8px;"
                      "text-align:left;font-size:12px;font-weight:500}"
                      "td{padding:4px 8px;border-bottom:1px solid var(--border2);font-size:12px}"
                      "tr:hover td{background:#1e2a30}"
                      ".back-link{display:inline-block;margin-bottom:12px;color:var(--accent);"
                      "text-decoration:none;font-size:14px}"
                      ".back-link:hover{text-decoration:underline}"
                      ".hidden{display:none}"
                      ".js-only{display:none !important}"
                      ".nojs-toggle{display:flex;align-items:center;gap:8px}"
                      "html.js .js-only{display:flex !important}"
                      "html.js .nojs-toggle{display:none !important}"
                      // Settings page
                      ".setting-row{background:var(--surface);border:1px solid var(--border);"
                      "border-radius:4px;padding:12px 16px;margin-bottom:10px;"
                      "display:flex;align-items:center;justify-content:space-between;gap:16px}"
                      ".setting-info{flex:1}"
                      ".setting-name{font-weight:500;color:var(--txt);margin-bottom:2px}"
                      ".setting-desc{color:var(--txt-dim);font-size:12px}"
                      ".setting-control input[type=text],"
                      ".setting-control input[type=number]{"
                      "background:var(--surface2);color:var(--txt);"
                      "border:1px solid #444;border-radius:4px;"
                      "padding:4px 8px;font-size:13px;font-family:'Roboto Mono',monospace;width:180px}"
                      ".setting-save{margin-left:8px;background:var(--accent);color:#000;"
                      "border:none;border-radius:4px;padding:4px 12px;"
                      "font-size:12px;cursor:pointer;font-family:'Roboto',sans-serif}"
                      ".setting-save:hover{opacity:0.85}"
                      "</style>"
                      "<script>document.documentElement.className+=' js';</script>"
                      "</head>"
                      "<body><div class='container' style='padding-top:16px'>\n";
        return o.str();
    }

    static std::string common_foot()
    {
        return "<script src='/materialize.min.js'></script>\n"
               "<script>\n"
               "document.addEventListener('DOMContentLoaded',function(){\n"
               "  var tabElems=document.querySelectorAll('.tabs');\n"
               "  M.Tabs.init(tabElems,{swipeable:false});\n"
               "  var collElems=document.querySelectorAll('.collapsible');\n"
               "  if(collElems.length) M.Collapsible.init(collElems);\n"
               "});\n"
               "</script>\n"
               "</div></body></html>\n";
    }

    std::string build_tab_nav(const std::vector<fs::path>& dirs,
                              const std::string& active_dirname) const
    {
        if (dirs.empty() && active_dirname != SETTINGS_TAB)
        {
            return "";
        }

        std::ostringstream o;
        o << "<div class='row' style='margin-bottom:0'>\n"
             "<div class='col s12'>\n"
             "<ul class='tabs' id='main-tabs'>\n";
        for (auto& dir : dirs)
        {
            std::string d = dir.filename().string();
            bool active = (d == active_dirname);
            o << "<li class='tab'><a href='/" << he(d) << "'"
              << (active ? " class='active'" : "") << ">"
              << he(friendly_tab_name(d)) << "</a></li>\n";
        }
        const bool settings_active = (active_dirname == SETTINGS_TAB);
        o << "<li class='tab'><a href='/settings'"
          << (settings_active ? " class='active'" : "")
          << ">&#9881; Settings</a></li>\n";
        o << "</ul>\n</div>\n</div>\n";
        return o.str();
    }

    std::string build_settings_page(const std::vector<fs::path>& all_dirs) const
    {
        fs::path gsp = global_settings_path();

        std::ostringstream o;
        o << common_head("Patch Manager - Settings");
        o << "<h5 style='margin-bottom:8px'>Patch Manager</h5>\n";
        o << build_tab_nav(all_dirs, SETTINGS_TAB);

        o << "<h6 style='margin:16px 0 10px;color:var(--txt-dim)'>Settings</h6>\n";
        o << "<p style='color:var(--txt-dim);font-size:12px;margin-bottom:14px'>"
             "Stored in <code class='code-font'>"
          << he(gsp.string()) << "</code></p>\n";

        if (settings_registry.empty())
        {
            o << "<p style='color:var(--txt-dim)'>No settings registered.</p>\n";
        }
        else
        {
            for (const auto& se : settings_registry)
            {
                std::string cur_val = get_settings_field_raw(gsp, se.field, se.default_value);

                o << "<div class='setting-row'>\n"
                  << "<div class='setting-info'>\n"
                  << "<div class='setting-name'>" << he(se.name) << "</div>\n";
                if (!se.description.empty())
                {
                    o << "<div class='setting-desc'>" << he(se.description) << "</div>\n";
                }
                o << "<div class='setting-desc' style='margin-top:2px'>"
                     "<code class='code-font' style='font-size:11px'>"
                  << he(se.field) << "</code>"
                                     " &nbsp;["
                  << (se.type == SettingType::Bool    ? "bool"
                      : se.type == SettingType::Int   ? "int"
                      : se.type == SettingType::Float ? "float"
                                                      : "string")
                  << "]"
                     "</div>\n"
                  << "</div>\n"  // .setting-info
                  << "<div class='setting-control'>\n";

                if (se.type == SettingType::Bool)
                {
                    const bool on = (!cur_val.empty() && cur_val != "0" && cur_val != "false");
                    // JS toggle
                    o << "<label class='js-only' style='display:none'>\n"
                      << "<input type='checkbox' class='filled-in'"
                      << " onchange=\"saveSetting('" << he(se.field) << "',this.checked?'1':'0',true)\""
                      << (on ? " checked" : "") << ">\n"
                      << "<span></span>\n</label>\n";
                    // No-JS fallback
                    o << "<div class='nojs-toggle'>\n"
                      << "<a class='waves-effect waves-light btn-small'"
                      << " style='background:" << (on ? "#e57373" : "var(--accent)") << ";color:#000;"
                                                                                        "padding:0 8px;height:24px;line-height:24px;font-size:11px'"
                      << " href='/set_setting?field=" << he(se.field)
                      << "&amp;value=" << (on ? "0" : "1") << "&amp;ret=/settings'>"
                      << (on ? "Disable" : "Enable") << "</a>\n"
                      << "</div>\n";
                }
                else
                {
                    const char* itype = (se.type == SettingType::Int || se.type == SettingType::Float)
                                            ? "number"
                                            : "text";
                    std::string input_id = "si_" + se.field;
                    o << "<div class='js-only' style='display:none'>\n"
                      << "<input id='" << he(input_id) << "' type='" << itype << "' value='" << he(cur_val) << "'"
                      << " onkeydown=\"if(event.key==='Enter')saveSetting('" << he(se.field) << "',this.value,true)\">\n"
                      << "<button class='setting-save'"
                      << " onclick=\"saveSetting('" << he(se.field) << "',document.getElementById('" << he(input_id) << "').value,true)\">"
                      << "Save</button>\n"
                      << "</div>\n";
                    // No-JS fallback
                    o << "<form class='nojs-toggle' method='GET' action='/set_setting'"
                         " style='display:flex;align-items:center;gap:6px'>\n"
                      << "<input type='hidden' name='ret' value='/settings'>\n"
                      << "<input type='hidden' name='field' value='" << he(se.field) << "'>\n"
                      << "<input type='" << itype << "' name='value'"
                                                     " style='background:var(--surface2);color:var(--txt);"
                                                     "border:1px solid #444;border-radius:4px;"
                                                     "padding:4px 8px;font-size:13px;width:160px'"
                                                     " value='"
                      << he(cur_val) << "'>\n"
                      << "<button type='submit' class='setting-save'>Save</button>\n"
                      << "</form>\n";
                }

                o << "</div>\n"   // .setting-control
                  << "</div>\n";  // .setting-row
            }
        }

        o << "<script>\n"
             "function saveSetting(field,value,reload){\n"
             "  fetch('/set_setting?field='+encodeURIComponent(field)+'&value='+encodeURIComponent(value))\n"
             "    .then(function(){\n"
             "      if(reload) location.reload();\n"
             "    })\n"
             "    .catch(function(e){console.error('save setting failed',e)});\n"
             "}\n"
             "</script>\n";

        o << common_foot();
        return o.str();
    }

    std::string build_folder_page(const fs::path& subdir,
                                  const std::vector<fs::path>& all_dirs) const
    {
        std::string dirname = subdir.filename().string();

        struct RowInfo
        {
            std::string fname, route, titles, tid, appVers;
        };
        std::vector<RowInfo> rows;

        for (auto& de : fs::directory_iterator(subdir))
        {
            if (sid(de.path().extension().c_str()) != xml_hash)
            {
                continue;
            }
            if (de.path() == settings_path_for(subdir))
            {
                continue;
            }

            ParsedFile pf = parse_xml(de.path(), dirname);
            if (pf.entries.empty())
            {
                continue;
            }

            std::string fname = de.path().filename().string();
            std::string tid = title_id_from_filename(fname);

            std::vector<std::string> titles, vers;
            std::map<std::string, bool> seen_title, seen_ver;

            for (const auto& e : pf.entries)
            {
                if (e.attr.title[0] && !seen_title[e.attr.title])
                {
                    titles.push_back(e.attr.title);
                    seen_title[e.attr.title] = true;
                }
                if (!e.singleAppVer.empty() && !seen_ver[e.singleAppVer])
                {
                    vers.push_back(e.singleAppVer);
                    seen_ver[e.singleAppVer] = true;
                }
            }

            auto join = [](const std::vector<std::string>& v)
            {
                std::string s;
                for (size_t i = 0; i < v.size(); i++)
                {
                    if (i)
                    {
                        s += ", ";
                    }
                    s += v[i];
                }
                return s;
            };
            rows.push_back({fname, fname, join(titles), tid, join(vers)});
        }

        std::sort(rows.begin(), rows.end(), [](const RowInfo& a, const RowInfo& b)
                  { return a.fname < b.fname; });

        std::ostringstream o;
        o << common_head("Patch Manager");
        o << "<h5 style='margin-bottom:8px'>Patch Manager</h5>\n";
        o << build_tab_nav(all_dirs, dirname);

        o << "<div class='search-wrap js-only' style='display:none'>"
             "<input type='text' id='search' placeholder='Search title, title ID, app version...'"
             " oninput=\"filterRows(this.value)\">"
             "</div>\n";

        if (rows.empty())
        {
            o << "<p style='color:var(--txt-dim)'>No XML files found.</p>\n";
        }
        else
        {
            o << "<div id='file-list'>\n";
            for (const auto& r : rows)
            {
                o << "<div class='file-row' data-search='"
                  << he(r.titles) << " " << he(r.tid) << " " << he(r.appVers) << "'>\n"
                  << "<a href='/" << he(dirname) << "/" << r.route << "'>" << he(r.tid) << "</a>\n"
                  << "<span class='file-meta'>"
                  << he(r.titles.empty() ? "(no title)" : r.titles)
                  << " &nbsp;|&nbsp; "
                  << he(r.appVers.empty() ? "-" : r.appVers)
                  << "</span>\n</div>\n";
            }
            o << "</div>\n";
        }

        o << "<script>\n"
             "function filterRows(q){\n"
             "  q=q.trim().toLowerCase();\n"
             "  document.querySelectorAll('#file-list .file-row').forEach(function(r){\n"
             "    var s=(r.dataset.search||'').toLowerCase();\n"
             "    r.classList.toggle('hidden',q.length>0&&s.indexOf(q)===-1);\n"
             "  });\n"
             "}\n"
             "</script>\n";

        o << common_foot();
        return o.str();
    }

    std::string build_file_page(const fs::path& subdir,
                                const std::string& fname,
                                const std::vector<fs::path>& all_dirs) const
    {
        std::string dirname = subdir.filename().string();
        ParsedFile pf = parse_xml(subdir / fname, dirname);
        std::string tid = title_id_from_filename(fname);

        std::vector<TitleVerKey> order;
        std::map<TitleVerKey, std::vector<const PatchEntry*>> groups;
        for (const auto& e : pf.entries)
        {
            TitleVerKey key{e.attr.title, e.singleAppVer};
            if (groups.find(key) == groups.end())
            {
                order.push_back(key);
            }
            groups[key].push_back(&e);
        }

        std::string page_title = "Patch Manager - " + tid;
        std::ostringstream o;
        o << common_head(page_title.c_str());
        o << "<h5 style='margin-bottom:8px'>Patch Manager</h5>\n";
        o << build_tab_nav(all_dirs, dirname);

        o << "<a class='back-link' href='/" << he(dirname) << "'>"
          << "&larr; " << he(friendly_tab_name(dirname)) << "</a>\n"
          << "<h6 style='color:var(--txt-dim);margin:0 0 12px'>"
          << "<code class='code-font'>" << he(fname) << "</code></h6>\n";

        if (pf.entries.empty())
        {
            o << "<p style='color:var(--txt-dim)'>No patches found.</p>\n";
        }

        auto td_opt = [&](const char* val) -> bool
        {
            if (!val || !val[0])
            {
                return false;
            }
            o << "<td><code class='code-font'>" << he(val) << "</code></td>";
            return true;
        };

        for (const auto& key : order)
        {
            const std::string& title = key.first;
            const std::string& appVer = key.second;
            const auto& patches = groups.at(key);

            o << "<div class='section-header'>"
              << he(title.empty() ? "(no title)" : title)
              << " | <code class='code-font'>" << he(tid) << "</code>";
            if (!appVer.empty())
            {
                o << " | App Ver: <code class='code-font'>" << he(appVer) << "</code>";
            }
            o << "</div>\n";

            for (const auto* ep : patches)
            {
                const PatchEntry& e = *ep;
                int on = state.count(e.hash) ? state.at(e.hash) : 0;

                o << "<div class='patch-card'>\n";

                o << "<label class='js-only' style='display:none'>\n"
                  << "<input type='checkbox' class='filled-in'"
                  << " onchange=\"togglePatch('" << e.hash << "',this.checked)\""
                  << (on ? " checked" : "") << ">\n"
                  << "<span></span>\n"
                  << "<b>" << he(e.attr.name[0] ? e.attr.name : e.attr.title) << "</b>"
                  << " <small style='color:#666'><code class='code-font'>#" << e.hash << "</code></small>\n"
                  << "</label>\n";

                o << "<div class='nojs-toggle'>\n"
                  << "<a class='waves-effect waves-light btn-small'"
                  << " style='background:" << (on ? "#e57373" : "var(--accent)") << ";color:#000;"
                                                                                    "padding:0 8px;height:24px;line-height:24px;font-size:11px;margin-right:8px'"
                  << " href='/toggle?hash=" << e.hash << "&amp;v=" << (on ? "0" : "1")
                  << "&amp;ret=/" << he(dirname) << "/" << he(fname) << "'>"
                  << (on ? "Disable" : "Enable") << "</a>\n"
                  << "<b>" << he(e.attr.name[0] ? e.attr.name : e.attr.title) << "</b>"
                  << " <small style='color:#666'><code class='code-font'>#" << e.hash << "</code></small>\n"
                  << "</div>\n";

                if (e.attr.note[0])
                {
                    o << "<div class='patch-note'>" << he(e.attr.note) << "</div>\n";
                }

                const bool has_author = e.attr.author[0];
                const bool has_patchVer = e.attr.patchVer[0];
                const bool has_appElf = e.attr.appElf[0];
                const bool has_imageBase = e.attr.ImageBase[0];
                const bool has_any_meta = has_author || has_patchVer || has_appElf || has_imageBase;

                o << "<details><summary>Details</summary>\n";

                if (has_any_meta)
                {
                    o << "<table>\n<tr>";
                    if (has_author)
                    {
                        o << "<th>Author</th>";
                    }
                    if (has_patchVer)
                    {
                        o << "<th>PatchVer</th>";
                    }
                    if (has_appElf)
                    {
                        o << "<th>AppElf</th>";
                    }
                    if (has_imageBase)
                    {
                        o << "<th>ImageBase</th>";
                    }
                    o << "</tr>\n<tr>";
                    td_opt(e.attr.author);
                    td_opt(e.attr.patchVer);
                    td_opt(e.attr.appElf);
                    td_opt(e.attr.ImageBase);
                    o << "</tr>\n</table>\n";
                }

                if (!e.lines.empty())
                {
                    bool col_offset = false, col_size = false, col_target = false;
                    for (const auto& l : e.lines)
                    {
                        if (l.address_offset && l.address_offset[0])
                        {
                            col_offset = true;
                        }
                        if (l.pad_jump_size && l.pad_jump_size[0])
                        {
                            col_size = true;
                        }
                        if (l.cave_jump_target && l.cave_jump_target[0])
                        {
                            col_target = true;
                        }
                    }
                    o << "<table style='margin-top:6px'>\n<tr>"
                      << "<th>Type</th><th>Address</th><th>Value</th>";
                    if (col_offset)
                    {
                        o << "<th>Offset</th>";
                    }
                    if (col_size)
                    {
                        o << "<th>Size</th>";
                    }
                    if (col_target)
                    {
                        o << "<th>Target</th>";
                    }
                    o << "</tr>\n";
                    for (const auto& l : e.lines)
                    {
                        o << "<tr>"
                          << "<td><code class='code-font'>" << he(l.type) << "</code></td>"
                          << "<td><code class='code-font'>" << he(l.address) << "</code></td>"
                          << "<td><code class='code-font'>" << he(l.value) << "</code></td>";
                        if (col_offset)
                        {
                            td_opt(l.address_offset);
                        }
                        if (col_size)
                        {
                            td_opt(l.pad_jump_size);
                        }
                        if (col_target)
                        {
                            td_opt(l.cave_jump_target);
                        }
                        o << "</tr>\n";
                    }
                    o << "</table>\n";
                }

                o << "</details>\n</div>\n";
            }
        }

        o << "<script>\n"
             "var __es=new EventSource('/events');\n"
             "__es.onmessage=function(e){if(e.data==='reload')location.reload();};\n"
             "\n"
             "function togglePatch(hash,on){\n"
             "  fetch('/toggle?hash='+hash+'&v='+(on?1:0))\n"
             "    .then(function(){location.reload();})\n"
             "    .catch(function(err){console.error('toggle failed',err)});\n"
             "}\n"
             "</script>\n";

        o << common_foot();
        return o.str();
    }

    static std::string static_resp(const char* ctype,
                                   const void* data,
                                   unsigned long len)
    {
        std::ostringstream r;
        r << "HTTP/1.1 200 OK\r\n"
          << "Content-Type: " << ctype << "\r\n"
          << "Content-Length: " << len << "\r\n"
          << "Cache-Control: max-age=31536000, immutable\r\n"
          << "Connection: close\r\n\r\n";
        std::string resp = r.str();
        resp.append((const char*)data, len);
        return resp;
    }

    void notify_reload()
    {
        int fd = sse_client.load();
        if (fd == -1)
        {
            return;
        }
        const char* msg = "data: reload\n\n";
        write(fd, msg, strlen(msg));
    }

    void handle_client(int cli)
    {
        try
        {
            char buf[32768 + 1] = {};
            read(cli, buf, _countof_1(buf));

            std::string req(buf);
            std::string method, url;
            {
                std::istringstream ss(req.substr(0, req.find("\r\n")));
                ss >> method >> url;
            }

            std::string path, qs;
            auto qpos = url.find('?');
            if (qpos != std::string::npos)
            {
                path = url.substr(0, qpos);
                qs = url.substr(qpos + 1);
            }
            else
            {
                path = url;
            }

            std::string resp;
            const auto path_hash = sid(path.c_str());
            switch (path_hash)
            {
                case sid("/materialize.min.css"):
                {
                    resp = static_resp("text/css", materialize_min_css_data, sizeof(materialize_min_css_data));
                    write(cli, resp.c_str(), resp.size());
                    close(cli);
                    return;
                }
                case sid("/materialize.min.js"):
                {
                    resp = static_resp("application/javascript", materialize_min_js_data, sizeof(materialize_min_js_data));
                    write(cli, resp.c_str(), resp.size());
                    close(cli);
                    return;
                }
                case sid("/roboto-regular.woff2"):
                {
                    resp = static_resp("font/woff2", roboto_v51_latin_regular_woff2_data, sizeof(roboto_v51_latin_regular_woff2_data));
                    write(cli, resp.c_str(), resp.size());
                    close(cli);
                    return;
                }
                case sid("/roboto-medium.woff2"):
                {
                    resp = static_resp("font/woff2", roboto_v51_latin_500_woff2_data, sizeof(roboto_v51_latin_500_woff2_data));
                    write(cli, resp.c_str(), resp.size());
                    close(cli);
                    return;
                }
                case sid("/roboto-mono.woff2"):
                {
                    resp = static_resp("font/woff2", roboto_mono_v31_latin_regular_woff2_data, sizeof(roboto_mono_v31_latin_regular_woff2_data));
                    write(cli, resp.c_str(), resp.size());
                    close(cli);
                    return;
                }
                case sid("/events"):
                {
                    const char* hdr =
                        "HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/event-stream\r\n"
                        "Cache-Control: no-cache\r\n"
                        "Connection: keep-alive\r\n\r\n";
                    write(cli, hdr, strlen(hdr));
                    sse_client.store(cli);
                    char probe[1];
                    while (read(cli, probe, 1) > 0)
                    {
                        usleep(1 * 1000);
                    }
                    sse_client.store(-1);
                    close(cli);
                    return;
                }
                case sid("/toggle"):
                {
                    auto params = parse_qs(qs);
                    auto hi = params.find("hash");
                    auto vi = params.find("v");
                    if (hi != params.end() && vi != params.end())
                    {
                        std::lock_guard<std::mutex> lock(mtx);
                        state[hi->second] = std::stoi(vi->second);

                        for (auto& de : fs::directory_iterator(patches_root))
                        {
                            if (!de.is_directory())
                            {
                                continue;
                            }
                            std::string dirname = de.path().filename().string();
                            std::vector<std::string> hashes;
                            for (auto& xe : fs::directory_iterator(de.path()))
                            {
                                if (sid(xe.path().extension().c_str()) != xml_hash)
                                {
                                    continue;
                                }
                                if (xe.path() == settings_path_for(de.path()))
                                {
                                    continue;
                                }
                                ParsedFile pf = parse_xml(xe.path(), dirname);
                                for (auto& e : pf.entries)
                                {
                                    hashes.push_back(e.hash);
                                }
                            }
                            save_settings_for(de.path(), hashes);
                        }
                    }
                    notify_reload();
                    auto ri = params.find("ret");
                    if (ri != params.end() && !ri->second.empty())
                    {
                        std::ostringstream redir;
                        redir << "HTTP/1.1 302 Found\r\nLocation: " << ri->second
                              << "\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
                        resp = redir.str();
                    }
                    else
                    {
                        resp = http_resp(204, "text/plain", "");
                    }
                    write(cli, resp.c_str(), resp.size());
                    close(cli);
                    return;
                }
                case sid("/set_setting"):
                {
                    auto params = parse_qs(qs);
                    auto fi = params.find("field");
                    auto vi = params.find("value");
                    if (fi != params.end() && vi != params.end())
                    {
                        std::lock_guard<std::mutex> lock(mtx);
                        set_global_field(fi->second, vi->second);
                    }
                    notify_reload();

                    auto ri = params.find("ret");
                    if (ri != params.end() && !ri->second.empty())
                    {
                        std::ostringstream redir;
                        redir << "HTTP/1.1 302 Found\r\nLocation: " << ri->second.c_str()
                              << "\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
                        resp = redir.str();
                    }
                    else
                    {
                        resp = http_resp(204, "text/plain", "");
                    }
                    write(cli, resp.c_str(), resp.size());
                    close(cli);
                    return;
                }
            }

            {
                std::lock_guard<std::mutex> lock(mtx);
                load_all_settings();
            }

            auto dirs = get_tab_dirs();
            switch (path_hash)
            {
                case sid("/"):
                case sid("/index.html"):
                {
                    std::string target = dirs.empty() ? "/settings" : ("/" + dirs.front().filename().string());
                    std::ostringstream redir;
                    redir << "HTTP/1.1 302 Found\r\nLocation: " << target
                          << "\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
                    resp = redir.str();
                    break;
                }
                case sid("/settings"):
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    resp = http_resp(200, "text/html; charset=utf-8", build_settings_page(dirs));
                    write(cli, resp.c_str(), resp.size());
                    close(cli);
                    return;
                }
                default:
                {
                    std::string seg1, seg2;
                    {
                        std::string p = path.substr(1);
                        auto sl = p.find('/');
                        if (sl != std::string::npos)
                        {
                            seg1 = p.substr(0, sl);
                            seg2 = p.substr(sl + 1);
                        }
                        else
                        {
                            seg1 = p;
                        }
                    }

                    fs::path subdir = fs::path(patches_root) / seg1;

                    if (!fs::is_directory(subdir))
                    {
                        resp = http_resp(404, "text/plain", "404 Not Found");
                    }
                    else if (seg2.empty())
                    {
                        std::lock_guard<std::mutex> lock(mtx);
                        resp = http_resp(200, "text/html; charset=utf-8", build_folder_page(subdir, dirs));
                    }
                    else
                    {
                        std::string target_fname;
                        for (auto& de : fs::directory_iterator(subdir))
                        {
                            if (sid(de.path().extension().c_str()) != xml_hash)
                            {
                                continue;
                            }
                            if (de.path() == settings_path_for(subdir))
                            {
                                continue;
                            }
                            std::string fn = de.path().filename().string();
                            if (sid(fn.c_str()) == sid(seg2.c_str()))
                            {
                                target_fname = fn;
                                break;
                            }
                        }

                        if (target_fname.empty())
                        {
                            resp = http_resp(404, "text/plain", "404 Not Found");
                        }
                        else
                        {
                            std::lock_guard<std::mutex> lock(mtx);
                            resp = http_resp(200, "text/html; charset=utf-8", build_file_page(subdir, target_fname, dirs));
                        }
                    }
                    break;
                }
            }
            write(cli, resp.c_str(), resp.size());
        }
        catch (const std::exception& ex)
        {
            notify(FILE_FUNC_LINE "\nexception: %s\n", ex.what());
        }
        catch (...)
        {
            notify(FILE_FUNC_LINE "\nunknown exception\n");
        }
        close(cli);
        active_client.store(-1);
    }

    void add_settings()
    {
        add_setting("apply_patch_diagnostic", "Patch Diagnostic", "Enable verbose notification during patching process", SettingType::Bool, "0");
    }

    void run(const std::string& root, int p)
    {
        patches_root = root;
        port = p;
        add_settings();

        while (true)
        {
            int lfd = 0;
            perror_on_less_zero(lfd = socket(AF_INET, SOCK_STREAM, 0), lfd);
            if (lfd < 0)
            {
                ui_perror("socket", ELF_NAME ": failed to open socket, maybe not valid permission?");
                return;
            }

            int opt = 1, r = 0;
            perror_on_less_zero(r = setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)), r);

            sockaddr_in addr{};
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = INADDR_ANY;

            perror_on_less_zero(r = bind(lfd, (sockaddr*)&addr, sizeof(addr)), r);
            if (r < 0)
            {
                ui_perror("bind", ELF_NAME ": Try starting server again.");
                close(lfd);
                return;
            }

            perror_on_less_zero(r = listen(lfd, 32), r);
            if (r < 0)
            {
                ui_perror("listen", ELF_NAME ": Try starting server again.");
                close(lfd);
                return;
            }

            char url[MAX_PATH + 1] = {};
            snprintf(url, _countof_1(url), "http://127.0.0.1:%d", port);
            notify(ELF_NAME ": serving at %s, xml root %s\n", url, root.c_str());
            static bool shown = false;
            if (!shown)
            {
                sceUserServiceInitialize(0);
                sceSystemServiceLaunchWebBrowser(url, 0);
                shown = true;
            }

            printf("[" ELF_NAME "] waiting for connections on port %d...\n", port);

            while (true)
            {
                int cli = accept(lfd, nullptr, nullptr);
                if (cli < 0)
                {
                    break;
                }
                std::thread([this, cli]
                            { handle_client(cli); })
                    .detach();
            }

            close(lfd);
            sleep(1);
        }
    }
};

static void __attribute__((constructor)) set_thread_name(void)
{
    syscall(SYS_thr_set_name, -1, ELF_NAME);
    puts("set thread name to " ELF_NAME);
}

#if defined(__PROSPERO__)
extern "C" int install_launcher(void);
#endif

static void install_shortcut()
{
#if defined(__PROSPERO__)
    install_launcher();
#endif
}

int main(void)
{
    signal(SIGPIPE, SIG_IGN);
    notify("%s start\nBuilt: " __DATE__ " @ " __TIME__ "\n", ELF_NAME);

    kill_last_pid(ELF_NAME);
    create_default_folders();
    install_shortcut();

    patch_web_context ctx;
    ctx.run(USER_PATCH_PATH, WEB_PORT);
    return 0;
}
