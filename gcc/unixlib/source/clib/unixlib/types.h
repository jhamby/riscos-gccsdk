/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/unixlib/types.h,v $
 * $Date: 2002/02/14 15:56:35 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

/* Used by a few header files to define types. This is internal to UnixLib,
   please use <sys/types.h> instead.  */

#ifndef __UNIXLIB_TYPES_H
#define __UNIXLIB_TYPES_H 1

#define __need_size_t
#include <stddef.h>

/* Convenience types.  */
typedef unsigned char __u_char;
typedef unsigned short __u_short;
typedef unsigned int __u_int;
typedef unsigned long __u_long;

#ifdef __GNUC__
__extension__
typedef unsigned long long int __u_quad_t;
__extension__
typedef long long int __quad_t;
typedef __quad_t *__qaddr_t;
#else
typedef struct
{
  long val[2];
} __quad_t;
typedef struct
{
  __u_long val[2];
} __u_quad_t;
#endif

typedef int __dev_t;		/* Type of device numbers.  */
typedef unsigned int __uid_t;	/* Type of user identifications.  */
typedef unsigned int __gid_t;	/* Type of group identifications.  */
typedef unsigned int __ino_t;	/* Type of file serial numbers.  */
typedef unsigned int __mode_t;	/* Type of file attribute bitmasks.  */
typedef unsigned short int __nlink_t; /* Type of file link counts.  */
typedef long int __off_t;	/* Type of file sizes and offsets.  */
typedef int __pid_t;		/* Type of process identifications.  */
typedef int __ssize_t;		/* Type of a byte count, or error.  */
typedef __u_quad_t __fsid_t;	/* Type of file system IDs.  */
typedef __u_long __rlim_t;	/* Type of resource counts.  */
typedef long int __daddr_t;	/* The type of a disk address.  */
typedef char *__caddr_t;
typedef unsigned int __time_t;  /* Calendar time.  */
typedef long int __clock_t;	/* Clock ticks.  */
typedef int __clockid_t;	/* Used in clock/timer functions.  */
typedef unsigned int __useconds_t;
typedef long int __suseconds_t;
typedef int __timer_t;		/* Timer ID returned by timer_create.  */
typedef long int __swblk_t;	/* Type of a swap block maybe?  */
typedef __u_int __id_t;		/* General type for ID.  */

/* An unsigned type used to represent the various bit masks for
   terminal flags.  */
typedef unsigned long __tcflag_t;

/* Unsigned type used to represent characters associated with various
   terminal control functions.  */
typedef unsigned char __cc_t;

/* Type of baud rate specifiers.  */
typedef long int __speed_t;

/* These size-specific names are used by some of the inet code.  */

#ifdef __GNUC__
typedef int __int8_t __attribute__ ((__mode__ (__QI__)));
typedef unsigned int __u_int8_t __attribute__ ((__mode__ (__QI__)));
typedef int __int16_t __attribute__ ((__mode__ (__HI__)));
typedef unsigned int __u_int16_t __attribute__ ((__mode__ (__HI__)));
typedef int __int32_t __attribute__ ((__mode__ (__SI__)));
typedef unsigned int __u_int32_t __attribute__ ((__mode__ (__SI__)));
typedef int __int64_t __attribute__ ((__mode__ (__DI__)));
typedef unsigned int __u_int64_t __attribute__ ((__mode__ (__DI__)));
#else
typedef	char __int8_t;
typedef	unsigned char __u_int8_t;
typedef	short __int16_t;
typedef	unsigned short __u_int16_t;
typedef	int __int32_t;
typedef	unsigned int __u_int32_t;
#endif

typedef int __sig_atomic_t;
typedef unsigned long __sigset_t;

/* Type to represent block size.  */
typedef long int __blksize_t;

/* Types from the Large File Support interface.  */

/* Type to count number os disk blocks.  */
typedef long int __blkcnt_t;
typedef __quad_t __blkcnt64_t;

/* Type to count file system blocks.  */
typedef __u_long __fsblkcnt_t;
typedef __u_quad_t __fsblkcnt64_t;

/* Type to count file system inodes.  */
typedef __u_long __fsfilcnt_t;
typedef __u_quad_t __fsfilcnt64_t;

/* Type of file serial numbers.  */
typedef __u_quad_t __ino64_t;

/* Used in XTI.  */
typedef long int __t_scalar_t;
typedef unsigned long int __t_uscalar_t;

/* Duplicates info from stdint.h but this is used in unistd.h.  */
typedef int __intptr_t;

/* Duplicate info from sys/socket.h.  */
typedef unsigned int __socklen_t;

#ifdef __GNUC__
typedef long long int __off64_t;
typedef long long int __fpos64_t;
#endif

#endif /* __UNIXLIB_TYPES_H */
