/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/sys/types.h,v $
 * $Date: 2002/11/27 19:07:01 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

/* POSIX Standard 2.6: Primitive System Data Types <sys/types.h>.  */

#ifndef	__SYS_TYPES_H
#define	__SYS_TYPES_H 1

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

typedef __u_char u_char;
typedef __u_short u_short;
typedef __u_int u_int;
typedef __u_long u_long;
typedef __quad_t quad_t;
typedef __u_quad_t u_quad_t;
typedef __fsid_t fsid_t;

#ifndef __dev_t_defined
typedef __dev_t dev_t;
#define __dev_t_defined
#endif

#ifndef __gid_t_defined
typedef __gid_t gid_t;
#define __gid_t_defined
#endif

#ifndef __ino_t_defined
typedef __ino_t ino_t;
#define __ino_t_defined
#endif

#ifndef __mode_t_defined
typedef __mode_t mode_t;
#define __mode_t_defined
#endif

#ifndef __nlink_t_defined
typedef __nlink_t nlink_t;
#define __nlink_t_defined
#endif

#ifndef __off_t_defined
typedef __off_t off_t;
#define __off_t_defined
#endif

#ifndef __off64_t_defined
typedef __off64_t off64_t;
#define __off64_t_defined
#endif

#ifndef __pid_t_defined
typedef __pid_t pid_t;
#define __pid_t_defined
#endif

#ifndef __uid_t_defined
typedef __uid_t uid_t;
#define __uid_t_defined
#endif

#ifndef __id_t_defined
typedef __id_t id_t;
#define __id_t_defined
#endif

#ifndef __ssize_t_defined
typedef __ssize_t ssize_t;
#define __ssize_t_defined
#endif

typedef __daddr_t daddr_t;
typedef __caddr_t caddr_t;

#define __need_clock_t
#define __need_time_t
#define __need_timer_t
#define __need_clockid_t
#include <time.h>

#define __need_size_t
#include <stddef.h>

/* These size-specific names are used by some of the inet code.  */

#ifndef __int8_t_defined
#define __int8_t_defined
typedef __int32_t int32_t;
typedef __int16_t int16_t;
typedef __int8_t int8_t;
typedef __u_int32_t u_int32_t;
typedef __u_int16_t u_int16_t;
typedef __u_int8_t u_int8_t;
#endif

#ifndef __useconds_t_defined
typedef __useconds_t useconds_t;
#define __useconds_t_defined
#endif

#ifndef __suseconds_t_defined
typedef __suseconds_t suseconds_t;
#define __suseconds_t_defined
#endif


#if !defined __blksize_t_defined
typedef __blksize_t blksize_t;
#define __blksize_t_defined
#endif

#ifndef __blkcnt_t_defined
/* Type to count number of disk blocks.  */
typedef __blkcnt_t blkcnt_t;
#define __blkcnt_t_defined
#endif

#ifndef __fsblkcnt_t_defined
/* Type to count file system blocks.  */
typedef __fsblkcnt_t fsblkcnt_t;
#define __fsblkcnt_t_defined
#endif

#ifndef __fsfilcnt_t_defined
/* Type to count file system inodes.  */
typedef __fsfilcnt_t fsfilcnt_t;
#define __fsfilcnt_t_defined
#endif

/* For BSD.  */
#ifndef __MACHINE_ENDIAN_H
#include <machine/endian.h>
#endif

/* pthread related types */
#define __need_pthread_t
#include <pthread.h>

#endif
