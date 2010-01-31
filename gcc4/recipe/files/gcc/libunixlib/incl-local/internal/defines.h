/* UnixLib internal #defines's.  */

/* The following is an extract from gcc's lib1funcs.asm.  */

/* Copyright 1995, 1996, 1998, 1999, 2000, 2003, 2004, 2005
   Free Software Foundation, Inc.

This file is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.

In addition to the permissions in the GNU General Public License, the
Free Software Foundation gives you unlimited permission to link the
compiled version of this file into combinations with other programs,
and to distribute those combinations without any restriction coming
from the use of this file.  (The General Public License restrictions
do apply in other respects; for example, they cover modification of
the file, and distribution when not linked into a combine
executable.)

This file is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to
the Free Software Foundation, 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

#ifndef __UNIXLIB_DEFINES
#define __UNIXLIB_DEFINES

/* We minimally require ARM architecture 3.  */
#define __ARM_ARCH__ 3
#define __ARM_ARCH_STR__ "3"

#if defined(__ARM_ARCH_3M__) || defined(__ARM_ARCH_4__) \
	|| defined(__ARM_ARCH_4T__)
/* We use __ARM_ARCH__ set to 4 here, but in reality it's any processor with
   long multiply instructions.  That includes v3M.  */
# undef __ARM_ARCH__
# define __ARM_ARCH__ 4
# undef __ARM_ARCH_STR__
# define __ARM_ARCH_STR__ "4"
#endif
	
#if defined(__ARM_ARCH_5__) || defined(__ARM_ARCH_5T__) \
	|| defined(__ARM_ARCH_5E__) || defined(__ARM_ARCH_5TE__) \
	|| defined(__ARM_ARCH_5TEJ__)
# undef __ARM_ARCH__
# define __ARM_ARCH__ 5
# undef __ARM_ARCH_STR__
# define __ARM_ARCH_STR__ "5"
#endif

#if defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) \
	|| defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) \
	|| defined(__ARM_ARCH_6ZK__)
# undef __ARM_ARCH__
# define __ARM_ARCH__ 6
# undef __ARM_ARCH_STR__
# define __ARM_ARCH_STR__ "6"
#endif

#endif
