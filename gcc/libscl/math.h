/* math.h.  Mathematics for the Standard C Library.

   For use with the GNU compilers.
   (c) Copyright 1997, Nick Burrett.  */

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
/* Arc sine of x.  */
extern double asin (double __x) __attribute__ ((__const__));
/* Arc tangent of x.  */
extern double atan (double __x) __attribute__ ((__const__));
/* Arc tangent of y/x.  */
extern double atan2 (double __y, double __x) __attribute__ ((__const__));

/* Cosine of x.  */
extern double cos (double __x) __attribute__ ((__const__));
/* Sine of x.  */
extern double sin (double __x) __attribute__ ((__const__));
/* Tangent of x.  */
extern double tan (double __x) __attribute__ ((__const__));

/* Hyperbolic functions.  */

/* Hyperbolic cosine of x.  */
extern double cosh (double __x) __attribute__ ((__const__));
/* Hyperbolic sine of x.  */
extern double sinh (double __x) __attribute__ ((__const__));
/* Hyperbolic tangent of x.  */
extern double tanh (double __x) __attribute__ ((__const__));

/* Exponential and logarithmic functions.  */

/* Exponentional function of x.  */
extern double exp (double __x) __attribute__ ((__const__));
/* Break value into a normalized fraction and an integral power of 2.  */
extern double frexp (double __value, int *__exp);
/* x times (two to the exp power).  */
extern double ldexp (double __x, int exp) __attribute__ ((__const__));
/* Natural logarithm of x.  */
extern double log (double __x) __attribute__ ((__const__));
/* Base-ten logarithm of x.  */
extern double log10 (double __x) __attribute__ ((__const__));
/* Break value into integral and fractional parts.  */
extern double modf (double __value, double *__integer);

/* Power functions.  */

/* Return x to the y power.  */
extern double pow (double __x, double __y) __attribute__ ((__const__));
/* Return the square root of x.  */
extern double sqrt (double __x) __attribute__ ((__const__));

/* Nearest integer, absolute value, and remainder functions.  */

/* Smallest integral value not less than X.  */
extern double ceil (double __x) __attribute__ ((__const__));
/* Absolute value of X.  */
extern double fabs (double __x) __attribute__ ((__const__));
/* Largest integer not greater than X.  */
extern double floor (double __x) __attribute__ ((__const__));
/* Floating-point modulo remainder of X/Y.  */
extern double fmod (double __x, double __y) __attribute__ ((__const__));

#ifdef __cplusplus
}
#endif

#endif
