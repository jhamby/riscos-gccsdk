/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/wchar.h,v $
 * $Date: 2003/05/26 18:53:34 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: peter $
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

/*
 *      ISO C Standard, Amendment 1, 7.16.4
 *	General wide-string utilities	<wchar.h>
 */

#ifndef __WCHAR_H
#define __WCHAR_H 1

#ifndef __UNIXLIB_FEATURES_H
#include <unixlib/features.h>
#endif

__BEGIN_DECLS

/* Get size_t, wchar_t, wint_t and NULL from <stddef.h>.  */
#define __need_size_t
#define __need_wchar_t
#define __need_wint_t
#define __need_NULL
#include <stddef.h>


/* Conversion state information.  */
typedef struct
{
  int count;		/* Number of bytes needed for the current character. */
  wint_t value;		/* Value so far.  */
} mbstate_t;

#ifndef WCHAR_MIN
/* These constants might also be defined in <stdint.h> */
# define WCHAR_MIN	(-2147483647-1)
# define WCHAR_MAX	(2147483647)
#endif

#ifndef WEOF
# define WEOF (0xffffffffu)
#endif


/* Copy SRC to DEST.  */
extern wchar_t *wcscpy (wchar_t *__dest, const wchar_t *__src);
/* Copy no more than N wide-characters of SRC to DEST.  */
extern wchar_t *wcsncpy (wchar_t *__dest, const wchar_t *__src,
			      size_t __n);

/* Append SRC onto DEST.  */
extern wchar_t *wcscat (wchar_t *__dest, const wchar_t *__src);
/* Append no more than N wide-characters of SRC onto DEST.  */
extern wchar_t *wcsncat (wchar_t *__dest, const wchar_t *__src,
			      size_t __n);

/* Compare S1 and S2.  */
extern int wcscmp (const wchar_t *__s1, const wchar_t *__s2);
/* Compare N wide-characters of S1 and S2.  */
extern int wcsncmp (const wchar_t *__s1, const wchar_t *__s2,
			 size_t __n);

/* Compare S1 and S2, both interpreted as appropriate to the
   LC_COLLATE category of the current locale.  */
extern int wcscoll (const wchar_t *__s1, const wchar_t *__s2);
/* Transform S2 into array pointed to by S1 such that if wcscmp is
   applied to two transformed strings the result is the as applying
   `wcscoll' to the original strings.  */
extern size_t wcsxfrm (wchar_t *__s1, const wchar_t *__s2, size_t __n);

/* Duplicate S, returning an identical malloc'd string.  */
extern wchar_t *wcsdup (const wchar_t *__s);

/* Find the first occurrence of WC in WCS.  */
extern wchar_t *wcschr (const wchar_t *__wcs, wchar_t __wc);
/* Find the last occurrence of WC in WCS.  */
extern wchar_t *wcsrchr (const wchar_t *__wcs, wchar_t __wc);

/* Return the length of the initial segmet of WCS which
   consists entirely of wide-characters not in REJECT.  */
extern size_t wcscspn (const wchar_t *__wcs,
			    const wchar_t *__reject);
/* Return the length of the initial segmet of WCS which
   consists entirely of wide-characters in  ACCEPT.  */
extern size_t wcsspn (const wchar_t *__wcs, const wchar_t *__accept);
/* Find the first occurrence in WCS of any character in ACCEPT.  */
extern wchar_t *wcspbrk (const wchar_t *__wcs,
			      const wchar_t *__accept);
/* Find the first occurrence of NEEDLE in HAYSTACK.  */
extern wchar_t *wcsstr (const wchar_t *__haystack,
			     const wchar_t *__needle);
/* Divide WCS into tokens separated by characters in DELIM.  */
extern wchar_t *wcstok (wchar_t *__s, const wchar_t *__delim,
			     wchar_t **ptr);

/* Return the number of wide-characters in S.  */
extern size_t wcslen (const wchar_t *__s);


