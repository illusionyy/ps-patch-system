#include <cstdint>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

#include <keystone/keystone.h>
#include <Zydis/Zydis.h>

#include <fstream>
#include <string>
#include <stdexcept>

#include "../shared/macro.h"

struct InsnRecord
{
    std::string label;
    std::string mnemonic;
    std::vector<uint8_t> bytes;
    uint64_t addr;
};

struct OffsetEntry
{
    std::string label;
    size_t offset;
};

static void str_replace_all(std::string& s, const std::string& from, const std::string& to)
{
    size_t pos = 0;
    while ((pos = s.find(from, pos)) != std::string::npos)
    {
        s.replace(pos, from.size(), to);
        pos += to.size();
    }
}

static std::vector<std::string> split_lines(const std::string& s)
{
    std::vector<std::string> lines;
    std::istringstream ss(s);
    std::string line;
    while (std::getline(ss, line))
    {
        size_t start = line.find_first_not_of(" \t");
        if (start != std::string::npos)
        {
            lines.push_back(line.substr(start));
        }
        else if (!line.empty())
        {
            lines.push_back(line);
        }
    }
    return lines;
}

static bool is_label(const std::string& line)
{
    return !line.empty() && line.back() == ':';
}

static std::string label_name(const std::string& line)
{
    return line.substr(0, line.size() - 1);
}

static std::string fmt_bytes(const std::vector<uint8_t>& b)
{
    std::ostringstream os;
    os << std::hex << std::setfill('0');
    for (auto byte : b)
    {
        os << "0x" << std::setw(2) << static_cast<int>(byte) << ", ";
    }
    return os.str();
}

static std::string readFile(const std::string& path)
{
    std::ifstream f(path);
    if (!f.is_open())
    {
        throw std::runtime_error("Cannot open file: " + path);
    }
    return std::string((std::istreambuf_iterator<char>(f)),
                       std::istreambuf_iterator<char>());
}

static inline bool is_octal_digit(char c)
{
    return c >= '0' && c <= '7';
}

static inline unsigned int hex_digit_to_int(char c)
{
    unsigned int x = static_cast<unsigned char>(c);
    if (x > '9')
    {
        x += 9;
    }
    return x & 0xf;
}

static inline bool is_hex_digit(char c)
{
    return (c >= '0' && c <= '9') ||
           (c >= 'a' && c <= 'f') ||
           (c >= 'A' && c <= 'F');
}

static size_t encode_utf8(char* dst, char32_t rune)
{
    if (rune <= 0x7F)
    {
        dst[0] = static_cast<char>(rune);
        return 1;
    }
    if (rune <= 0x7FF)
    {
        dst[0] = static_cast<char>(0xC0 | (rune >> 6));
        dst[1] = static_cast<char>(0x80 | (rune & 0x3F));
        return 2;
    }
    if (rune <= 0xFFFF)
    {
        dst[0] = static_cast<char>(0xE0 | (rune >> 12));
        dst[1] = static_cast<char>(0x80 | ((rune >> 6) & 0x3F));
        dst[2] = static_cast<char>(0x80 | (rune & 0x3F));
        return 3;
    }
    /* <= 0x10FFFF */ dst[0] = static_cast<char>(0xF0 | (rune >> 18));
    dst[1] = static_cast<char>(0x80 | ((rune >> 12) & 0x3F));
    dst[2] = static_cast<char>(0x80 | ((rune >> 6) & 0x3F));
    dst[3] = static_cast<char>(0x80 | (rune & 0x3F));
    return 4;
}

