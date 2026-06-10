#include "patch_reg.hpp"
#include "../shared/macro.h"
#include "../shared/stringid.hpp"

static bool is_pid_alive(const pid_t pid)
{
    int mib[4] = {CTL_KERN, KERN_PROC, KERN_PROC_PROC, 0};
    size_t buf_size = 0;

    if (sysctl(mib, _countof(mib), 0, &buf_size, 0, 0))
    {
        perror("sysctl size");
        return false;
    }

    uint8_t* buf = (uint8_t*)malloc(buf_size);
    if (!buf)
    {
        perror("malloc");
        return false;
    }

    if (sysctl(mib, _countof(mib), buf, &buf_size, 0, 0))
    {
        perror("sysctl query");
        free(buf);
        return false;
    }

    bool alive = false;
    for (uint8_t* ptr = buf; ptr < (buf + buf_size);)
    {
        kinfo_proc* ki = (kinfo_proc*)ptr;
        if (ki->ki_pid == pid)
        {
            alive = true;
            break;
        }
        ptr = ptr + ki->ki_structsize;
    }

    free(buf);
    return alive;
}

void PatchRegistry::remove_stale_pids()
{
    for (auto it = patched.begin(); it != patched.end();)
    {
        if (!is_pid_alive(it->first))
        {
            printf("Removing stale pid %d\n", it->first);
            it = patched.erase(it);
#if defined(__PROSPERO__)
            m_hfr_patched = false;
#endif
        }
        else
        {
            it++;
        }
    }
}

bool PatchRegistry::is_patched(const pid_t pid, const uint32_t module_handle, const uint32_t path_hash)
{
    std::lock_guard<std::mutex> lock(mtx);
    auto it = patched.find(pid);
    if (it == patched.end())
    {
        return false;
    }
    auto it2 = it->second.find(path_hash);
    if (it2 == it->second.end())
    {
        return false;
    }
    return it2->second == module_handle;
}

void PatchRegistry::mark_patched(const pid_t pid, const uint32_t module_handle, const uint32_t path_hash)
{
    std::lock_guard<std::mutex> lock(mtx);
    patched[pid][path_hash] = module_handle;
}

uint32_t PatchRegistry::find_module(const pid_t pid, const uint32_t path_hash)
{
    std::lock_guard<std::mutex> lock(mtx);
    auto it = patched.find(pid);
    if (it == patched.end())
    {
        return 0;
    }
    auto it2 = it->second.find(path_hash);
    if (it2 == it->second.end())
    {
        return 0;
    }
    return it2->second;
}

bool PatchRegistry::clear_if_exists(const pid_t pid)
{
    std::lock_guard<std::mutex> lock(mtx);
    remove_stale_pids();
    auto it = patched.find(pid);
    if (it == patched.end())
    {
        return false;
    }
    patched.erase(it);
    return true;
}
