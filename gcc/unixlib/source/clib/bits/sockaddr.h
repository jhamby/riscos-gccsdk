/*
 * File taken from glibc 2.2.5.
 * Following changes were made:
 *  - Added #define test on __UNIXLIB_COMPAT_INET4 (equivalent to
 *    COMPAT_INET4 in Acorn's TCPIPLibs).
 */

/* Definition of `struct sockaddr_*' common members.  4.4 BSD version.
   Copyright (C) 1995, 1996, 1997, 1998, 2001 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

/*
 * Never include this file directly; use <sys/socket.h> instead.
 */

#ifndef __BITS_SOCKADDR_H
#define __BITS_SOCKADDR_H	1


/* POSIX.1g specifies this type name for the `sa_family' member.  */
#if __UNIXLIB_COMPAT_INET4
typedef unsigned short sa_family_t;
#else
typedef unsigned char sa_family_t;
#endif

/* This macro is used to declare the initial common members
   of the data types used for socket addresses, `struct sockaddr',
   `struct sockaddr_in', `struct sockaddr_un', etc.  */

#if __UNIXLIB_COMPAT_INET4
#define	__SOCKADDR_COMMON(sa_prefix)	\
  sa_family_t sa_prefix##family

#define __SOCKADDR_COMMON_SIZE	(sizeof (sa_family_t))

#define _HAVE_SA_LEN	0	/* We don't have the sa_len field.  */
#else
#define	__SOCKADDR_COMMON(sa_prefix)	\
  unsigned char sa_prefix##len;		\
  sa_family_t sa_prefix##family

#define __SOCKADDR_COMMON_SIZE	(2 * sizeof (unsigned char))

#define _HAVE_SA_LEN	1	/* We have the sa_len field.  */
#endif

#endif	/* bits/sockaddr.h */
