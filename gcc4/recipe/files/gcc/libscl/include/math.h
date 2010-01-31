/* math.h standard header for the RISC OS SharedCLibrary.
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

#ifndef __MATH_H
#define __MATH_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef HUGE_VAL
#define HUGE_VAL __huge_val
extern const double HUGE_VAL;
#endif

/* GCC has various useful declarations that can be made with the
   __attribute__ syntax.  Disable its use for other compilers.  */
#ifndef __GNUC__
#ifndef __attribute__
#define __attribute__(x) /* Ignore.  */
#endif
#endif

/* Trigonometric functions.  */

/* Arc cosine of x.  */
extern double acos (double __x) __attribute__ ((__const__));
extern float acosf (float __x) __attribute__ ((__const__));
extern long double acosl (long double __x) __attribute__ ((__const__));

/* Arc sine of x.  */
extern double asin (double __x) __attribute__ ((__const__));
extern float asinf (float __x) __attribute__ ((__const__));
extern long double asinl (long double __x) __attribute__ ((__const__));

/* Arc tangent of x.  */
extern double atan (double __x) __attribute__ ((__const__));
extern float atanf (float __x) __attribute__ ((__const__));
extern long double atanl (long double __x) __attribute__ ((__const__));

/* Arc tangent of y/x.  */
extern double atan2 (double __y, double __x) __attribute__ ((__const__));
extern float atan2f (float __y, float __x) __attribute__ ((__const__));
extern long double atan2l (long double __y,
                           long double __x) __attribute__ ((__const__));

/* Cosine of x.  */
extern double cos (double __x) __attribute__ ((__const__));
extern float cosf (float __x) __attribute__ ((__const__));
extern long double cosl (long double __x) __attribute__ ((__const__));

/* Sine of x.  */
extern double sin (double __x) __attribute__ ((__const__));
extern float sinf (float __x) __attribute__ ((__const__));
extern long double sinl (long double __x) __attribute__ ((__const__));

/* Tangent of x.  */
extern double tan (double __x) __attribute__ ((__const__));
extern float tanf (float __x) __attribute__ ((__const__));
extern long double tanl (long double __x) __attribute__ ((__const__));

/* Hyperbolic functions.  */

/* Hyperbolic cosine of x.  */
extern double cosh (double __x) __attribute__ ((__const__));
extern float coshf (float __x) __attribute__ ((__const__));
extern long double coshl (long double __x) __attribute__ ((__const__));

/* Hyperbolic sine of x.  */
extern double sinh (double __x) __attribute__ ((__const__));
extern float sinhf (float __x) __attribute__ ((__const__));
extern long double sinhl (long double __x) __attribute__ ((__const__));

/* Hyperbolic tangent of x.  */
extern double tanh (double __x) __attribute__ ((__const__));
extern float tanhf (float __x) __attribute__ ((__const__));
extern long double tanhl (long double __x) __attribute__ ((__const__));

/* Hyperbolic arc cosine of x.  */
extern double acosh (double __x) __attribute__ ((__const__));

/* Hyperbolic arc sine of x.  */
extern double asinh (double __x) __attribute__ ((__const__));

/* Hyperbolic arc tangent of x.  */
extern double atanh (double __x) __attribute__ ((__const__));

/* Exponential and logarithmic functions.  */

/* Exponentional function of x (2 ^ e).  */
extern double exp (double __x) __attribute__ ((__const__));
extern float expf (float __x) __attribute__ ((__const__));
extern long double expl (long double __x) __attribute__ ((__const__));

/* Exponentional function of x (2 ^ x).  */
extern double exp2 (double __x) __attribute__ ((__const__));

/* Exponentional function of x (10 ^ x).  */
extern double exp10 (double __x) __attribute__ ((__const__));

/* Break value into a normalized fracton and an integral power of 2.  */
extern double frexp (double value, int *__exp);
extern float frexpf (float value, int *__exp);
extern long double frexpl (long double value, int *__exp);

/* x times (two to the exp power).  */
extern double ldexp (double __x, int __exp) __attribute__ ((__const__));
extern float ldexpf (float __x, int __exp) __attribute__ ((__const__));
extern long double ldexpl (long __x, int __exp) __attribute__ ((__const__));

