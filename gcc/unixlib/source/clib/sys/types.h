/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/sys/types.h,v $
 * $Date: 2000/07/15 14:52:16 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

/* POSIX Standard 2.6: Primitive System Data Types <sys/types.h>.  */

#ifndef	__SYS_TYPES_H
#define	__SYS_TYPES_H 1

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

#define u_char __u_char
#define u_short __u_short
#define u_int __u_int
#define u_long __u_long
#define quad_t __quad_t
#define u_quad_t __u_quad_t
#define	fsid_t __fsid_t

#define dev_t __dev_t
#define gid_t __gid_t
#define ino_t __ino_t
#define mode_t __mode_t
#define nlink_t __nlink_t
#define off_t __off_t
#define pid_t __pid_t
#define uid_t __uid_t
#ifndef	ssize_t
#define	ssize_t	__ssize_t
#endif

#define daddr_t __daddr_t
#define caddr_t __caddr_t
#define tcflag_t __tcflag_t
#define cc_t __cc_t
#define speed_t __speed_t

#ifndef __TIME_H
#include <time.h>
#endif

#ifndef __STDDEF_H
#include <stddef.h>
#endif

/* These size-specific names are used by some of the inet code.  */

#define int32_t __int32_t
#define int16_t __int16_t
#define int8_t __int8_t
#define u_int32_t __u_int32_t
#define u_int16_t __u_int16_t
#define u_int8_t __u_int8_t

#endif