// https://github.com/abseil/abseil-cpp/blob/892f5132899db75d75d909285e70128ca6bd14d3/absl/strings/escaping.cc#L93
static std::vector<uint8_t> cunescape_content(const std::string& src,
                                              std::string& error)
{
    std::vector<uint8_t> out;
    out.reserve(src.size() + 1);

    for (size_t p = 0; p < src.size();)
    {
        if (src[p] != '\\')
        {
            out.push_back(static_cast<uint8_t>(src[p++]));
            continue;
        }

        if (++p >= src.size())
        {
            error = "String cannot end with \\";
            return {};
        }

        switch (src[p])
        {
            case 'a':
                out.push_back('\a');
                break;
            case 'b':
                out.push_back('\b');
                break;
            case 'f':
                out.push_back('\f');
                break;
            case 'n':
                out.push_back('\n');
                break;
            case 'r':
                out.push_back('\r');
                break;
            case 't':
                out.push_back('\t');
                break;
            case 'v':
                out.push_back('\v');
                break;
            case '\\':
                out.push_back('\\');
                break;
            case '?':
                out.push_back('\?');
                break;
            case '\'':
                out.push_back('\'');
                break;
            case '"':
                out.push_back('"');
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            {
                unsigned int ch = static_cast<unsigned int>(src[p] - '0');
                if (p + 1 < src.size() && is_octal_digit(src[p + 1]))
                {
                    ch = ch * 8 + static_cast<unsigned int>(src[++p] - '0');
                }
                if (p + 1 < src.size() && is_octal_digit(src[p + 1]))
                {
                    ch = ch * 8 + static_cast<unsigned int>(src[++p] - '0');
                }
                if (ch > 0xFF)
                {
                    error = "Octal escape value exceeds 0xFF";
                    return {};
                }
                out.push_back(static_cast<uint8_t>(ch));
                break;
            }
            case 'x':
            case 'X':
            {
                if (p + 1 >= src.size() || !is_hex_digit(src[p + 1]))
                {
                    error = "\\x must be followed by at least one hex digit";
                    return {};
                }
                unsigned int ch = 0;
                while (p + 1 < src.size() && is_hex_digit(src[p + 1]))
                {
                    ch = (ch << 4) + hex_digit_to_int(src[++p]);
                    if (ch > 0xFF)
                    {
                        error = "\\x value exceeds 0xFF";
                        return {};
                    }
                }
                out.push_back(static_cast<uint8_t>(ch));
                break;
            }
            case 'u':
            {
                if (p + 4 >= src.size())
                {
                    error = "\\u must be followed by exactly 4 hex digits";
                    return {};
                }
                char32_t rune = 0;
                for (int i = 0; i < 4; ++i)
                {
                    if (!is_hex_digit(src[p + 1]))
                    {
                        error = "\\u must be followed by exactly 4 hex digits";
                        return {};
                    }
                    rune = (rune << 4) + hex_digit_to_int(src[++p]);
                }
                if (rune >= 0xD800 && rune <= 0xDFFF)
                {
                    error = "\\u surrogate character (0xD800-0xDFFF) is invalid";
                    return {};
                }
                char buf[4];
                size_t n = encode_utf8(buf, rune);
                for (size_t i = 0; i < n; ++i)
                {
                    out.push_back(static_cast<uint8_t>(buf[i]));
                }
                break;
            }
            case 'U':
            {
                if (p + 8 >= src.size())
                {
                    error = "\\U must be followed by exactly 8 hex digits";
                    return {};
                }
                char32_t rune = 0;
                for (int i = 0; i < 8; ++i)
                {
                    if (!is_hex_digit(src[p + 1]))
                    {
                        error = "\\U must be followed by exactly 8 hex digits";
                        return {};
                    }
                    uint32_t next = (rune << 4) + hex_digit_to_int(src[++p]);
                    if (next > 0x10FFFF)
                    {
                        error = "\\U value exceeds Unicode limit (0x10FFFF)";
                        return {};
                    }
                    rune = next;
                }
                if (rune >= 0xD800 && rune <= 0xDFFF)
                {
                    error = "\\U surrogate character is invalid";
                    return {};
                }
                char buf[4];
                size_t n = encode_utf8(buf, rune);
                for (size_t i = 0; i < n; ++i)
                {
                    out.push_back(static_cast<uint8_t>(buf[i]));
                }
                break;
            }
            default:
            {
                error = std::string("Unknown escape sequence: \\") + src[p];
                return {};
            }
        }
        ++p;
    }

    out.push_back(0);
    return out;
}

