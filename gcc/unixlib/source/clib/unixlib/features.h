/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/unixlib/features.h,v $
 * $Date: 2003/04/29 21:16:11 $
 * $Revision: 1.8 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

/* The header file <unixlib/features.h> contains test macros for optionally
   turning on/off certain features that the library provides or integrity
   checks.  */

/* This file is derived from GNU Libc 2.2.4, features.h  We are attempting
   to closely match the macros defined within it in order to maintain
   a decent level of compatibility.  A desire to do this forces me to
   include the following license:

   Copyright (C) 1992,93,94,95,96,97,98,99, 2000 Free Software Foundation, Inc.
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

#ifndef __UNIXLIB_FEATURES_H
#define __UNIXLIB_FEATURES_H 1


/* These are defined by the user (or the compiler)
   to specify the desired environment:

   __STRICT_ANSI__      ISO Standard C.
   _ISOC99_SOURCE       Extensions to ISO C89 from ISO C99.
   _POSIX_SOURCE        IEEE Std 1003.1.
   _POSIX_C_SOURCE      If ==1, like _POSIX_SOURCE; if >=2 add IEEE Std 1003.2;
                        if >=199309L, add IEEE Std 1003.1b-1993;
                        if >=199506L, add IEEE Std 1003.1c-1995
   _XOPEN_SOURCE        Includes POSIX and XPG things.  Set to 500 if
                        Single Unix conformance is wanted, to 600 for the
                        upcoming sixth revision.
   _XOPEN_SOURCE_EXTENDED XPG things and X/Open Unix extensions.
   _LARGEFILE_SOURCE    Some more functions for correct standard I/O.
   _LARGEFILE64_SOURCE  Additional functionality from LFS for large files.
   _FILE_OFFSET_BITS=N  Select default filesystem interface.
   _BSD_SOURCE          ISO C, POSIX, and 4.3BSD things.
   _SVID_SOURCE         ISO C, POSIX, and SVID things.
   _GNU_SOURCE          All of the above, plus GNU extensions.
   _REENTRANT           Select additionally reentrant object.
   _THREAD_SAFE         Same as _REENTRANT, often used by other systems.

   The `-ansi' switch to the GNU C compiler defines __STRICT_ANSI__.
   If none of these are defined, the default is to have _SVID_SOURCE,
   _BSD_SOURCE, and _POSIX_SOURCE set to one and _POSIX_C_SOURCE set to
   199506L.  If more than one of these are defined, they accumulate.
   For example __STRICT_ANSI__, _POSIX_SOURCE and _POSIX_C_SOURCE
   together give you ISO C, 1003.1, and 1003.2, but nothing else.

   These are defined by this file and are used by the
   header files to decide what to declare or define:

   __USE_ISOC99         Define ISO C99 things.
   __USE_POSIX          Define IEEE Std 1003.1 things.
   __USE_POSIX2         Define IEEE Std 1003.2 things.
   __USE_POSIX199309    Define IEEE Std 1003.1, and .1b things.
   __USE_POSIX199506    Define IEEE Std 1003.1, .1b, .1c and .1i things.
   __USE_XOPEN          Define XPG things.
   __USE_XOPEN_EXTENDED Define X/Open Unix things.
   __USE_UNIX98         Define Single Unix V2 things.
   __USE_XOPEN2K        Define XPG6 things.
   __USE_LARGEFILE      Define correct standard I/O things.
   __USE_LARGEFILE64    Define LFS things with separate names.
   __USE_FILE_OFFSET64  Define 64bit interface as default.
   __USE_BSD            Define 4.3BSD things.
   __USE_SVID           Define SVID things.
   __USE_MISC           Define things common to BSD and System V Unix.
   __USE_GNU            Define GNU extensions.
   __USE_REENTRANT      Define reentrant/thread-safe *_r functions.
   __FAVOR_BSD          Favor 4.3BSD things in cases of conflict.

   The macros `__GNU_LIBRARY__', `__GLIBC__', and `__GLIBC_MINOR__' are
   defined by this file unconditionally.  `__GNU_LIBRARY__' is provided
   only for compatibility.  All new code should use the other symbols
   to test for features.

   All macros listed above as possibly being defined by this file are
   explicitly undefined if they are not explicitly defined.
   Feature-test macros that are not defined by the user or compiler
   but are implied by the other feature-test macros defined (or by the
   lack of any definitions) are defined by the file.  */


