#pragma once

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include <sys/sysctl.h>
#include <sys/user.h>
#include <sys/syscall.h>
#include <sys/mman.h>

#include <unordered_map>
#include <string>
#include <mutex>

struct PatchRegistry
{
    std::mutex mtx;
    std::unordered_map<pid_t, std::unordered_map<uint32_t, uint32_t>> patched;  // pid -> path_hash -> module_handle
#if defined(__PROSPERO__)
    bool m_hfr_patched = false;
#endif

    void remove_stale_pids();
    bool is_patched(const pid_t pid, const uint32_t module_handle, const uint32_t path_hash);
    void mark_patched(const pid_t pid, const uint32_t module_handle, const uint32_t path_hash);
    uint32_t find_module(const pid_t pid, const uint32_t path_hash);
    bool clear_if_exists(const pid_t pid);
};