static std::vector<uint64_t> parse_int_list(const std::string& line)
{
    std::vector<uint64_t> vals;
    size_t p = line.find_first_of(" \t");
    if (p == std::string::npos)
    {
        return vals;
    }

    std::istringstream ss(line.substr(p));
    std::string tok;
    while (std::getline(ss, tok, ','))
    {
        // trim whitespace
        size_t s = tok.find_first_not_of(" \t");
        size_t e = tok.find_last_not_of(" \t");
        if (s == std::string::npos)
        {
            continue;
        }
        tok = tok.substr(s, e - s + 1);
        try
        {
            vals.push_back(std::stoull(tok, nullptr, 0));
        }
        catch (...)
        { /* ignore malformed tokens */
        }
    }
    return vals;
}

static size_t parse_count_arg(const std::string& line)
{
    size_t p = line.find_first_of(" \t");
    if (p == std::string::npos)
    {
        return 0;
    }
    size_t s = line.find_first_not_of(" \t", p);
    if (s == std::string::npos)
    {
        return 0;
    }

    size_t comma = line.find(',', s);
    std::string tok = (comma == std::string::npos)
                          ? line.substr(s)
                          : line.substr(s, comma - s);
    try
    {
        return static_cast<size_t>(std::stoull(tok, nullptr, 0));
    }
    catch (...)
    {
        return 0;
    }
}

static uint8_t parse_skip_fill(const std::string& line)
{
    size_t comma = line.find(',');
    if (comma == std::string::npos)
    {
        return 0x00;
    }
    size_t s = line.find_first_not_of(" \t", comma + 1);
    if (s == std::string::npos)
    {
        return 0x00;
    }
    try
    {
        return static_cast<uint8_t>(std::stoull(line.substr(s), nullptr, 0));
    }
    catch (...)
    {
        return 0x00;
    }
}

static bool is_zero(const std::string& l)
{
    return l.rfind(".zero", 0) == 0;
}
static bool is_skip(const std::string& l)
{
    return l.rfind(".skip", 0) == 0;
}
static bool is_byte(const std::string& l)
{
    return l.rfind(".byte", 0) == 0;
}
static bool is_word(const std::string& l)
{
    return l.rfind(".word", 0) == 0;
}
static bool is_long(const std::string& l)
{
    return l.rfind(".long", 0) == 0;
}
static bool is_quad(const std::string& l)
{
    return l.rfind(".quad", 0) == 0;
}

static void push_le(std::vector<uint8_t>& out, uint64_t val, size_t nbytes)
{
    for (size_t i = 0; i < nbytes; ++i)
    {
        out.push_back(static_cast<uint8_t>((val >> (8 * i)) & 0xFF));
    }
}

static int build_usercode(std::string& SOURCE)
{
    SOURCE += R"(
    __export_post_prx_load_append_module:
    movabs rax, 0x252
    .byte 0xe9
    .long 0
    int3 # if failed to write, then raise execption nya
    payload_pid:
    movabs rax, -1
    ret
    getpid:
    movabs rax, -1
    jmp rax
    kill:
    movabs rax, -1
    jmp rax
    payload_console_fd:
    movabs rax, -1
    ret
    __sys_rdup:
    movabs rax, -1
    jmp rax
    dup2:
    movabs rax, -1
    jmp rax
    printf:
    movabs rax, -1
    jmp rax
)";
    try
    {
        SOURCE += readFile("user_load_prx_ps4.txt");
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -__LINE__;
    }
    return 0;
}

