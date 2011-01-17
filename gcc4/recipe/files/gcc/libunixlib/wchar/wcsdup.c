/*
 * File taken from glibc.
 * Following changes were made:
 *  - SCL poison added.
 */
#ifdef __TARGET_SCL__
#  error "SCL build should not use (L)GPL code."
#endif

/* Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@gnu.ai.mit.edu>, 1995.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#include <wchar.h>
#include <string.h>
#include <stdlib.h>

/* Duplicate S, returning an identical malloc'd string.	 */
wchar_t *
wcsdup (const wchar_t *s)
{
  size_t len = (wcslen (s) + 1) * sizeof (wchar_t);
  void *newstr = malloc (len);

  if (newstr == NULL)
    return NULL;

  memcpy (newstr, (void *) s, len);

  return (wchar_t *) newstr;
}
