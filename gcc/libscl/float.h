/* float.h standard header for the RISC OS SharedCLibrary.
   Copyright (c) 1997-2005 Nick Burrett
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

#ifndef __FLOAT_H
#define __FLOAT_H

/* Value of the base, or radix, of exponent representation.  */
#define FLT_RADIX 2

/* Characterizes the rounding mode for floating point addition.
   -1 = the mode is indeterminable.
   0 = rounding is towards zero.
   1 = rounding is to the nearest number.
   2 = rounding is towards positive infinity. */
#define FLT_ROUNDS 1

/* Number of base-'FLT_RADIX' digits in the floating point mantissa
   for the float data type.  */
#define FLT_MANT_DIG 24

/* Number of base-'FLT_RADIX' digits in the floating point mantissa
   for the double data type.  */
#define DBL_MANT_DIG 53

/* Number of base-'FLT_RADIX' digits in the floating point mantissa
   for the long double data type.  */
#define LDBL_MANT_DIG 53

/* Number of decimal digits of precision for the float data type.  */
#define FLT_DIG 6

/* Number of decimal digits of precision for the double data type.  */
#define DBL_DIG 15

/* Number of decimal digits of precision for the long double data type.  */
#define LDBL_DIG 15

/* Smallest possible exponent value for type float.  */
#define FLT_MIN_EXP (-125)

/* Smallest possible exponent value for type double.  */
#define DBL_MIN_EXP (-1021)

/* Smallest possible exponent value for type long double.  */
#define LDBL_MIN_EXP (-1021)

/* Minimum negative integer such that 10 raised to this power
   minus 1 can be represented as a normalized floating point
   number of type float.  */
#define FLT_MIN_10_EXP (-37)

/* Minimum negative integer such that 10 raised to this power
   minus 1 can be represented as a normalized floating point
   number of type double.  */
#define DBL_MIN_10_EXP (-307)

/* Minimum negative integer such that 10 raised to this power
   minus 1 can be represented as a normalized floating point
   number of type long double.  */
#define LDBL_MIN_10_EXP (-307)

/* Largest possible exponent value for type float.  */
#define FLT_MAX_EXP 128

/* Largest possible exponent value for type double.  */
#define DBL_MAX_EXP 1024

/* Largest possible exponent value for type long double.  */
#define LDBL_MAX_EXP 1024

/* Maximum positive integer such that 10 raised to this power
   minus 1 can be represented as a normalized floating point
   number of type long double.  */
#define FLT_MAX_10_EXP 38

/* Maximum positive integer such that 10 raised to this power
   minus 1 can be represented as a normalized floating point
   number of type long double.  */
#define DBL_MAX_10_EXP 308

/* Maximum positive integer such that 10 raised to this power
   minus 1 can be represented as a normalized floating point
   number of type long double.  */
#define LDBL_MAX_10_EXP 308

/* Maximum number representable in type float.  */
#define FLT_MAX 3.40282347e+38F
/* Maximum number representable in type double.  */
#define DBL_MAX 1.79769313486231571e+308
/* Maximum number representable in type long double.  */
#define LDBL_MAX 1.79769313486231571e+308L

/* Minimum positive floating point number of type float
   such that '1.0 + FLT_EPSILON != 1.0' is true.  */
#define FLT_EPSILON 1.19209290e-7F

/* Minimum positive floating point number of type double
   such that '1.0 + DBL_EPSILON != 1.0' is true.  */
#define DBL_EPSILON 2.2204460492503131e-16

/* Minimum positive floating point number of type long double
   such that '1.0 + DBL_EPSILON != 1.0' is true.  */
#define LDBL_EPSILON 2.2204460492503131e-16L

/* Minimum number representable in type float.  */
#define FLT_MIN 1.17549435e-38F
/* Minimum number representable in type double.  */
#define DBL_MIN 2.22507385850720138e-308
/* Minimum number representable in type long double.  */
#define LDBL_MIN 2.22507385850720138e-308L

#endif