/* scalb is the BSD name for ldexp.  */
extern double scalb (double __x, int __exp) __attribute__ ((__const__));

/* Natural logarithm of x.  */
extern double log (double __x) __attribute__ ((__const__));
extern float logf (float __x) __attribute__ ((__const__));
extern long double logl (long double __x) __attribute__ ((__const__));

/* Base-ten logarithm of x.  */
extern double log10 (double __x) __attribute__ ((__const__));
extern float log10f (float __x) __attribute__ ((__const__));
extern long double log10l (long double __x) __attribute__ ((__const__));

/* Break value into integral and fractional parts.  */
extern double modf (double __value, double *__iprt);
extern float modff (float __value, float *__iprt);
extern long double modfl (long double __value, long double *__iprt);

/* Power functions.  */

/* Return x to the y power.  */
extern double pow (double __x, double __y) __attribute__ ((__const__));
extern float powf (float __x, float __y) __attribute__ ((__const__));
extern long double powl (long double __x,
                         long double __y) __attribute__ ((__const__));

/* Return the square root of x.  */
extern double sqrt (double __x) __attribute__ ((__const__));

/* Nearest integer, absolute value, and remainder functions.  */

/* Smallest integral value not less than X.  */
extern double ceil (double __x) __attribute__ ((__const__));
extern float ceilf (float __x) __attribute__ ((__const__));
extern long double ceill (long double __x) __attribute__ ((__const__));

/* Absolute value of X.  */
extern double fabs (double __x) __attribute__ ((__const__));


/* Largest integer not greater than X.  */
extern double floor (double __x) __attribute__ ((__const__));
extern float floorf (float __x) __attribute__ ((__const__));
extern long double floorl (long double __x) __attribute__ ((__const__));

/* Nearest integer to X, away from 0 as a double.  */
extern double round (double __x) __attribute__ ((__const__));

/* Nearest integer to X, away from 0 as a long int.  */
extern long lround (double __x) __attribute__ ((__const__));

#ifdef __GNUC__
/* Nearest integer to X, away from 0 as a long long.  */
extern long long llround (double __x) __attribute__ ((__const__));
#endif

/* Floating-point modulo remainder of X/Y.  */
extern double fmod (double __x, double __y) __attribute__ ((__const__));
extern float fmodf (float __x, float __y) __attribute__ ((__const__));
extern long double fmodl (long double __x,
                          long double __y) __attribute__ ((__const__));

/* Hypotenuese of x and y.  */
extern double hypot (double __x, double __y) __attribute__ ((__const__));

/* (Exponent of x) minus 1.  */
extern double expm1 (double __x) __attribute__ ((__const__));

/* Natural logarithm of (x plus 1).  */
extern double log1p (double __x) __attribute__ ((__const__));

/* Return 1 if x is infinite, else 0. */
extern int isinf (double __x) __attribute__ ((__const__));

/* Return 1 if x is Not A Number, else 0.  */
extern int isnan (double __x) __attribute__ ((__const__));

/* Return cube root of x. */
extern double cbrt (double __x) __attribute__ ((__const__));

/* Return 1 is x is finite, else 0.  */
extern int finite (double __x) __attribute__ ((__const__));

/* Return a value with magnitude of x and with the sign bit of y.  */
extern double copysign (double __x, double __y) __attribute__ ((__const__));

/* Return the next machine floating-point number of x in the
   direction toward y.  */
extern double nextafter (double __x, double __y) __attribute__ ((__const__));

/* Return x rounded to integral value according to the prevailing
   rounding mode.  */
extern double rint (double __x) __attribute__ ((__const__));

/* Return x*(2^n) computed by exponent manipulation rather than
   by actually performing an exponentiation or a multiplication.  */
extern double scalbn (double __x, int __n) __attribute__ ((__const__));

/* Return x rounded toward +inf to integral value. */
extern double ceil (double __x) __attribute__ ((__const__));

/* Return x rounded toward -inf to integral value.  */
extern double floor (double __x) __attribute__ ((__const__));


#ifdef __cplusplus
}
#endif

#endif
