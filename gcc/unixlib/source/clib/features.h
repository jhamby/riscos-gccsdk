/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/features.h,v $
 * $Date: 2005/04/07 18:46:23 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

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

#ifdef __UNIXLIB_INTERNALS
# define __GNU_LIBRARY__
#endif

/* Always use ISO C things.  */
#define	__USE_ANSI	1


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
# define __USE_POSIX	1
#endif

#if defined _POSIX_C_SOURCE && _POSIX_C_SOURCE >= 2 || defined _XOPEN_SOURCE
# define __USE_POSIX2	1
#endif

#if (_POSIX_C_SOURCE - 0) >= 199309L
# define __USE_POSIX199309	1
#endif

#if (_POSIX_C_SOURCE - 0) >= 199506L
# define __USE_POSIX199506	1
#endif

#if (_POSIX_C_SOURCE - 0) >= 200112L
# define __USE_XOPEN2K		1
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

#if 0
/* wchar_t uses ISO 10646-1 (2nd ed., published 2000-09-15) / Unicode 3.0.  */
//#define __STDC_ISO_10646__              200009L
#endif

/* Major and minor version number of the UnixLib C library package.  Use
   these macros to test for features in specific releases.  */
#define __ULIBC__       4
#define __ULIBC_MINOR__ 0

/* Convenience macros to test the versions of glibc and gcc.
   Use them like this:
   #if __GNUC_PREREQ (2,8)
   ... code requiring gcc 2.8 or later ...
   #endif
   Note - they won't work for gcc1 or glibc1, since the _MINOR macros
   were not defined then.  */
#if defined __GNUC__ && defined __GNUC_MINOR__
# define __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#else
# define __GNUC_PREREQ(maj, min) 0
#endif

#define __ULIBC_PREREQ(maj, min) \
	((__ULIBC__ << 16) + __ULIBC_MINOR__ >= ((maj) << 16) + (min))

/* Decide whether a compiler supports the long long datatypes.  */
#if defined __GNUC__
# define __GLIBC_HAVE_LONG_LONG	1
#endif

/* This is here only because every header file already includes this one.  */
#ifndef __ASSEMBLER__
# ifndef _SYS_CDEFS_H
#  include <sys/cdefs.h>
# endif

/* If we don't have __REDIRECT, prototypes will be missing if
   __USE_FILE_OFFSET64 but not __USE_LARGEFILE[64]. */
# if defined __USE_FILE_OFFSET64 && !defined __REDIRECT
#  define __USE_LARGEFILE	1
#  define __USE_LARGEFILE64	1
# endif

#endif	/* !ASSEMBLER */

/* Decide whether we can define 'extern inline' functions in headers.  */
#if __GNUC_PREREQ (2, 7) && defined __OPTIMIZE__ \
    && !defined __OPTIMIZE_SIZE__ && !defined __NO_INLINE__
# define __USE_EXTERN_INLINES	1
#endif


/* This is here only because every header file already includes this one.
   Get the definitions of all the appropriate `__stub_FUNCTION' symbols.
   <gnu/stubs.h> contains `#define __stub_FUNCTION' when FUNCTION is a stub
   that will always return failure (and set errno to ENOSYS).  */
#include <unixlib/stubs.h>

/* Default to recognising Image filesystems as directories.  Some programs
   may wish to expose them as files for the purpose of compression
   or direct manipulation of the contents.  Set to one in this case.  */
extern int __feature_imagefs_is_file; /* Note: this is a weak symbol.  */

#ifdef __UNIXLIB_INTERNALS

/* Include the UnixLib build options.  */
#include <unixlib/stdcmac.h>
#include <unixlib/buildoptions.h>

/* Amount to align the wimpslot or dynamic area. */
#define __DA_WIMPSLOT_ALIGNMENT (32*1024-1)

#ifndef __GNUC__
#define __builtin_expect(exp, c)  (exp)
#endif

/* Gets the __feature_imagefs_is_file value which can be defined by
   the global variable __feature_imagefs_is_file in the user program.
   Returns a copy of __feature_imagefs_is_file_internal (its default
   value is 0) when __feature_imagefs_is_file is not defined.  */
extern int __get_feature_imagefs_is_file (void);
#if 0
/* Currently the code __set_feature_imagefs_is_file commented out because
   we don't need it in UnixLib internally.  */

/* Sets the __feature_imagefs_is_file value when it's defined.
   Otherwise __feature_imagefs_is_file_internal gets written.  */
extern void __set_feature_imagefs_is_file (int __value);
#endif

#ifndef __ELF__

/* NULL if user didn't specify __program_name; non-NULL otherwise and then
   its value equals __program_name.

   These symbols are only required for compatibility with Norcroft CC
   and AOF/GCC.  */
extern const char * const * const ___program_name;
extern const char * const * const ___dynamic_da_name;
#endif

#endif  /* __UNIXLIB_INTERNALS */

/* Note that AOF/GCC and Norcroft CC will ignore the WEAK attribute
   here.  ELF/GCC will correctly handle it and do the right thing.  */
#ifdef __ELF__
#define __attribute_weak__ __attribute__ ((__weak__))
#else
#define __attribute_weak__ /**/
#endif

/* When defined, specifies the <program name> part of the UnixLib OS
   variables.  Otherwise, the leaf filename part of argv[0] is used.

   Note: this is a weak symbol.  */
extern const char * const __program_name __attribute_weak__;

/* When defined, the memory pool will be created in the WimpSlot area
   instead of a dynamic area (on RISC OS versions supporting dynamic
   areas). Its value is unimportant.

   Note: this is a weak symbol.  */
extern int __dynamic_no_da __attribute_weak__;

/* When a dynamic area is created as memory pool, __dynamic_da_name can
   be used to specify its name.  When this variable is not defined, the
   dynamic area name will be <program name> + "$Heap".

   Note: this is a weak symbol.  */
extern const char * const __dynamic_da_name __attribute_weak__;

/* When defined, indicates the maximum size in bytes of the dynamic area
   used for the heap. If undefined, UnixLib defaults to 32MB.

   Note: this is a weak symbol.  */
extern int __dynamic_da_max_size __attribute_weak__;

#undef __attribute_weak__

#ifndef __SYS_CDEFS_H
# include <sys/cdefs.h>
#endif

#endif
