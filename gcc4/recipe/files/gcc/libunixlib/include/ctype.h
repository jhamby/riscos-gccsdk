/*
 * ANSI Standard 4.3: Character Handling <ctype.h>
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#ifndef __CTYPE_H
#define __CTYPE_H

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

__BEGIN_DECLS

/* c is alphabetic or numeric.  */
extern int isalnum (int __c) __THROW;

/* c is alphabetic.  */
extern int isalpha (int __c) __THROW;

/* c is a control character.  */
extern int iscntrl (int __c) __THROW;

/* c is a decimal digit.  */
extern int isdigit (int __c) __THROW;

/* c is any printable character other than a space. */
extern int isgraph (int __c) __THROW;

/* c is a lower-case letter.  */
extern int islower (int __c) __THROW;

/* c is an upper-case letter.  */
extern int isupper (int __c) __THROW;

/* c is a printable character.  */
extern int isprint (int __c) __THROW;

/* c is a printable character other than a space or a
   alphanumeric character.  */
extern int ispunct (int __c) __THROW;

/* c is a white space character e.g. space, newline, tab, linefeed,
   return, vertical tab.  */
extern int isspace (int __c) __THROW;

/* c is a hex digit.  */
extern int isxdigit (int __c) __THROW;

#ifndef __TARGET_SCL__
/* Characteristics. */
extern const unsigned char * const __ctype;
/* Lower case table.  */
extern const short * const __ctype_lower;
/* Upper case table.  */
extern const short * const __ctype_upper;

/* Based on Territory_CharacterPropertyTable.  */
#  define ___ctype_ctrl 1 /* control code */
#  define ___ctype_upper 2 /* upper case */
#  define ___ctype_lower 4 /* lower case */
#  define ___ctype_alpha 8 /* alphabetic */
#  define ___ctype_punc 16 /* punctuation */
#  define ___ctype_white 32 /* white space */
#  define ___ctype_digit 64 /* digit */
#  define ___ctype_xdigit 128 /* hex digit */

#  define isalnum(c) (__ctype[(int) (c)] & (___ctype_alpha | ___ctype_digit))
#  define isalpha(c) (__ctype[(int) (c)] & ___ctype_alpha)
#  define iscntrl(c) (__ctype[(int) (c)] & ___ctype_ctrl)
#  define isdigit(c) (__ctype[(int) (c)] & ___ctype_digit)
#  define isgraph(c) (__ctype[(int) (c)] & ~(___ctype_white | ___ctype_ctrl))
#  define islower(c) (__ctype[(int) (c)] & ___ctype_lower)
#  define isupper(c) (__ctype[(int) (c)] & ___ctype_upper)
#  define isprint(c) ((__ctype[(int) (c)] & (___ctype_upper | ___ctype_lower | ___ctype_digit | ___ctype_punc | ___ctype_white)) && !(__ctype[(int) (c)] & ___ctype_ctrl))
#  define ispunct(c) (__ctype[(int) (c)] & ___ctype_punc)
#  define isspace(c) (__ctype[(int) (c)] & ___ctype_white)
#  define isxdigit(c) (__ctype[(int) (c)] & ___ctype_xdigit)
#else
/* Characteristics. */
#  ifdef __TARGET_MODULE__
/* As __ctype is a non-const .bss array, we can not declare it const for
   module code.  */
extern unsigned char __ctype[];
#  else
/* We can safely make this a const array.  */
extern const unsigned char __ctype[];
#  endif

#  define isspace(c) (__ctype[(int) (c)] & 1)
#  define ispunct(c) (__ctype[(int) (c)] & 2)
#  define islower(c) (__ctype[(int) (c)] & 8)
#  define isupper(c) (__ctype[(int) (c)] & 16)
#  define isdigit(c) (__ctype[(int) (c)] & 32)
#  define iscntrl(c) (__ctype[(int) (c)] & 64)
#  define isxdigit(c) (__ctype[(int) (c)] & (32 | 128))
#  define isalpha(c) (__ctype[(int) (c)] & (8 | 16))
#  define isalnum(c) (__ctype[(int) (c)] & (8 | 16 | 32))
#  define isgraph(c) (__ctype[(int) (c)] & (2 | 8 | 16 | 32))
#  define isprint(c) (__ctype[(int) (c)] & (2 | 4 | 8 | 16 | 32))
#endif

/* Convert c to upper case.  */
extern int toupper (int __c) __THROW;
#ifndef __TARGET_SCL__
#  define toupper(c) ((int) __ctype_upper[(int) (c)])
#endif

/* Convert c to lower case.  */
extern int tolower (int __c) __THROW;
#ifndef __TARGET_SCL__
#  define tolower(c) ((int) __ctype_lower[(int) (c)])
#endif

#ifndef __TARGET_SCL__
/* Convert c to upper case.  */
extern int _toupper (int __c) __THROW;
#  define _toupper(c) ((int) __ctype_upper[(int) (c)])

/* Convert c to lower case.  */
extern int _tolower (int __c) __THROW;
#  define _tolower(c) ((int) __ctype_lower[(int) (c)])

#  if defined __USE_SVID || defined __USE_MISC || defined __USE_XOPEN
/* System V extension functions.  */

/* Convert c to an ASCII character.  */
extern int toascii (int __c) __THROW;
#    define toascii(c) ((c) & 0x7f)

/* Is c an ASCII character.  */
extern int isascii (int __c) __THROW;
#    define isascii(c) ((unsigned)(c) <= 0x7f)
#  endif
#endif

__END_DECLS

#endif
