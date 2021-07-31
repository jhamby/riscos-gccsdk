/*
 * ANSI Standard 4.14/2.2.4.2: Limits of integral types <limits.h>
 * Copyright (c) 2000-2013 UnixLib Developers
 */

#ifndef __LIMITS_H
#define __LIMITS_H

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

/* Number of bits in a 'char'.  */
#define CHAR_BIT	8
/* Minimum value that can be represented by a 'signed char'.  */
#define SCHAR_MIN	(-0x80)
/* Maximum values that can be represented by a 'signed char'
   and 'unsigned char', respectively.  */
#define SCHAR_MAX	0x7f
#define UCHAR_MAX	0xff
/* Minimum and maximum values that can be represented by a 'char'.  */
#define CHAR_MIN	0x00
#define CHAR_MAX	0xff

/* Maximum length of a multibyte character.  */
#define MB_LEN_MAX	1

/* Minimum value that can be represented by a 'signed short int'.  */
#define SHRT_MIN	(-32768)
/* Maximum values that can be represented by a 'signed short int'
   and 'unsigned short int', respectively.  */
#define SHRT_MAX	0x7fff
#define USHRT_MAX	0xffffU

/* Minimum value that can be represented by a 'signed int'.  */
#define INT_MIN 	(-INT_MAX - 1)
/* Maximum values that can be represented by a 'signed int'
   and 'unsigned int'.  */
#define INT_MAX 	0x7fffffff
#define UINT_MAX	0xffffffffU

/* The number of bits in a 'long int'.  */
#define LONGBITS 32
/* Minimum value that can be represented by a 'signed long int'.  */
#define LONG_MIN	(-LONG_MAX - 1L)
/* Maximum values that can be represented by a 'signed long int'
   and 'unsigned long int'.  */
#define LONG_MAX	0x7fffffffL
#define ULONG_MAX	0xffffffffUL

/* The minimum value that can be represented by a
   'signed long long int'.  */
#define LONG_LONG_MIN (-LONG_LONG_MAX - 1LL)

/* The maximum values that can be represented by a
   'signed long long int' and 'unsigned long long int'.  */
#define LONG_LONG_MAX 0x7fffffffffffffffLL
#define ULONG_LONG_MAX 0xffffffffffffffffULL

#ifdef __USE_ISOC99
/* Minimum and maximum that a signed long long int can hold.  */
#define LLONG_MIN 0x8000000000000000LL
#define LLONG_MAX 0x7fffffffffffffffLL

/* Maximum value that an unsigned long long int can hold.  */
#define ULLONG_MAX 0xffffffffffffffffULL
#endif

#define QUAD_MAX LONG_LONG_MAX

#ifdef __USE_XOPEN
/* Number of bits in a word of type 'int'.  */
#define WORD_BIT 32

/* Number of bits in a word of type 'long'.  */
#define LONG_BIT 32
#endif

/* POSIX Standard 2.9.2: Minimum Values <limits.h>.  */

#ifdef __USE_POSIX
/* These are the standard-mandated minimum values.  */

/* Maximum length of arguments to `execve', including environment.  */
#define	_POSIX_ARG_MAX		1024
#define ARG_MAX _POSIX_ARG_MAX

/* Maximum simultaneous processes per real user ID.  */
#define	_POSIX_CHILD_MAX	1
#define CHILD_MAX _POSIX_CHILD_MAX

/* Maximum link count of a file.  */
#define	_POSIX_LINK_MAX		1
#define LINK_MAX _POSIX_LINK_MAX

/* Number of bytes in a terminal canonical input queue.  */
#define	_POSIX_MAX_CANON	256
#define MAX_CANON _POSIX_MAX_CANON

/* Number of bytes for which space will be
   available in a terminal input queue.  */
#define	_POSIX_MAX_INPUT	256
#define MAX_INPUT _POSIX_MAX_INPUT

/* Number of simultaneous supplementary group IDs per process.  */
#define	_POSIX_NGROUPS_MAX	0
#define NGROUPS_MAX _POSIX_NGROUPS_MAX

/* Number of files one process can have open at once.
   Keep in sync with <stdio.h>, FOPEN_MAX.  */
#ifdef __TARGET_SCL__
#  define _POSIX_OPEN_MAX	8
#else
#  define _POSIX_OPEN_MAX	64
#endif
#define OPEN_MAX _POSIX_OPEN_MAX

/* Number of bytes in a filename.
   Keep in sync with <stdio.h>, FILENAME_MAX.  */
#ifdef __TARGET_SCL__
#  define _POSIX_NAME_MAX	80
#else
#  define _POSIX_NAME_MAX	252
#endif
#define NAME_MAX _POSIX_NAME_MAX

/* Number of bytes in a pathname.  */
#define	_POSIX_PATH_MAX		1024
#define PATH_MAX _POSIX_PATH_MAX

/* Number of bytes than can be written atomically to a pipe.  */
#define	_POSIX_PIPE_BUF		512
#define PIPE_BUF _POSIX_PIPE_BUF

/* Largest value of a `ssize_t'.  */
#define	_POSIX_SSIZE_MAX	INT_MAX
#define SSIZE_MAX _POSIX_SSIZE_MAX

/* Number of streams a process can have open at once.  */
#define	_POSIX_STREAM_MAX	8
#define STREAM_MAX _POSIX_STREAM_MAX

/* Maximum length of a timezone name (element of `tzname').  */
#define	_POSIX_TZNAME_MAX	3
#define TZNAME_MAX _POSIX_TZNAME_MAX
#endif

/* POSIX2 limits.  */

#ifdef __USE_POSIX2
/* The maximum `ibase' and `obase' values allowed by the `bc' utility.  */
#define	_POSIX2_BC_BASE_MAX	99
#define	BC_BASE_MAX	_POSIX2_BC_BASE_MAX

/* The maximum number of elements allowed in an array by the `bc' utility.  */
#define	_POSIX2_BC_DIM_MAX	2048
#define	BC_DIM_MAX	_POSIX2_BC_DIM_MAX

/* The maximum `scale' value allowed by the `bc' utility.  */
#define	_POSIX2_BC_SCALE_MAX	99
#define	BC_SCALE_MAX	_POSIX2_BC_SCALE_MAX

/* The maximum length of a string constant accepted by the `bc' utility.  */
#define	_POSIX2_BC_STRING_MAX	1000
#define	BC_STRING_MAX	_POSIX2_BC_STRING_MAX

/* The maximum number of weights that can be assigned to an entry of
   the LC_COLLATE category `order' keyword in a locale definition.  */
#define	_POSIX2_EQUIV_CLASS_MAX	2
#define	EQUIV_CLASS_MAX	_POSIX2_EQUIV_CLASS_MAX

/* The maximum number of expressions that can be nested
   within parentheses by the `expr' utility.  */
#define	_POSIX2_EXPR_NEST_MAX	32
#define	EXPR_NEST_MAX	_POSIX2_EXPR_NEST_MAX

/* The maximum length, in bytes, of an input line.  */
#define	_POSIX2_LINE_MAX	2048
#define	LINE_MAX	_POSIX2_LINE_MAX

/* The maximum number of repeated occurrences of a regular expression
   permitted when using the interval notation `\{M,N\}'.  */
#define	_POSIX2_RE_DUP_MAX	255

/* This value is defined like this in regex.h.  */
#define	RE_DUP_MAX	(0x7fff)
#endif

#endif
