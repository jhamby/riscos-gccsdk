/*
 * ANSI Standard 4.1.3: Errors <errno.h>
 * Copyright (c) 1997-2005 Nick Burrett
 * Copyright (c) 2000-2010 UnixLib Developers
 */

/*
 * Copyright (c) 1982, 1986, 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
 * (c) UNIX System Laboratories, Inc.
 * All or some portions of this file are derived from material licensed
 * to the University of California by American Telephone and Telegraph
 * Co. or Unix System Laboratories, Inc. and are reproduced herein with
 * the permission of UNIX System Laboratories, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)errno.h	8.5 (Berkeley) 1/21/94
 */

#ifndef __ERRNO_H
#define __ERRNO_H

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#ifdef __TARGET_SCL__
/* Domain error. An input argument is outside the domain over
   which the mathematical function is defined.  */
#  define EDOM 1

/* Range error. The result of a function cannot be represented
   as a double floating point value.  */
#  define ERANGE 2

/* An unrecognised signal is caught by the default signal handler.  */
#  define ESIGNUM 3
#endif

#define EPERM			1 /* Operation not permitted.  */
#define ENOENT			2 /* No such file or directory.  */
#define ESRCH			3 /* No such process.  */
#define EINTR			4 /* Interrupted function call. */
#define EIO			5 /* Input/output error.  */
#define ENXIO			6 /* No such device or address (device not configured).  */
#define E2BIG			7 /* Argument list too long.  */
#define ENOEXEC			8 /* Invalid executable file format (exec format error). */
#define EBADF			9 /* Bad file descriptor.  */
#define ECHILD			10 /* There are no child processes.  */
#define EDEADLK			11 /* Deadlock situation detected/avoided.  */
#define ENOMEM			12 /* Cannot allocate memory.  */
/* Permission denied.
   File permissions do not allow the attempted operation.  */
#define EACCES			13
#define EFAULT			14 /* Bad address. An invalid pointer was detected.  */
#define ENOTBLK			15 /* Block device required.  */
/* Device busy. Caused by a system resource that can't be shared
   is already in use.  */
#define EBUSY			16
#define EEXIST			17 /* File exists.  */
#define EXDEV			18 /* Invalid cross-device link.  */
#define ENODEV			19 /* Operation not supported by device.  */
#define ENOTDIR 		20 /* Not a directory.  */
#define EISDIR			21 /* Is a directory.  */
#define EINVAL			22 /* Invalid argument.  */
#define ENFILE			23 /* Too many open files in system.  */
#define EMFILE			24 /* Too many open files.  */
#define ENOTTY			25 /* Inappropriate ioctl for device.  */
#define ETXTBSY 		26 /* Text file busy.  */
#define EFBIG			27 /* File too large.  */
#define ENOSPC			28 /* No space left on device.  */
#define ESPIPE			29 /* Illegal seek.  */
#define EROFS			30 /* Read-only filing system.  */
#define EMLINK			31 /* Too many links.  */
#define EPIPE			32 /* Broken pipe.  */
#ifndef __TARGET_SCL__
#  define EDOM			33 /* Domain error.  */
#  define ERANGE		34 /* Range error.  */
#endif
#define EAGAIN			35 /* Resource temporarily unavailable.  */
#define EWOULDBLOCK		EAGAIN /* Operation would block.  */
#define EINPROGRESS		36 /* Operation now in progress.  */
#define EALREADY		37 /* Operation already in progress.  */
#define ENOTSOCK		38 /* Socket operation on non-socket.  */
#define EDESTADDRREQ		39 /* Destination address required.  */
#define EMSGSIZE		40 /* Message too long.  */
#define EPROTOTYPE		41 /* Protocol wrong type for socket.  */
#define ENOPROTOOPT		42 /* Option not supported by protocol.  */
#define EPROTONOSUPPORT		43 /* Protocol not supported */
#define ESOCKTNOSUPPORT		44 /* Socket type not supported */
#define EOPNOTSUPP		45 /* Operation not supported */
#define EPFNOSUPPORT		46 /* Protocol family not supported.  */
#define EAFNOSUPPORT		47 /* Address family not supported by protocol family. */
#define EADDRINUSE		48 /* Address already in use */
#define EADDRNOTAVAIL		49 /* Can't assign requested address.  */
#define ENETDOWN		50 /* Network is down.  */
#define ENETUNREACH		51 /* Network unreachable */
#define ENETRESET  		52 /* Network dropped connection on reset.  */
#define ECONNABORTED		53 /* Software caused connection abort.  */
#define ECONNRESET		54 /* Connection reset by peer */
#define ENOBUFS			55 /* No buffer space available */
#define EISCONN			56 /* Socket is already connected */
#define ENOTCONN		57 /* Socket is not connected */
#define ESHUTDOWN		58 /* Can't send after socket shutdown.  */
#define ETOOMANYREFS		59 /* Too many references: can't splice.  */
#define ETIMEDOUT		60 /* Connection timed out.  */
#define ECONNREFUSED		61 /* Connection refused */
#define EREFUSED		61 /* Acorn Internet name.  */
#define ELOOP			62 /* Too many levels of symbolic links.  */
#define ENAMETOOLONG		63 /* File name too long.  */
#define EHOSTDOWN		64 /* Host is down.  */
#define EHOSTUNREACH		65 /* Host unreachable.  */
#define ENOTEMPTY		66 /* Directory not empty.  */
#define EPROCLIM		67 /* Too many processes.  */
#define EUSERS			68 /* Too many users.  */
#define EDQUOT			69 /* Disc quota exceeded.  */
#define ESTALE			70 /* Stale NFS file handle.  */
#define EREMOTE			71 /* Too many levels of remote in path.  */

