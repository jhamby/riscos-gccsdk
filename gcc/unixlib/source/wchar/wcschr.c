/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/wchar/c/wcschr,v $
 * $Date: 2000/06/03 16:53:10 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

/* Copyright (C) 1995, 1996 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

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

/* Find the first occurrence of WC in WCS.  */
wchar_t *
wcschr (wcs, wc)
     register const wchar_t *wcs;
     register const wchar_t wc;
{
  while (*wcs != L'\0')
    if (*wcs == wc)
      return (wchar_t *) wcs;
    else
      ++wcs;

  return NULL;
}
