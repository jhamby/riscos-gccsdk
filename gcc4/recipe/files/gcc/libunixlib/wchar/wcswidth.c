/*
 * File taken from glibc.
 * Following changes were made:
 *  - SCL poison added.
 */
#ifdef __TARGET_SCL__
#  error "SCL build should not use (L)GPL code."
#endif

/* Copyright (C) 1996, 1997 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@gnu.ai.mit.edu>, 1996.

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

/* Determine number of column positions required for first N wide
   characters (or fewer if S ends before this) in S.  */
int
wcswidth (const wchar_t *s, size_t n)
{
  int result = 0;

  while (n > 0 && *s != L'\0')
    {
      int now = 1;
      if (now == -1)
	return -1;
      result += now;
      ++s;
    }

  return result;
}
