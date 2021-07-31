/*-
 * Copyright (c) 2011 Joseph Koshy
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/types.h>

#include "libelftc.h"

#include "_libelftc.h"

ELFTC_VCSID("$Id$");

/*
 * Determine the field name for the timestamp fields inside a 'struct
 * stat'.
 */

#ifdef HAVE_CONFIG_H
#  if HAVE_UTIME
#    define LIBELFTC_HAVE_UTIME	1
#  else
#    define LIBELFTC_HAVE_UTIME	0
#  endif
#  if HAVE_UTIMES \
	&& (HAVE_STRUCT_STAT_ST_ATIM || HAVE_STRUCT_STAT_ST_ATIMESPEC) \
	&& (HAVE_STRUCT_STAT_ST_MTIM || HAVE_STRUCT_STAT_ST_MTIMESPEC)
#    define LIBELFTC_HAVE_UTIMES	1
#    if HAVE_STRUCT_STAT_ST_ATIM
#      define ATIME st_atim
#    elif HAVE_STRUCT_STAT_ST_ATIMESPEC
#      define ATIME st_atimespec
#    endif
#    if HAVE_STRUCT_STAT_ST_MTIM
#      define MTIME st_mtim
#    elif HAVE_STRUCT_STAT_ST_MTIMESPEC
#      define MTIME st_mtimespec
#    endif
#  else
#    define LIBELFTC_HAVE_UTIMES	0
#  endif

#else

#define LIBELFTC_HAVE_UTIME	1

#if	defined(__FreeBSD__) || defined(__NetBSD__)
#define	ATIME	st_atimespec
#define	MTIME	st_mtimespec
#define	LIBELFTC_HAVE_UTIMES	1
#endif

#if	defined(__DragonFly__) || defined(__linux__) || defined(__OpenBSD__)
#define	ATIME	st_atim
#define	MTIME	st_mtim
#define	LIBELFTC_HAVE_UTIMES	1
#endif

#endif /* !HAVE_CONFIG_H */

#if	LIBELFTC_HAVE_UTIMES
#include <sys/time.h>
#elif	LIBELFTC_HAVE_UTIME
#include <utime.h>
#endif

int
elftc_set_timestamps(const char *fn, struct stat *sb)
{
#if	LIBELFTC_HAVE_UTIMES
	/*
	 * The BSD utimes() system call offers timestamps
	 * 1-microsecond granularity.
	 */
	struct timeval tv[2];

	tv[0].tv_sec = sb->ATIME.tv_sec;
	tv[0].tv_usec = sb->ATIME.tv_nsec / 1000;
	tv[1].tv_sec = sb->MTIME.tv_sec;
	tv[1].tv_usec = sb->MTIME.tv_nsec / 1000;

	return (utimes(fn, tv));
#elif	LIBELFTC_HAVE_UTIME
	/*
	 * On OSes without utimes(), fall back to the POSIX utime()
	 * call, which offers 1-second granularity.
	 */
	struct utimbuf utb;

	utb.actime = sb->st_atime;
	utb.modtime = sb->st_mtime;
	return (utime(fn, &utb));
#else
#error	"Need utime or utimes"
#endif
}