#ifdef __TARGET_SCL__
#  define EBADRPC		72 /* RPC struct is bad */
#  define ERPCMISMATCH		73 /* RPC version wrong */
#  define EPROGUNAVAIL		74 /* RPC prog. not avail */
#  define EPROGMISMATCH		75 /* Program version wrong */
#  define EPROCUNAVAIL		76 /* Bad procedure for program */
#  define ENOLCK		77 /* No locks available */
#  define ENOSYS		78 /* Function not implemented */
#  define EFTYPE		79 /* Inappropriate file type or format */
#  define EAUTH			80 /* Authentication error */
#  define ENEEDAUTH		81 /* Need authenticator */
#  define EOPSYS		82 /* RISC OS operating system error.  */
#  define EILSEQ		83 /* Invalid multibyte sequence.  */

#  define __SYS_NERR	83
#else
#  define ENOSTR		72 /* Not a stream device.  */
#  define ETIME			73 /* Timer expired.  */
#  define ENOSR			74 /* Out of stream resources.  */
#  define ENOMSG		75 /* No message of desired type.  */
#  define EBADMSG		76 /* Not a data message.  */
#  define EIDRM			77 /* Identifier removed.  */
#  define ENOTSUP		78 /* Not supported. */
#  define ENOLCK		79 /* No record locks available.  */
#  define ELIBVER		82 /* Wrong version of shared library.  */
#  define ELIBACC		83 /* Permission denied (shared library).  */
#  define ELIBLIM		84 /* Shared libraries nested too deeply.  */
#  define ELIBNOENT		85 /* Shared library file not found.  */
#  define ELIBNOEXEC		86 /* Shared library exec format error.  */
#  define ENOSYS		87 /* Function not implemented.  */
/* Keep EOPSYS value in sync with EOPSYS value defined in
   internal/asm_dec.s.  */
#  define EOPSYS		88 /* RISC OS operating system error.  */
#  define ESIG			89
#  define EILSEQ		90 /* Invalid multibyte sequence.  */
#  define EOVERFLOW		91 /* Value too large for defined data type */

#  define __SYS_NERR		91
#endif

#undef errno

__BEGIN_DECLS

#if __UNIXLIB_ERRNO_THREADED
/* errno is thread local.  */
#  ifndef __pthread_t_defined
#    define __need_pthread_t
#    include <pthread.h>
#  endif

#  define errno (__pthread_running_thread->thread_errno)
#else
/* Build options specify that errno should be global and not per-thread.  */
extern int errno;
#  define errno errno
#endif
#define __errno errno

extern const char *sys_errlist[];
extern int sys_nerr;

__END_DECLS

#endif