/* Search N bytes of S for C.  */
extern wchar_t *wmemchr (const wchar_t *__s, wchar_t __c, size_t __n);

/* Compare N bytes of S1 and S2.  */
extern int wmemcmp (const wchar_t *__s1, const wchar_t *__s2,
			 size_t __n);

/* Copy N bytes of SRC to DEST.  */
extern wchar_t *wmemcpy (wchar_t *__s1, const wchar_t *__s2,
			      size_t __n);

/* Copy N bytes of SRC to DEST, guaranteeing
   correct behavior for overlapping strings.  */
extern wchar_t *wmemmove (wchar_t *__s1, const wchar_t *__s2,
			       size_t __n);

/* Set N bytes of S to C.  */
extern wchar_t *wmemset (wchar_t *__s, wchar_t __c, size_t __n);


/* Determine whether C constitutes a valid (one-byte) multibyte
   character.  */
extern wint_t btowc (int __c);

/* Determine whether C corresponds to a member of the extended
   character set whose multibyte representation is a single byte.  */
extern int wctob (wint_t __c);

/* Determine whether PS points to an object representing the initial
   state.  */
extern int mbsinit (const mbstate_t *__ps);

/* Write wide character representation of multibyte character pointed
   to by S to PWC.  */
extern size_t __mbrtowc (wchar_t *__pwc, const char *__s, size_t __n,
			      mbstate_t *__p);
extern size_t mbrtowc (wchar_t *__pwc, const char *__s, size_t __n,
			    mbstate_t *__p);

/* Write multibyte representation of wide character WC to S.  */
extern size_t __wcrtomb (char *__s, wchar_t __wc, mbstate_t *__ps);
extern size_t wcrtomb (char *__s, wchar_t __wc, mbstate_t *__ps);

/* Return number of bytes in multibyte character pointed to by S.  */
extern size_t __mbrlen (const char *__s, size_t __n, mbstate_t *__ps);
extern size_t mbrlen (const char *__s, size_t __n, mbstate_t *__ps);

#if 0
#if defined (__OPTIMIZE__) && defined (__GNUC__)
/* Define inline function as optimization.  */
extern __inline size_t mbrlen (const char *s, size_t n, mbstate_t *ps)
{ return ps != NULL ? __mbrtowc (NULL, s, n, ps) : __mbrlen (s, n, NULL); }
#endif
#endif

/* Write wide character representation of multibyte character string
   SRC to DST.  */
extern size_t __mbsrtowcs (wchar_t *__dst, const char **__src,
				size_t __len, mbstate_t *__ps);
extern size_t mbsrtowcs (wchar_t *__dst, const char **__src,
			      size_t __len, mbstate_t *__ps);

/* Write multibyte character representation of wide character string
   SRC to DST.  */
extern size_t __wcsrtombs (char *__dst, const wchar_t **__src,
				size_t __len, mbstate_t *__ps);
extern size_t wcsrtombs (char *__dst, const wchar_t **__src,
			      size_t __len, mbstate_t *__ps);

/* Write wide character representation of at most NMC bytes of the
   multibyte character string SRC to DST.  */
extern size_t __mbsnrtowcs (wchar_t *__dst, const char **__src,
				 size_t __nmc, size_t __len, mbstate_t *__ps);
extern size_t mbsnrtowcs (wchar_t *__dst, const char **__src,
			       size_t __nmc, size_t __len, mbstate_t *__ps);

/* Write multibyte character representation of at most NWC characters
   from the wide character string SRC to DST.  */
extern size_t __wcsnrtombs (char *__dst, const wchar_t **__src,
				 size_t __nwc, size_t __len, mbstate_t *__ps);
extern size_t wcsnrtombs (char *__dst, const wchar_t **__src,
			       size_t __nwc, size_t __len, mbstate_t *__ps);


/* The following functions are extensions found in X/Open CAE.  */

