/* stdint.h header for the RISC OS SharedCLibrary.
   Copyright (c) 2005 UnixLib Developers
   All rights reserved.
 
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
   3. The name of the author may not be used to endorse or promote products
      derived from this software without specific prior written permission.
 
   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
   IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
   IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
   NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
   THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */


/* Modified from UnixLib's stdint.h */


#ifndef __STDINT_H
#define __STDINT_H 1

#include <sys/types.h>

/* Exact integral types.  */

/* Signed.  */

#ifndef __int8_t_defined
#define __int8_t_defined
typedef signed char int8_t;
typedef short int int16_t;
typedef int int32_t;
__extension__
typedef long long int int64_t;
#endif

/* Unsigned.  */
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
__extension__
typedef unsigned long long int uint64_t;


/* Small types.  */

/* Signed.  */
typedef signed char int_least8_t;
typedef short int int_least16_t;
typedef int int_least32_t;
__extension__
typedef long long int int_least64_t;

/* Unsigned.  */
typedef unsigned char uint_least8_t;
typedef unsigned short int uint_least16_t;
typedef unsigned int uint_least32_t;
__extension__
typedef unsigned long long int uint_least64_t;


/* Fast types.  */

/* Signed.  */
typedef signed char int_fast8_t;
typedef int int_fast16_t;
typedef int int_fast32_t;
__extension__
typedef long long int int_fast64_t;

/* Unsigned.  */
typedef unsigned char uint_fast8_t;
typedef unsigned int uint_fast16_t;
typedef unsigned int uint_fast32_t;
__extension__
typedef unsigned long long int uint_fast64_t;


/* Types for `void *' pointers.  */
#ifndef __intptr_t_defined
#define __intptr_t_defined
typedef int intptr_t;
#endif
typedef unsigned int uintptr_t;


#ifdef __GNUC__
/* Largest integral types.  */
__extension__
typedef long long int intmax_t;
__extension__
typedef unsigned long long int uintmax_t;
#else
/* Largest integral types.  */
typedef long int intmax_t;
typedef unsigned long int uintmax_t;
#endif

/* The ISO C 9X standard specifies that in C++ implementations these
   macros should only be defined if explicitly requested.  */
#if !defined __cplusplus || defined __STDC_LIMIT_MACROS

/* Limits of integral types.  */

/* Minimum of signed integral types.  */
#define INT8_MIN		(-128)
#define INT16_MIN		(-32767-1)
#define INT32_MIN		(-2147483647-1)
#define INT64_MIN		(-9223372036854775807LL-1)

/* Maximum of signed integral types.  */
#define INT8_MAX		(127)
#define INT16_MAX		(32767)
#define INT32_MAX		(2147483647)
#define INT64_MAX		(9223372036854775807LL)

/* Maximum of unsigned integral types.  */
#define UINT8_MAX		(255U)
#define UINT16_MAX		(65535U)
#define UINT32_MAX		(4294967295U)
#define UINT64_MAX		(18446744073709551615ULL)


/* Minimum of signed integral types having a minimum size.  */
#define INT_LEAST8_MIN		(-128)
#define INT_LEAST16_MIN	(-32767-1)
#define INT_LEAST32_MIN	(-2147483647-1)
#define INT_LEAST64_MIN	(-9223372036854775807LL-1)

/* Maximum of signed integral types having a minimum size.  */
#define INT_LEAST8_MAX		(127)
#define INT_LEAST16_MAX	(32767)
#define INT_LEAST32_MAX	(2147483647)
#define INT_LEAST64_MAX	(9223372036854775807LL)

/* Maximum of unsigned integral types having a minimum size.  */
#define UINT_LEAST8_MAX	(255U)
#define UINT_LEAST16_MAX	(65535U)
#define UINT_LEAST32_MAX	(4294967295U)
#define UINT_LEAST64_MAX	(18446744073709551615ULL)

/* Minimum of fast signed integral types having a minimum size.  */
#define INT_FAST8_MIN		(-128)
#define INT_FAST16_MIN		(-2147483647-1)
#define INT_FAST32_MIN		(-2147483647-1)
#define INT_FAST64_MIN		(-9223372036854775807LL-1)

/* Maximum of fast signed integral types having a minimum size.  */
#define INT_FAST8_MAX		(127)
#define INT_FAST16_MAX		(2147483647)
#define INT_FAST32_MAX		(2147483647)
#define INT_FAST64_MAX		(9223372036854775807LL)

/* Maximum of fast unsigned integral types having a minimum size.  */
#define UINT_FAST8_MAX		(255U)
#define UINT_FAST16_MAX	(4294967295U)
#define UINT_FAST32_MAX	(4294967295U)
#define UINT_FAST64_MAX	(18446744073709551615ULL)


/* Values to test for integral types holding `void *' pointer.  */
#define INTPTR_MIN		(-2147483647-1)
#define INTPTR_MAX		(2147483647)
#define UINTPTR_MAX		(4294967295U)


/* Minimum for largest signed integral type.  */
#define INTMAX_MIN		(-9223372036854775807LL-1)
/* Maximum for largest signed integral type.  */
#define INTMAX_MAX		(9223372036854775807LL)

/* Maximum for largest unsigned integral type.  */
#define UINTMAX_MAX		(18446744073709551615ULL)

/* Limits of other integer types.  */

/* Limits of `ptrdiff_t' type.  */
#define PTRDIFF_MIN	(-2147483647-1)
#define PTRDIFF_MAX	(2147483647)

/* Limits of `sig_atomic_t'.  */
#define SIG_ATOMIC_MIN	(-2147483647-1)
#define SIG_ATOMIC_MAX	(2147483647)

/* Limit of `size_t' type.  */
#define SIZE_MAX	(4294967295U)

/* Limits of `wchar_t'.  */
#ifndef WCHAR_MIN
/* These constants might also be defined in <wchar.h>.  */
#define WCHAR_MIN	(-2147483647-1)
#define WCHAR_MAX	(2147483647)
#endif

/* Limits of `wint_t'.  */
#define WINT_MIN	(0)
#define WINT_MAX	(4294967295U)

#endif	/* C++ && limit macros */

#if !defined __cplusplus || defined __STDC_CONSTANT_MACROS

/* Signed.  */
#define INT8_C(c)	c
#define INT16_C(c)	c
#define INT32_C(c)	c
#define INT64_C(c)	c ## LL

/* Unsigned.  */
#define UINT8_C(c)	c ## U
#define UINT16_C(c)	c ## U
#define UINT32_C(c)	c ## U
#define UINT64_C(c)	c ## ULL

/* Maximal type.  */
#define INTMAX_C(c)	c ## LL
#define UINTMAX_C(c)	c ## ULL

#endif

#endif
