#pragma once

#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/cdefs.h>

__BEGIN_DECLS
uintptr_t PatternScan(const void* module_base, const uint64_t module_size, const char* signature, const uint64_t offset);
uintptr_t pid_chunk_scan(const int pid, const uintptr_t mem_start, const uintptr_t mem_sz, const char* pattern, const size_t pattern_offset);
uintptr_t pid_chunk_scan_data(const int pid, const uintptr_t mem_start, const uintptr_t mem_sz, const void* data, const size_t data_sz, const size_t pattern_offset);
void hex_dump(const void* data, const size_t size, const uintptr_t real);
int32_t pid_write_call_(const pid_t pid, const uintptr_t addr, const uintptr_t dest, const bool jmp, const char* user);
int32_t pid_write_call2_(const pid_t pid, const uintptr_t addr, const uintptr_t dest, const bool jmp, const char* user);
uintptr_t pid_read_call_(const pid_t pid, const uintptr_t addr, const char* user);
void gen_backup_path(const char* file, const char* pattn, const size_t pattn_offset, char* buf, const size_t len);
int get_firmware_version_from_disk(uint32_t* out);
int write_backup_to_disk_(const char* path, const void* data, const size_t len, const char* user);
int read_backup_from_disk_(const char* path, void* data, const size_t len, const char* user);
int restore_cached_backup_(const pid_t pid, const uintptr_t mapbase, const char* patch_buf, void* backup, const size_t backup_size, const size_t backup_bytes_size, uintptr_t* out_addr, const bool is_ptr, const char* user);
__END_DECLS

#define pid_write_call(p, a, d, j) pid_write_call_(p, a, d, j, FILE_FUNC_LINE)
#define pid_write_call2(p, a, d, j) pid_write_call2_(p, a, d, j, FILE_FUNC_LINE)
#define pid_read_call(p, a) pid_read_call_(p, a, FILE_FUNC_LINE)
#define write_backup_to_disk(p, b, l) write_backup_to_disk_(p, b, l, FILE_FUNC_LINE)
#define read_backup_from_disk(p, b, l) read_backup_from_disk_(p, b, l, FILE_FUNC_LINE)
#define restore_cached_backup(p, m, f, b, l, bl, o, ip) restore_cached_backup_(p, m, f, b, l, bl, o, ip, FILE_FUNC_LINE)

#define make_backup_buf(d, n) \
    struct d##_               \
    {                         \
        uintptr_t addr;       \
        uint8_t bytes[n];     \
    } d = {}
