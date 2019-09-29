/*
 * File taken from glibc.
 *  - SCL poison added.
 */
#ifdef __TARGET_SCL__
#  error "SCL build should not use (L)GPL code."
#endif

/* Code to enable profiling at program startup.
   Copyright (C) 1995,1996,1997,2000,2001,2002 Free Software Foundation, Inc.
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

#include <sys/types.h>
#include <sys/gmon.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef __ARM_EABI__

/* Beginning and end of our code segment. We cannot declare them
   as the external functions since we want the addresses of those
   labels. Taking the address of a function may have different
   meanings on different platforms. */
extern void __text_start;
extern void etext;

#define TEXT_START &__text_start

/* In ELF and COFF, we cannot use the normal constructor mechanism to call
   __gmon_start__ because gcrt1.o appears before crtbegin.o in the link.
   Instead crti.o calls it specially (see initfini.c).  */
extern void __gmon_start__ (void);

void
__gmon_start__ (void)
{
#ifdef HAVE_INITFINI
  /* Protect from being called more than once.  Since crti.o is linked
     into every shared library, each of their init functions will call us.  */
  static int called;

  if (called)
    return;

  called = 1;
#endif

  /* Start keeping profiling records.  */
  __monstartup ((u_long) TEXT_START, (u_long) &etext);

  /* Call _mcleanup before exiting; it will write out gmon.out from the
     collected data.  */
  atexit (&_mcleanup);
}

#endif