/* Undefine everything, so we get a clean slate.  */
#undef  __USE_ISOC99
#undef  __USE_POSIX
#undef  __USE_POSIX2
#undef  __USE_POSIX199309
#undef  __USE_POSIX199506
#undef  __USE_XOPEN
#undef  __USE_XOPEN_EXTENDED
#undef  __USE_UNIX98
#undef  __USE_XOPEN2K
#undef  __USE_LARGEFILE
#undef  __USE_LARGEFILE64
#undef  __USE_FILE_OFFSET64
#undef  __USE_BSD
#undef  __USE_SVID
#undef  __USE_MISC
#undef  __USE_GNU
#undef  __USE_REENTRANT
#undef  __FAVOR_BSD
#undef  __KERNEL_STRICT_NAMES

/* This will reduce the number of Norcroft compiler warnings "Undefined macro
   'xxx' in #if - treated as 0".  */
#ifdef __CC_NORCROFT
# ifndef __STDC_VERSION__
#  define __STDC_VERSION__       199409L
# endif
# ifndef _ISOC99_SOURCE
#  define _ISOC99_SOURCE         1
# endif
# ifndef _POSIX_SOURCE
#  define _POSIX_SOURCE          1
# endif
# ifndef _POSIX_C_SOURCE
#  define _POSIX_C_SOURCE        199506L
# endif
# ifndef _XOPEN_SOURCE
#  define _XOPEN_SOURCE          500
# endif
# ifndef _XOPEN_SOURCE_EXTENDED
#  define _XOPEN_SOURCE_EXTENDED 1
# endif
# ifndef _LARGEFILE64_SOURCE
#  define _LARGEFILE64_SOURCE    1
# endif
# ifndef _BSD_SOURCE
#  define _BSD_SOURCE            1
# endif
# ifndef _SVID_SOURCE
#  define _SVID_SOURCE           1
# endif
# ifndef _FILE_OFFSET_BITS
#  define _FILE_OFFSET_BITS      32
# endif
#endif

/* A lot of GNU glibc code is depending on this. I don't know if it is
   a good idea to define this here:  */
#undef BSD
#define BSD    199103

/* If _BSD_SOURCE was defined by the user, favor BSD over POSIX.  */
#if defined _BSD_SOURCE && \
    !(defined _POSIX_SOURCE || defined _POSIX_C_SOURCE || \
      defined _XOPEN_SOURCE || defined _XOPEN_SOURCE_EXTENDED || \
      defined _GNU_SOURCE || defined _SVID_SOURCE)
# define __FAVOR_BSD    1
#endif

/* If _GNU_SOURCE was defined by the user, turn on all the other features.  */
#ifdef _GNU_SOURCE
# undef  _ISOC99_SOURCE
# define _ISOC99_SOURCE 1
# undef  _POSIX_SOURCE
# define _POSIX_SOURCE  1
# undef  _POSIX_C_SOURCE
# define _POSIX_C_SOURCE        199506L
# undef  _XOPEN_SOURCE
# define _XOPEN_SOURCE  600
# undef  _XOPEN_SOURCE_EXTENDED
# define _XOPEN_SOURCE_EXTENDED 1
# undef  _LARGEFILE64_SOURCE
# define _LARGEFILE64_SOURCE    1
# undef  _BSD_SOURCE
# define _BSD_SOURCE    1
# undef  _SVID_SOURCE
# define _SVID_SOURCE   1
#endif

/* If nothing (other than _GNU_SOURCE) is defined,
   define _BSD_SOURCE and _SVID_SOURCE.  */
#if (!defined __STRICT_ANSI__ && !defined _ISOC99_SOURCE && \
     !defined _POSIX_SOURCE && !defined _POSIX_C_SOURCE && \
     !defined _XOPEN_SOURCE && !defined _XOPEN_SOURCE_EXTENDED && \
     !defined _BSD_SOURCE && !defined _SVID_SOURCE)
# define _BSD_SOURCE    1
# define _SVID_SOURCE   1
#endif

/* This is to enable the ISO C99 extension.  Also recognize the old macro
   which was used prior to the standard acceptance.  This macro will
   eventually go away and the features enabled by default once the ISO C99
   standard is widely adopted.  */
#if (defined _ISOC99_SOURCE || defined _ISOC9X_SOURCE \
     || (defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L))
# define __USE_ISOC99   1
#endif

/* If none of the ANSI/POSIX macros are defined, use POSIX.1 and POSIX.2
   (and IEEE Std 1003.1b-1993 unless _XOPEN_SOURCE is defined).  */
