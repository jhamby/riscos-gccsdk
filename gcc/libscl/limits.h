/* limits.h

   For use with the GNU compilers and the SharedCLibrary.
   (c) Copyright 1997, Nick Burrett.  */

#ifndef __LIMITS_H
#define __LIMITS_H

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
#define SHRT_MIN	(short)(0x8000U)

/* Maximum values that can be represented by a 'signed short int'
   and 'unsigned short int', respectively.  */
#define SHRT_MAX	0x7fff
#define USHRT_MAX	0xffffU

/* Minimum value that can be represented by a 'signed int'.  */
#define INT_MIN 	(int)(0x80000000U)

/* Maximum values that can be represented by a 'signed int'
   and 'unsigned int'.  */
#define INT_MAX 	0x7fffffff
#define UINT_MAX	0xffffffffU

/* The number of bits in a 'long int'.  */
#define LONGBITS 32

/* Minimum value that can be represented by a 'signed long int'.  */
#define LONG_MIN	(long)(0x80000000UL)

/* Maximum values that can be represented by a 'signed long int'
   and 'unsigned long int'.  */
#define LONG_MAX	0x7fffffffL
#define ULONG_MAX	0xffffffffUL

#ifdef __GNUC__
/* For GNU C 'long long' compatibility only.  */

/* The minimum value that can be represented by a
   'signed long long int'.  */
#define LONG_LONG_MIN	0x8000000000000000LL

/* The maximum values that can be represented by a
   'signed long long int' and 'unsigned long long int'.  */
#define LONG_LONG_MAX	0x7fffffffffffffffLL
#define ULONG_LONG_MAX	0xffffffffffffffffULL

#endif

#endif