static int build_shellcore(std::string& SOURCE)
{
    SOURCE += R"(
.zero 5 # pad out original entry point
onNewProcess_original:
movabs rax, -1
jmp rax
sysctl:
movabs rax, -1
jmp rax
malloc:
movabs rax, -1
jmp rax
free:
movabs rax, -1
jmp rax
sceKernelGetAppInfo:
movabs rax, -1
jmp rax
getpid:
movabs rax, -1
jmp rax
kill:
movabs rax, -1
jmp rax
puts:
movabs rax, -1
jmp rax
usleep:
movabs rax, -1
jmp rax
fopen:
movabs rax, -1
jmp rax
fwrite:
movabs rax, -1
jmp rax
fclose:
movabs rax, -1
jmp rax
stat:
movabs rax, -1
jmp rax
memset:
movabs rax, -1
jmp rax
vsnprintf:
movabs rax, -1
jmp rax
printf:
movabs rax, -1
jmp rax
strlen:
movabs rax, -1
jmp rax
sceKernelSendNotificationRequest:
movabs rax, -1
jmp rax
perror:
movabs rax, -1
jmp rax
sceKernelGetProcessName:
movabs rax, -1
jmp rax
fgets:
movabs rax, -1
jmp rax
__export_GetAppInfoHook:
.zero 5
socket:
movabs rax, -1
jmp rax
htons:
movabs rax, -1
jmp rax
connect:
movabs rax, -1
jmp rax
close:
movabs rax, -1
jmp rax
recv:
movabs rax, -1
jmp rax
send:
movabs rax, -1
jmp rax
__inet_pton:
movabs rax, -1
jmp rax
__error:
movabs rax, -1
jmp rax
strerror:
movabs rax, -1
jmp rax
strncpy:
movabs rax, -1
jmp rax
)";
    try
    {
        SOURCE += readFile("shellcore_ps4.txt");
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -__LINE__;
    }
    return 0;
}

static int build_videoout(std::string& SOURCE)
{
    SOURCE += R"(
sceVideoOutOpen:
movabs rax, -1
jmp rax
sceVideoOutSetFlipRate:
movabs rax, -1
jmp rax
sceVideoOutConfigureOutput:
movabs rax, -1
jmp rax
sceVideoOutIsOutputSupported:
movabs rax, -1
jmp rax
sceVideoOutRegisterBuffers:
movabs rax, -1
jmp rax
sceVideodec2CreateDecoder:
movabs rax, -1
jmp rax
sceVideodec2DeleteDecoder:
movabs rax, -1
jmp rax
memset:
movabs rax, -1
jmp rax
vsnprintf:
movabs rax, -1
jmp rax
strlen:
movabs rax, -1
jmp rax
printf:
movabs rax, -1
jmp rax
sceKernelSendNotificationRequest:
movabs rax, -1
jmp rax
strerror:
movabs rax, -1
jmp rax
__error:
movabs rax, -1
jmp rax
strncpy:
movabs rax, -1
jmp rax
)";
    try
    {
        SOURCE += readFile("user_videoout_120hz_ps5.txt");
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -__LINE__;
    }
    return 0;
}