#if (!defined __STRICT_ANSI__ && !defined _POSIX_SOURCE && \
     !defined _POSIX_C_SOURCE)
# define _POSIX_SOURCE  1
# if defined _XOPEN_SOURCE && (_XOPEN_SOURCE - 0) < 500
#  define _POSIX_C_SOURCE       2
# else
#  define _POSIX_C_SOURCE       199506L
# endif
#endif

#if defined _POSIX_SOURCE || _POSIX_C_SOURCE >= 1 || defined _XOPEN_SOURCE
# define __USE_POSIX    1
#endif

#if defined _POSIX_C_SOURCE && _POSIX_C_SOURCE >= 2 || defined _XOPEN_SOURCE
# define __USE_POSIX2   1
#endif

#if (_POSIX_C_SOURCE - 0) >= 199309L
# define __USE_POSIX199309      1
#endif

#if (_POSIX_C_SOURCE - 0) >= 199506L
# define __USE_POSIX199506      1
#endif

#ifdef  _XOPEN_SOURCE
# define __USE_XOPEN    1
# if (_XOPEN_SOURCE - 0) >= 500
#  define __USE_XOPEN_EXTENDED  1
#  define __USE_UNIX98  1
#  undef _LARGEFILE_SOURCE
#  define _LARGEFILE_SOURCE     1
#  if (_XOPEN_SOURCE - 0) >= 600
#   define __USE_XOPEN2K        1
#   undef __USE_ISOC99
#   define __USE_ISOC99         1
#  endif
# else
#  ifdef _XOPEN_SOURCE_EXTENDED
#   define __USE_XOPEN_EXTENDED 1
#  endif
# endif
#endif

#ifdef _LARGEFILE_SOURCE
# define __USE_LARGEFILE        1
#endif

#ifdef _LARGEFILE64_SOURCE
# define __USE_LARGEFILE64      1
#endif

#if defined _FILE_OFFSET_BITS && _FILE_OFFSET_BITS == 64
# define __USE_FILE_OFFSET64    1
#endif

#if defined _BSD_SOURCE || defined _SVID_SOURCE
# define __USE_MISC     1
#endif

#ifdef  _BSD_SOURCE
# define __USE_BSD      1
#endif

#ifdef  _SVID_SOURCE
# define __USE_SVID     1
#endif

#ifdef  _GNU_SOURCE
# define __USE_GNU      1
#endif

#if defined _REENTRANT || defined _THREAD_SAFE
# define __USE_REENTRANT        1
#endif

/* We do support the IEC 559 math functionality, real and complex.  */
#define __STDC_IEC_559__                1
#define __STDC_IEC_559_COMPLEX__        1

/* wchar_t uses ISO 10646-1 (2nd ed., published 2000-09-15) / Unicode 3.0.  */
#define __STDC_ISO_10646__              200009L


/* Major and minor version number of the UnixLib C library package.  Use
   these macros to test for features in specific releases.  */
#define __ULIBC__       4
#define __ULIBC_MINOR__ 0

