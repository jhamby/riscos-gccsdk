/* ctype.h

   For use with the GNU compilers and the SharedCLibrary.
   (c) Copyright 1997, 2002, Nick Burrett.  */

#ifndef __CTYPE_H
#define __CTYPE_H

#ifdef __cplusplus
extern "C" {
#endif

extern unsigned char __ctype[];

/* True if c is alphabetic or numberic.  */
extern int isalnum (int __c);
#define isalnum(c) (__ctype[(int) (c)] & 56)

/* True if c is alphabetic.  */
extern int isalpha (int __c);
#define isalpha(c) (__ctype[(int) (c)] & 24)

/* True if c is a control character.  */
extern int iscntrl (int __c);
#define iscntrl(c) (__ctype[(int) (c)] & 64)

/* True if c is a decimal digit.  */
extern int isdigit (int __c);
#define isdigit(c) (__ctype[(int) (c)] & 32)

/* True if c is a printable character other than space.  */
extern int isgraph (int __c);
#define isgraph(c) (__ctype[(int) (c)] & 58)

/* True if c is a lower-case letter.  */
extern int islower (int __c);
#define islower(c) (__ctype[(int) (c)] & 8)

/* True is c is a printable character.  */
extern int isprint (int __c);
#define isprint(c) (__ctype[(int) (c)] & 62)

/* True if c is a printable character other than space
   or an alphanumeric character.  */
extern int ispunct (int __c);
#define ispunct(c) (__ctype[(int) (c)] & 2)

/* True if c is a white space character e.g. space, LF, CR,
   tab or vertical tab.  */
extern int isspace (int __c);
#define isspace(c) (__ctype[(int) (c)] & 1)

/* True if c is an upper-case letter.  */
extern int isupper (int __c);
#define isupper(c) (__ctype[(int) (c)] & 16)

/* True if c is a hexadecimal digit.  */
extern int isxdigit (int __c);
#define isxdigit(c) (__ctype[(int) (c)] & 160)

/* Return the lowercase version of c.  */
extern int tolower (int __c);

/* Return the uppercase version of c.  */
extern int toupper (int __c);

#ifdef __cplusplus
}
#endif

#endif