int build_code(std::string& SOURCE, const char* file)
{
    const size_t numspaces = 0;
    constexpr uint64_t OG_ADDR = 0x0;
    constexpr int MOVABS_IMM_OFF = 2;
    constexpr bool PRINT_C_ARRAY = true;

    std::string code = SOURCE;
    str_replace_all(code, "::", "_");
    str_replace_all(code, ".L", "L_");
    str_replace_all(code, ".addrsig", "#.addrsig");
    str_replace_all(code, "@PLT", "");
    str_replace_all(code, "\\000", "\\0");
    {
        std::ofstream out(std::string(file) + ".gen.txt");
        out << code;
    }

    auto lines = split_lines(code);
    lines.erase(std::remove_if(lines.begin(), lines.end(), [](const std::string& l)
                               { return l.empty() || l[0] == '#'; }),
                lines.end());

    ks_engine* ks = nullptr;
    if (ks_open(KS_ARCH_X86, KS_MODE_64, &ks) != KS_ERR_OK)
    {
        std::cerr << "ks_open failed\n";
        return 1;
    }

    unsigned char* full_enc = nullptr;
    size_t full_size = 0, full_stat = 0;

    if (ks_asm(ks, code.c_str(), OG_ADDR, &full_enc, &full_size, &full_stat) != KS_ERR_OK)
    {
        std::cerr << "ks_asm failed: " << ks_strerror(ks_errno(ks)) << "\n";
        ks_close(ks);
        return 1;
    }
    ks_close(ks);

    ZydisDecoder decoder;
    ZydisFormatter formatter;
    ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64);
    ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_ATT);

    std::vector<InsnRecord> records;

    auto is_asciz = [](const std::string& m)
    {
        return m.find(".asciz") == 0;
    };

    std::string cur_label;
    size_t byte_off = 0;

    for (size_t li = 0; li < lines.size(); ++li)
    {
        const std::string& line = lines[li];

        if (is_label(line))
        {
            cur_label = label_name(line);
            continue;
        }

        if (is_asciz(line))
        {
            size_t q1 = line.find('"');
            size_t q2 = line.rfind('"');
            if (q1 == std::string::npos || q1 == q2)
            {
                continue;
            }

            std::string content = line.substr(q1 + 1, q2 - q1 - 1);
            bool is_ascii_nodot = (line.rfind(".ascii", 0) == 0 &&
                                   line.rfind(".asciz", 0) == std::string::npos);

            std::string err;
            std::vector<uint8_t> raw = cunescape_content(content, err);
            if (raw.empty() && !err.empty())
            {
                std::cerr << "WARNING: .asciz/.ascii unescape failed (" << err
                          << ") at line: " << line << "\n";
                continue;
            }
            if (is_ascii_nodot && !raw.empty())
            {
                raw.pop_back();
            }

            size_t n = raw.size();
            if (byte_off + n > full_size)
            {
                std::cerr << "WARNING: .asciz overruns buffer at line: " << line << "\n";
                break;
            }

            InsnRecord r;
            r.label = cur_label;
            r.mnemonic = line;
            r.bytes.assign(full_enc + byte_off, full_enc + byte_off + n);
            r.addr = OG_ADDR + byte_off;
            records.push_back(std::move(r));
            byte_off += n;
            continue;
        }

        if (is_zero(line))
        {
            size_t n = parse_count_arg(line);
            if (n == 0)
            {
                continue;
            }
            if (byte_off + n > full_size)
            {
                std::cerr << "WARNING: .zero overruns buffer at line: " << line << "\n";
                break;
            }
            InsnRecord r;
            r.label = cur_label;
            r.mnemonic = line;
            r.bytes.assign(full_enc + byte_off, full_enc + byte_off + n);
            r.addr = OG_ADDR + byte_off;
            records.push_back(std::move(r));
            byte_off += n;
            continue;
        }

        if (is_skip(line))
        {
            size_t n = parse_count_arg(line);
            uint8_t fill = parse_skip_fill(line);
            if (n == 0)
            {
                continue;
            }
            if (byte_off + n > full_size)
            {
                std::cerr << "WARNING: .skip overruns buffer at line: " << line << "\n";
                break;
            }
            InsnRecord r;
            r.label = cur_label;
            r.mnemonic = line;
            r.bytes.assign(full_enc + byte_off, full_enc + byte_off + n);
            r.addr = OG_ADDR + byte_off;
            for (size_t i = 0; i < n; ++i)
            {
                if (full_enc[byte_off + i] != fill)
                {
                    std::cerr << "WARNING: .skip fill mismatch at offset 0x"
                              << std::hex << (byte_off + i) << "\n";
                    break;
                }
            }
            records.push_back(std::move(r));
            byte_off += n;
            continue;
        }

        if (is_byte(line))
        {
            auto vals = parse_int_list(line);
            size_t n = vals.size();
            if (n == 0)
            {
                continue;
            }
            if (byte_off + n > full_size)
            {
                std::cerr << "WARNING: .byte overruns buffer at line: " << line << "\n";
                break;
            }
            InsnRecord r;
            r.label = cur_label;
            r.mnemonic = line;
            r.bytes.assign(full_enc + byte_off, full_enc + byte_off + n);
            r.addr = OG_ADDR + byte_off;
            records.push_back(std::move(r));
            byte_off += n;
            continue;
        }

        if (is_word(line))
        {
            auto vals = parse_int_list(line);
            size_t n = vals.size() * 2;
            if (n == 0)
            {
                continue;
            }
            if (byte_off + n > full_size)
            {
                std::cerr << "WARNING: .word overruns buffer at line: " << line << "\n";
                break;
            }
            InsnRecord r;
            r.label = cur_label;
            r.mnemonic = line;
            r.bytes.assign(full_enc + byte_off, full_enc + byte_off + n);
            r.addr = OG_ADDR + byte_off;
            records.push_back(std::move(r));
            byte_off += n;
            continue;
        }

        if (is_long(line))
        {
            auto vals = parse_int_list(line);
            size_t n = vals.size() * 4;
            if (n == 0)
            {
                continue;
            }
            if (byte_off + n > full_size)
            {
                std::cerr << "WARNING: .long overruns buffer at line: " << line << "\n";
                break;
            }
            InsnRecord r;
            r.label = cur_label;
            r.mnemonic = line;
            r.bytes.assign(full_enc + byte_off, full_enc + byte_off + n);
            r.addr = OG_ADDR + byte_off;
            records.push_back(std::move(r));
            byte_off += n;
            continue;
        }

        if (is_quad(line))
        {
            auto vals = parse_int_list(line);
            size_t n = vals.size() * 8;
            if (n == 0)
            {
                continue;
            }
            if (byte_off + n > full_size)
            {
                std::cerr << "WARNING: .quad overruns buffer at line: " << line << "\n";
                break;
            }
            InsnRecord r;
            r.label = cur_label;
            r.mnemonic = line;
            r.bytes.assign(full_enc + byte_off, full_enc + byte_off + n);
            r.addr = OG_ADDR + byte_off;
            records.push_back(std::move(r));
            byte_off += n;
            continue;
        }

        if (byte_off >= full_size)
        {
            std::cerr << "WARNING: ran out of bytes at source line: " << line << "\n";
            break;
        }

        ZydisDecodedInstruction insn;
        ZydisDecodedOperand ops[ZYDIS_MAX_OPERAND_COUNT];

        if (!ZYAN_SUCCESS(ZydisDecoderDecodeFull(
                &decoder,
                full_enc + byte_off,
                full_size - byte_off,
                &insn,
                ops)))
        {
            std::cerr << "Zydis decode failed at offset 0x" << std::hex
                      << byte_off << " source: " << line << "\n";
            break;
        }

        InsnRecord r;
        r.label = cur_label;
        r.mnemonic = line;
        r.bytes.assign(full_enc + byte_off, full_enc + byte_off + insn.length);
        r.addr = OG_ADDR + byte_off;
        records.push_back(std::move(r));
        byte_off += insn.length;
    }

    if (byte_off != full_size)
    {
        std::cerr << "WARNING: consumed " << std::dec << byte_off
                  << " bytes but full_enc has " << full_size << "\n";
        return EXIT_FAILURE;
    }

    std::vector<OffsetEntry> offsets;

    for (auto& r : records)
    {
        if (r.label.rfind("__export_", 0) == 0)
        {
            auto exists = std::find_if(offsets.begin(), offsets.end(), [&](const auto& o)
                                       { return o.label == r.label; }) != offsets.end();

            if (!exists)
            {
                offsets.push_back({r.label, static_cast<size_t>(r.addr - OG_ADDR)});
            }
        }
        if (r.bytes.size() != 10)
        {
            continue;
        }

        ZydisDecodedInstruction insn;
        ZydisDecodedOperand ops[ZYDIS_MAX_OPERAND_COUNT];

        if (!ZYAN_SUCCESS(ZydisDecoderDecodeFull(
                &decoder, r.bytes.data(), r.bytes.size(), &insn, ops)))
        {
            continue;
        }

        if (insn.mnemonic != ZYDIS_MNEMONIC_MOV)
        {
            continue;
        }
        if (insn.operand_count < 2)
        {
            continue;
        }
        if (ops[1].type != ZYDIS_OPERAND_TYPE_IMMEDIATE)
        {
            continue;
        }
        if (static_cast<int64_t>(ops[1].imm.value.u) != -1)
        {
            continue;
        }

        if (!r.label.empty() && r.label[0] != 'L' && r.label != "notify_fixed_")
        {
            offsets.push_back({r.label,
                               static_cast<size_t>(r.addr - OG_ADDR) + MOVABS_IMM_OFF});
        }
    }

    std::ofstream out(file);
    if (!out)
    {
        char e[1024 + 1] = {};
        snprintf(e, _countof_1(e), "perror %s", file);
        std::perror(e);
        return 1;
    }

    const std::string indent(numspaces, ' ');

    out << "// clang-format off\n";
    out << std::hex;
    for (auto& e : offsets)
    {
        out << indent << "static const unsigned long " << e.label
            << "_offset = 0x" << e.offset << ";\n";
    }

    if (PRINT_C_ARRAY)
    {
        size_t max_bw = 0;
        for (auto& r : records)
        {
            if (!is_asciz(r.mnemonic))
            {
                max_bw = std::max(max_bw, fmt_bytes(r.bytes).size());
            }
        }

        const size_t label_col = numspaces + 4 + max_bw + 1;

        out << "\n"
            << indent << "static const unsigned char shellcode[] = {\n";

        std::string prev_label;
        size_t num_bytes = 0;

        for (auto& r : records)
        {
            if (r.label != prev_label)
            {
                out << std::string(label_col, ' ') << "// " << r.label << ":\n";
                prev_label = r.label;
            }
            num_bytes += r.bytes.size();
            std::string bs = fmt_bytes(r.bytes);
            if (is_asciz(r.mnemonic))
            {
                out << indent << "    " << bs << " // " << r.mnemonic << "\n";
            }
            else
            {
                out << indent << "    " << std::left << std::setw(max_bw) << bs
                    << " // " << r.mnemonic << "\n";
            }
        }

        out << indent << "};\n";

        if (num_bytes != full_size)
        {
            std::cerr << "WARNING: encoded bytes (" << full_size
                      << ") != output bytes (" << num_bytes << ")\n";
        }
        else
        {
            std::cout << "Byte counts match: " << std::dec << num_bytes << " bytes\n";
        }
    }
    out << "// clang-format on\n";

    ks_free(full_enc);
    std::cout << "Written to " << file << "\n";
    return 0;
}

int main()
{
    {
        std::string SOURCE = "";
        if (build_usercode(SOURCE) || build_code(SOURCE, "../patch-server/usermode_prx_load.h"))
        {
            return -__LINE__;
        }
    }
    {
        std::string SOURCE = "";
        if (build_shellcore(SOURCE) || build_code(SOURCE, "../patch/shellcore_onNewProcess.h"))
        {
            return -__LINE__;
        }
    }
    {
        std::string SOURCE = "";
        if (build_videoout(SOURCE) || build_code(SOURCE, "../patch-server/videoout_120hz.h"))
        {
            return -__LINE__;
        }
    }
    return 0;
}