/* Convenience macros to test the versions of unixlib and gcc.
   Use them like this:
   #if __GNUC_PREREQ (2,8)
   ... code requiring gcc 2.8 or later ...
   #endif
*/
#if defined __GNUC__ && defined __GNUC_MINOR__
# define __GNUC_PREREQ(maj, min) \
        ((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#else
# define __GNUC_PREREQ(maj, min) 0
#endif

#define __ULIBC_PREREQ(maj, min) \
        ((__ULIBC__ << 16) + __ULIBC_MINOR__ >= ((maj) << 16) + (min))


/* Some user header file might have defined this before.  */
#undef	__P
#undef	__PMT

#ifdef __GNUC__

/* GCC can always grok prototypes.  For C++ programs we add throw()
   to help it optimize the function calls.  But this works only with
   gcc 2.8.x and egcs.  */
# if defined __cplusplus && __GNUC_PREREQ (2,8)
#  define __THROW	throw ()
# else
#  define __THROW
# endif
# define __P(args)	args __THROW
/* This macro will be used for functions which might take C++ callback
   functions.  */
# define __PMT(args)	args

#else	/* Not GCC.  */

# define __inline		/* No inline functions.  */

# define __THROW
# define __P(args)	args
# define __PMT(args)	args

# define __const	const
# define __signed	signed
# define __volatile	volatile

#endif	/* GCC.  */

/* For these things, GCC behaves the ANSI way normally,
   and the non-ANSI way under -traditional.  */

#define __CONCAT(x,y)	x ## y
#define __STRING(x)	#x

/* This is not a typedef so `const __ptr_t' does the right thing.  */
#define __ptr_t void *
#define __long_double_t  long double


/* C++ needs to know that types and declarations are C, not C++.  */
#ifdef	__cplusplus
# define __BEGIN_DECLS	extern "C" {
# define __END_DECLS	}
#else
# define __BEGIN_DECLS
# define __END_DECLS
#endif

/* Support for bounded pointers.  */
#ifndef __BOUNDED_POINTERS__
# define __bounded	/* nothing */
# define __unbounded	/* nothing */
# define __ptrvalue	/* nothing */
#endif


#ifndef __flexarr
/* Support for flexible arrays.  */
# if __GNUC_PREREQ (2,97)
/* GCC 2.97 supports C99 flexible array members.  */
#  define __flexarr	[]
# else
#  ifdef __GNUC__
#   define __flexarr	[0]
#  else
#   if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#    define __flexarr	[]
#   else
/* Some other non-C99 compiler.  Approximate with [1].  */
#    define __flexarr	[1]
#   endif
#  endif
# endif
#endif


/* __asm__ ("xyz") is used throughout the headers to rename functions
   at the assembly language level.  This is wrapped by the __REDIRECT
   macro, in order to support compilers that can do this some other
   way.  When compilers don't support asm-names at all, we have to do
   preprocessor tricks instead (which don't have exactly the right
   semantics, but it's the best we can do).

   Example:
   int __REDIRECT(setpgrp, (__pid_t pid, __pid_t pgrp), setpgid); */

#if defined __GNUC__ && __GNUC_PREREQ(2,0)

# define __REDIRECT(name, proto, alias) name proto __asm__ (__ASMNAME (#alias))
# define __ASMNAME(cname)  __ASMNAME2 (__USER_LABEL_PREFIX__, cname)
# define __ASMNAME2(prefix, cname) __STRING (prefix) cname

/*
#elif __SOME_OTHER_COMPILER__

# define __REDIRECT(name, proto, alias) name proto; \
	_Pragma("let " #name " = " #alias)
*/
#endif

#ifndef __attribute__
/* GCC has various useful declarations that can be made with the
   `__attribute__' syntax.  All of the ways we use this do fine if
   they are omitted for compilers that don't understand it. */
# if !defined __GNUC__ || !__GNUC_PREREQ(2,0)
#  define __attribute__(xyz)	/* Ignore */
# endif
#endif

#ifndef __attribute_malloc__
/* At some point during the gcc 2.96 development the `malloc' attribute
   for functions was introduced.  We don't want to use it unconditionally
   (although this would be possible) since it generates warnings.  */
# if __GNUC_PREREQ (2,96)
#  define __attribute_malloc__ __attribute__ ((__malloc__))
# else
#  define __attribute_malloc__ /* Ignore */
# endif
#endif

#ifndef __attribute_pure__
/* At some point during the gcc 2.96 development the `pure' attribute
   for functions was introduced.  We don't want to use it unconditionally
   (although this would be possible) since it generates warnings.  */
# if __GNUC_PREREQ (2,96)
#  define __attribute_pure__ __attribute__ ((__pure__))
# else
#  define __attribute_pure__ /* Ignore */
# endif
#endif

#ifndef __attribute_used__
/* At some point during the gcc 3.1 development the `used' attribute
   for functions was introduced.  We don't want to use it unconditionally
   (although this would be possible) since it generates warnings.  */
# if __GNUC_PREREQ (3,1)
#  define __attribute_used__ __attribute__ ((__used__))
# else
#  define __attribute_used__ __attribute__ ((__unused__))
# endif
#endif

#ifndef __attribute_format_arg__
/* At some point during the gcc 2.8 development the `format_arg' attribute
   for functions was introduced.  We don't want to use it unconditionally
   (although this would be possible) since it generates warnings.
   If several `format_arg' attributes are given for the same function, in
   gcc-3.0 and older, all but the last one are ignored.  In newer gccs,
   all designated arguments are considered.  */
# if __GNUC_PREREQ (2,8)
#  define __attribute_format_arg__(x) __attribute__ ((__format_arg__ (x)))
# else
#  define __attribute_format_arg__(x) /* Ignore */
# endif
#endif

#ifndef __attribute_format_strfmon__
/* At some point during the gcc 2.97 development the `strfmon' format
   attribute for functions was introduced.  We don't want to use it
   unconditionally (although this would be possible) since it
   generates warnings.  */
# if __GNUC_PREREQ (2,97)
#  define __attribute_format_strfmon__(a,b) \
   __attribute__ ((__format__ (__strfmon__, a, b)))
# else
#  define __attribute_format_strfmon__(a,b) /* Ignore */
# endif
#endif

#ifndef __extension__
/* It is possible to compile containing GCC extensions even if GCC is
   run in pedantic mode if the uses are carefully marked using the
   `__extension__' keyword.  But this is not generally available before
   version 2.8.  */
# if !__GNUC_PREREQ (2,8)
#  define __extension__		/* Ignore */
# endif
#endif

#ifndef __restrict
/* __restrict is known in EGCS 1.2 and above. */
# if !__GNUC_PREREQ (2,92)
#  define __restrict	/* Ignore */
# endif
#endif

#ifndef __restrict_arr
/* ISO C99 also allows to declare arrays as non-overlapping.  The syntax is
     array_name[restrict]
   GCC 3.1 supports this.  */
# if __GNUC_PREREQ (3,1) && !defined __GNUG__
#  define __restrict_arr	__restrict
# else
#  ifdef __GNUC__
#   define __restrict_arr	/* Not supported in old GCC.  */
#  else
#   if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#    define __restrict_arr	restrict
#   else
/* Some other non-C99 compiler.  */
#    define __restrict_arr	/* Not supported.  */
#   endif
#  endif
# endif
#endif

#ifdef __UNIXLIB_INTERNALS
/* This comes between the return type and function name in
   a function definition to make that definition weak.  */
# define weak_function __attribute__ ((weak))
# define weak_const_function __attribute__ ((weak, __const__))

/* On some platforms we can make internal function calls (i.e., calls of
   functions not exported) a bit faster by using a different calling
   convention.  */
#ifndef internal_function
# define internal_function	/* empty */
#endif

#endif

/* GCC has lots of useful extensions that we can apply to our header
   files.  These aid to improve compile time syntax checking and also
   provide minor optimisations.  */


/* Decide whether we can define 'extern inline' functions in headers.  */
#if __GNUC_PREREQ (2, 7) && defined __OPTIMIZE__ && !defined __OPTIMIZE_SIZE__
# define __USE_EXTERN_INLINES   1
#endif

/* This is not yet supported by our GCC version but used by glibc 2.2.5.  */
#define __builtin_expect(exp, c)  (exp)

/* These macros should be kept in sync with clib.unixlib.s.asm_dec.  */

/* Define this if alloca() should call abort() if no memory can be
   allocated.  */
#define __ALLOCA_FATAL

/* Define if function call references through the dev structure should
   be first checked that their pointer is valid.  */
#undef __PARANOID

/* Define if support for dynamic areas on RISC OS 3.5+ should be compiled
   in.  */
#ifndef __DYNAMIC_AREA
#define __DYNAMIC_AREA 1
#endif

/* Amount to align the wimpslot or dynamic area. */
#define __DA_WIMPSLOT_ALIGNMENT (32*1024-1)

#undef __USEFILEPATH

/* Define if library functions should check certain parameters for their
   validness.  Undefining will remove these consistency checks.  */
#ifndef __INTEGRITY_CHECK
#define __INTEGRITY_CHECK 1
#endif

/* Define if we want to support POSIX interval timers.  */
#ifndef __FEATURE_ITIMERS
#define __FEATURE_ITIMERS 1
#endif

/* Define if we want Unix socket support.  */
#ifndef __FEATURE_SOCKET
#define __FEATURE_SOCKET 1
#endif

/* Define if we want support for pipes.  */
#ifndef __FEATURE_PIPEDEV
#define __FEATURE_PIPEDEV 1
#endif

/* Define for support of /dev/rs423.  */
#ifndef __FEATURE_DEV_RS423
#define __FEATURE_DEV_RS423 1
#endif

/* Don't want any compatibility with Internet 4 */
#undef COMPAT_INET4

/* Define for support for pthreads.  */
#ifndef __FEATURE_PTHREADS
#define __FEATURE_PTHREADS 1
#endif

/* Default to recognising Image filesystems as directories.  Some programs
   may wish to expose them as files for the purpose of compression
   or direct manipulation of the contents.  Set to one in this case.  */
extern int __feature_imagefs_is_file;

#endif