/* Determine number of column positions required for C.  */
extern int wcwidth (wint_t __c);

/* Determine number of column positions required for first N wide
   characters (or fewer if S ends before this) in S.  */
extern int wcswidth (const wchar_t *__s, size_t __n);


/* Convert initial portion of the wide string NPTR to `double'
   representation.  */
extern double wcstod (const wchar_t *__nptr, wchar_t **__endptr);

/* Likewise for `float' and `long double' sizes of floating-point numbers.  */
extern float wcstof (const wchar_t *__nptr, wchar_t **__endptr);
extern long double wcstold (const wchar_t *__nptr,
				     wchar_t **__endptr);


/* Convert initial portion of wide string NPTR to `long int'
   representation.  */
extern long int wcstol (const wchar_t *__nptr, wchar_t **__endptr,
			     int __base);

/* Convert initial portion of wide string NPTR to `unsigned long int'
   representation.  */
extern unsigned long int wcstoul (const wchar_t *__nptr,
				       wchar_t **__endptr, int __base);

#if defined (__GNUC__)
/* Convert initial portion of wide string NPTR to `long int'
   representation.  */
__extension__ extern long long int wcstoq (const wchar_t *__nptr,
					   wchar_t **__endptr,
					   int __base);

/* Convert initial portion of wide string NPTR to `unsigned long long int'
   representation.  */
__extension__ extern unsigned long long int wcstouq (const wchar_t *__nptr,
						     wchar_t **__endptr,
						     int __base);
#endif /* GCC.  */


/* The internal entry points for `wcstoX' take an extra flag argument
   saying whether or not to parse locale-dependent number grouping.  */
extern double __wcstod_internal (const wchar_t *__nptr,
				      wchar_t **__endptr, int __group);
extern float __wcstof_internal (const wchar_t *__nptr,
				     wchar_t **__endptr, int __group);

#if 0
#if defined (__OPTIMIZE__) && defined (__GNUC__)
/* Define inline functions which call the internal entry points.  */

extern inline double wcstod (const wchar_t *__nptr, wchar_t **__endptr)
{ return __wcstod_internal (__nptr, __endptr, 0); }
extern inline long int wcstol (const wchar_t *__nptr,
                                 wchar_t **__endptr, int __base)
{ return __wcstol_internal (__nptr, __endptr, __base, 0); }
extern inline unsigned long int wcstoul (const wchar_t *__nptr,
                                           wchar_t **__endptr, int __base)
{ return __wcstoul_internal (__nptr, __endptr, __base, 0); }

extern inline float wcstof (const wchar_t *__nptr, wchar_t **__endptr)
{ return __wcstof_internal (__nptr, __endptr, 0); }
extern inline __long_double_t wcstold (const wchar_t *__nptr,
				       wchar_t **__endptr)

{ return __wcstold_internal (__nptr, __endptr, 0); }


extern inline long long int wcstoq (const wchar_t *__nptr,
				      wchar_t **__endptr, int __base)
{ return __wcstoq_internal (__nptr, __endptr, __base, 0); }
extern inline unsigned long long int wcstouq (const wchar_t *__nptr,
						wchar_t **__endptr, int __base)
{ return __wcstouq_internal (__nptr, __endptr, __base, 0); }
#endif /* Optimizing GCC >=2.  */
#endif

/* Copy SRC to DEST, returning the address of the terminating L'\0' in
   DEST.  */
extern wchar_t *__wcpcpy (wchar_t *__dest, const wchar_t *__src);
extern wchar_t *wcpcpy (wchar_t *__dest, const wchar_t *__src);

/* Copy no more than N characters of SRC to DEST, returning the address of
   the last character written into DEST.  */
extern wchar_t *__wcpncpy (wchar_t *__dest, const wchar_t *__src,
				size_t __n);
extern wchar_t *wcpncpy (wchar_t *__dest, const wchar_t *__src,
			      size_t __n);

__END_DECLS

#endif /* wchar.h  */
