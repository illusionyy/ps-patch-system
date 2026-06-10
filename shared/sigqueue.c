/* Copyright (C) 2026 John Törnblom

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3, or (at your option) any
later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING. If not, see
<http://www.gnu.org/licenses/>.  */

#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <stdatomic.h>
#include <string.h>
#include <unistd.h>

#include "sigqueue.h"

/**
 * Size of the signal queue.
 **/
#define SIG_QUEUE_SIZE 0x1000

/**
 * Use the self-pipe trick with a ring buffer to process signals.
 **/
static siginfo_t sig_queue[SIG_QUEUE_SIZE] = {};
static atomic_int sig_head = 0;
static atomic_int sig_tail = 0;
static int sig_pipe[2] = {-1, -1};

/**
 * Signal handler.
 **/
static void on_signal(int sn, siginfo_t* si, void* ctx)
{
    int head = atomic_load_explicit(&sig_head, memory_order_relaxed);
    int tail = atomic_load_explicit(&sig_tail, memory_order_acquire);
    int next = (head + 1) % SIG_QUEUE_SIZE;

    if (next != tail)
    {
        sig_queue[head] = *si;
        atomic_store_explicit(&sig_head, next, memory_order_release);
    }

    while (write(sig_pipe[1], "x", 1) != 1)
    {
        if (errno == EINTR || errno == EAGAIN)
        {
            continue;
        }
        break;
    }
}

int sigqueue_init(void)
{
    if (pipe(sig_pipe) != 0)
    {
        return -1;
    }

    int flags = fcntl(sig_pipe[0], F_GETFL, 0);
    if (fcntl(sig_pipe[0], F_SETFL, flags | O_NONBLOCK) < 0)
    {
        return -1;
    }

    return 0;
}

void sigqueue_fini(void)
{
    close(sig_pipe[0]);
    close(sig_pipe[1]);
}

int sigqueue_subscribe(int sig)
{
    struct sigaction sa = {
        .sa_sigaction = on_signal,
        .sa_flags = SA_SIGINFO | SA_RESTART};

    sigemptyset(&sa.sa_mask);

    return sigaction(sig, &sa, 0);
}

int sigqueue_consume(sigqueue_cb_t* cb, int timeout_ms)
{
    char buf[SIG_QUEUE_SIZE];
    struct pollfd pfd = {
        .fd = sig_pipe[0],
        .events = POLLIN};
    siginfo_t si;

    if (poll(&pfd, 1, timeout_ms) == -1)
    {
        if (errno == EINTR)
        {
            return 0;
        }
        return -1;
    }

    if (!(pfd.revents & POLLIN))
    {
        return 0;
    }

    if (read(sig_pipe[0], buf, sizeof(buf)) < 0)
    {
        if (errno == EINTR)
        {
            return 0;
        }
        return -1;
    }

    int n = 0;
    size_t tail = 0;
    while (atomic_load(&sig_tail) != atomic_load(&sig_head))
    {
        tail = atomic_load_explicit(&sig_tail, memory_order_relaxed);
        memcpy(&si, &sig_queue[tail], sizeof(si));
        atomic_store_explicit(&sig_tail, (tail + 1) % SIG_QUEUE_SIZE, memory_order_release);
        cb(&si);
        n++;
    }

    return n;
}
