/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/clib/unixlib/h/types,v $
 * $Date: 1997/12/17 22:02:53 $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

/* Used by a few header files to define types. This is internal to UnixLib,
   please use <sys/types.h> instead.  */

#ifndef __UNIXLIB_TYPES_H
#define __UNIXLIB_TYPES_H 1

/* Convenience types.  */
typedef unsigned char __u_char;
typedef unsigned short __u_short;
typedef unsigned int __u_int;
typedef unsigned long __u_long;

#ifdef __GNUC__
typedef unsigned long long int __u_quad_t;
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

typedef long int __daddr_t;	/* The type of a disk address.  */
typedef char *__caddr_t;
typedef unsigned int __time_t;  /* Calendar time.  */
typedef long int __clock_t;	/* Clock ticks.  */
typedef long int __swblk_t;	/* Type of a swap block maybe?  */

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

/* fd_set for select.  */

/* Number of descriptors that can fit in an `fd_set'.  */
#define	__FD_SETSIZE	256

/* It's easier to assume 8-bit bytes than to get CHAR_BIT.  */
#define	__NFDBITS	(sizeof (unsigned long int) * 8)
#define	__FDELT(d)	((d) / __NFDBITS)
#define	__FDMASK(d)	(1ul << ((d) % __NFDBITS))

typedef struct
  {
    /* Some braindead old software uses this member name.  */
    unsigned long int fds_bits[(__FD_SETSIZE + (__NFDBITS - 1)) / __NFDBITS];
  } __fd_set;

typedef unsigned long int __fd_mask;

#define	__FD_ZERO(set) ((void) memset (set, 0, sizeof (*(set))))
#define	__FD_SET(d, set)	((set)->fds_bits[__FDELT(d)] |= __FDMASK(d))
#define	__FD_CLR(d, set)	((set)->fds_bits[__FDELT(d)] &= ~__FDMASK(d))
#define	__FD_ISSET(d, set)	((set)->fds_bits[__FDELT(d)] & __FDMASK(d))

#endif
