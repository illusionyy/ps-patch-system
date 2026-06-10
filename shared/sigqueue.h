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

#pragma once

#include <signal.h>
#include <sys/cdefs.h>

__BEGIN_DECLS
typedef void(sigqueue_cb_t)(const siginfo_t*);
int sigqueue_init(void);
void sigqueue_fini(void);
int sigqueue_subscribe(int sig);
int sigqueue_consume(sigqueue_cb_t* cb, int timeout_ms);
__END_DECLS
