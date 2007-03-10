/* Copyright (C) 1996,1997,1998,1999,2000,2001 Free Software Foundation, Inc.
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
 *	ISO C99 Standard: 7.25
 *	Wide character classification and mapping utilities  <wctype.h>
 */

#ifndef __WCTYPE_H
#define __WCTYPE_H

#include <wchar.h>
#include <stddef.h>
#include <sys/types.h>
#include <ctype.h>

/*
 * Wide-character classification functions: 7.15.2.1.
 */

typedef unsigned long int wctype_t;

/* Test for any wide character for which `iswalpha' or `iswdigit' is
   true.  */
extern int iswalnum (wint_t __wc);

/* Test for any wide character for which `iswupper' or 'iswlower' is
   true, or any wide character that is one of a locale-specific set of
   wide-characters for which none of `iswcntrl', `iswdigit',
   `iswpunct', or `iswspace' is true.  */
extern int iswalpha (wint_t __wc);

/* Test for any control wide character.  */
extern int iswcntrl (wint_t __wc);

/* Test for any wide character that corresponds to a decimal-digit
   character.  */
extern int iswdigit (wint_t __wc);

/* Test for any wide character for which `iswprint' is true and
   `iswspace' is false.  */
extern int iswgraph (wint_t __wc);

/* Test for any wide character that corresponds to a lowercase letter
   or is one of a locale-specific set of wide characters for which
   none of `iswcntrl', `iswdigit', `iswpunct', or `iswspace' is true.  */
extern int iswlower (wint_t __wc);

/* Test for any printing wide character.  */
extern int iswprint (wint_t __wc);

/* Test for any printing wide character that is one of a
   locale-specific et of wide characters for which neither `iswspace'
   nor `iswalnum' is true.  */
extern int iswpunct (wint_t __wc);

/* Test for any wide character that corresponds to a locale-specific
   set of wide characters for which none of `iswalnum', `iswgraph', or
   `iswpunct' is true.  */
extern int iswspace (wint_t __wc);

/* Test for any wide character that corresponds to an uppercase letter
   or is one of a locale-specific set of wide character for which none
   of `iswcntrl', `iswdigit', `iswpunct', or `iswspace' is true.  */
extern int iswupper (wint_t __wc);

/* Test for any wide character that corresponds to a hexadecimal-digit
   character equivalent to that performed be the functions described
   in the previous subclause.  */
extern int iswxdigit (wint_t __wc);

extern int towupper( wint_t __wc);

extern int towlower( wint_t __wc);

#endif /* wctype.h  */
