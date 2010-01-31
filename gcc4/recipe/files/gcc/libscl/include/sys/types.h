/* sys/types.h extension header for the RISC OS SharedCLibrary.
   Copyright (c) 1997-2005 Nick Burrett
   All rights reserved.
 
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
   3. The name of the author may not be used to endorse or promote products
      derived from this software without specific prior written permission.
 
   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
   IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
   IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
   NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
   THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#ifndef __SYS_TYPES_H
#define __SYS_TYPES_H 1

#include <stdint.h>

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
#ifndef ssize_t
#define ssize_t __ssize_t
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

#define u_int8_t __u_int8_t
#define u_int16_t __u_int16_t
#define u_int32_t __u_int32_t
#define u_int64_t __u_int64_t

#endif
