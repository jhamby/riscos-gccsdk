/*
 * ANSI Standard 4.3: Character Handling <ctype.h>
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#ifndef __CTYPE_H
#define __CTYPE_H

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

__BEGIN_DECLS

/* Characteristics. */
extern unsigned char *__ctype;
/* Lower case table.  */
extern short *__ctype_lower;
/* Upper case table.  */
extern short *__ctype_upper;


#define ___ctype_ctrl 1 /* control code */
#define ___ctype_upper 2 /* upper case */
#define ___ctype_lower 4 /* lower case */
#define ___ctype_alpha 8 /* alphabetic */
#define ___ctype_punc 16 /* punctuation */
#define ___ctype_white 32 /* white space */
#define ___ctype_digit 64 /* digit */
#define ___ctype_xdigit 128 /* hex digit */

/* c is alphabetic or numeric.  */
extern int isalnum (int __c) __THROW;
#define isalnum(c) (__ctype[(int) (c)] & (___ctype_alpha | ___ctype_digit))

/* c is alphabetic.  */
extern int isalpha (int __c) __THROW;
#define isalpha(c) (__ctype[(int) (c)] & ___ctype_alpha)

/* c is a control character.  */
extern int iscntrl (int __c) __THROW;
#define iscntrl(c) (__ctype[(int) (c)] & ___ctype_ctrl)

/* c is a decimal digit.  */
extern int isdigit (int __c) __THROW;
#define isdigit(c) (__ctype[(int) (c)] & ___ctype_digit)

/* c is any printable character other than a space. */
extern int isgraph (int __c) __THROW;
#define isgraph(c) (__ctype[(int) (c)] & ~(___ctype_white | ___ctype_ctrl))

/* c is a lower-case letter.  */
extern int islower (int __c) __THROW;
#define islower(c) (__ctype[(int) (c)] & ___ctype_lower)

/* c is an upper-case letter.  */
extern int isupper (int __c) __THROW;
#define isupper(c) (__ctype[(int) (c)] & ___ctype_upper)

/* c is a printable character.  */
extern int isprint (int __c) __THROW;
#define isprint(c) (~__ctype[(int) (c)] & ___ctype_ctrl)

/* c is a printable character other than a space or a
   alphanumeric character.  */
extern int ispunct (int __c) __THROW;
#define ispunct(c) (__ctype[(int) (c)] & ___ctype_punc)

/* c is a white space character e.g. space, newline, tab, linefeed,
   return, vertical tab.  */
extern int isspace (int __c) __THROW;
#define isspace(c) (__ctype[(int) (c)] & ___ctype_white)

/* c is a hex digit.  */
extern int isxdigit (int __c) __THROW;
#define isxdigit(c) (__ctype[(int) (c)] & ___ctype_xdigit)

/* Convert c to upper case.  */
extern int toupper (int __c) __THROW;
#define toupper(c) ((int) __ctype_upper[(int) (c)])

/* Convert c to lower case.  */
extern int tolower (int __c) __THROW;
#define tolower(c) ((int) __ctype_lower[(int) (c)])

/* Convert c to upper case.  */
extern int _toupper (int __c) __THROW;
#define _toupper(c) ((int) __ctype_upper[(int) (c)])

/* Convert c to lower case.  */
extern int _tolower (int __c) __THROW;
#define _tolower(c) ((int) __ctype_lower[(int) (c)])


#if defined __USE_SVID || defined __USE_MISC || defined __USE_XOPEN
/* System V extension functions.  */

/* Convert c to an ASCII character.  */
extern int toascii (int __c) __THROW;
#define toascii(c) ((c) & 0x7f)

/* Is c an ASCII character.  */
extern int isascii (int __c) __THROW;
#define isascii(c) ((unsigned)(c) <= 0x7f)
#endif

__END_DECLS

#endif
