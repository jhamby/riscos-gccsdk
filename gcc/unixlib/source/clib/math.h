/****************************************************************************
 *
 * $Source$
 * $Date$
 * $Revision$
 * $State$
 * $Author$
 *
 ***************************************************************************/

/* ANSI Standard 4.5: Mathematics <math.h>.  */

#ifndef __MATH_H
#define __MATH_H

#ifndef __UNIXLIB_FEATURES_H
#include <unixlib/features.h>
#endif

#ifndef __MACHINE_HUGE_VAL_H
#include <machine/huge_val.h>
#endif
#ifndef __MACHINE_NAN_H
#include <machine/nan.h>
#endif

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

__BEGIN_DECLS

#define INFINITY HUGE_VALF

/* Trigonometric functions.  */

/* Arc cosine of x.  */
extern double acos (double __x) __attribute__ ((__const__));

/* Arc sine of x.  */
extern double asin (double __x) __attribute__ ((__const__));

/* Arc tangent of x.  */
extern double atan (double __x) __attribute__ ((__const__));

/* Arc tangent of y/x.  */
extern double atan2 (double __y,double __x) __attribute__ ((__const__));

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

/* Hyperbolic arc cosine of x.  */
extern double acosh (double __x) __attribute__ ((__const__));

/* Hyperbolic arc sine of x.  */
extern double asinh (double __x) __attribute__ ((__const__));

/* Hyperbolic arc tangent of x.  */
extern double atanh (double __x) __attribute__ ((__const__));

/* Exponential and logarithmic functions.  */

/* Exponentional function of x (2 ^ e).  */
extern double exp (double __x) __attribute__ ((__const__));

/* Exponentional function of x (2 ^ x).  */
extern double exp2 (double __x) __attribute__ ((__const__));

/* Exponentional function of x (10 ^ x).  */
extern double exp10 (double __x) __attribute__ ((__const__));

/* Break value into a normalized fracton and an integral power of 2.  */
extern double frexp (double value, int *__exp);

/* x times (two to the exp power).  */
extern double ldexp (double __x, int __exp) __attribute__ ((__const__));
/* scalb is the BSD name for ldexp.  */
extern double scalb (double __x, int __exp) __attribute__ ((__const__));

/* Natural logarithm of x.  */
extern double log (double __x) __attribute__ ((__const__));

/* Base-ten logarithm of x.  */
extern double log10 (double __x) __attribute__ ((__const__));

/* Break value into integral and fractional parts.  */
extern double modf (double __value, double *__iprt);

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

/* Return remainder of dividing x by y.  */
extern double drem (double __x, double __y) __attribute__ ((__const__));

/* Returns x REM p = x - [x/p]*p as if in infinite precise arithmetic,
   where [x/p] is the (infinite bit) integet nearest x/p (in half way
   case choose the even one).  */
extern double remainder (double __x, double __p) __attribute__ ((__const__));

/* Return the binary exponent of non-zero x.
   ilogb(0) = 0x80000001
   ilogb(inf/NaN) = 0x7fffffff */
extern int ilogb (double __x) __attribute__ ((__const__));

/* IEEE 754 logb. Use ilogb instead.  */
extern double logb (double __x) __attribute__ ((__const__));

extern double significand (double __x) __attribute__ ((__const__));

/* Bessel function of the first and second kinds of order zero.  */
extern double j0 (double __x) __attribute__ ((__const__));
extern double y0 (double __x) __attribute__ ((__const__));

/* Bessel function of the first and second kinds of order one.  */
extern double j1 (double __x) __attribute__ ((__const__));
extern double y1 (double __x) __attribute__ ((__const__));

/* Bessel function of the first and second kinds of order n.  */
extern double jn (int __n, double __x) __attribute__ ((__const__));
extern double yn (int __n, double __x) __attribute__ ((__const__));

/*                           x
                      2      |\
       erf(x)  =  ---------  | exp(-t*t)dt
                   sqrt(pi) \|
                             0

       erfc(x) =  1-erf(x)
*/
extern double erf (double __x) __attribute__ ((__const__));
extern double erfc (double __x) __attribute__ ((__const__));

#ifdef __UNIXLIB_INTERNALS
/* Internal function used by rem_pio2().  */
extern int __kernel_rem_pio2 (double *__x, double *__y,
			      int __e0, int __nx, int __prec,
			      const __int32_t *__ipio2);
#endif

/* Return the remainder of x rem pi/2 in y[0] + y[1].  */
extern __int32_t rem_pio2 (double __x, double *__y);

/* Gamma function.  */
extern double gamma (double __x);
extern double lgamma (double __x);
/* Reentrant version of the logarithm of the Gamma function.  */
extern double lgamma_r (double __x, int *__signgamp);
extern int signgam;

#ifdef __UNIXLIB_INTERNALS
/* Internals used by lgamma().  */
extern double __kernel_sin (double __x, double __y,
			    int __iy) __attribute__ ((__const__));
extern double __kernel_cos (double __x,
			    double __y) __attribute__ ((__const__));
#endif

/* BSD useful constants.  */
#define	M_E		2.7182818284590452354	/* e */
#define	M_LOG2E		1.4426950408889634074	/* log 2e */
#define	M_LOG10E	0.43429448190325182765	/* log 10e */
#define	M_LN2		0.69314718055994530942	/* log e2 */
#define	M_LN10		2.30258509299404568402	/* log e10 */
#define	M_PI		3.14159265358979323846	/* pi */
#define	M_PI_2		1.57079632679489661923	/* pi/2 */
#define	M_PI_4		0.78539816339744830962	/* pi/4 */
#define	M_1_PI		0.31830988618379067154	/* 1/pi */
#define	M_2_PI		0.63661977236758134308	/* 2/pi */
#define	M_2_SQRTPI	1.12837916709551257390	/* 2/sqrt(pi) */
#define	M_SQRT2		1.41421356237309504880	/* sqrt(2) */
#define	M_SQRT1_2	0.70710678118654752440	/* 1/sqrt(2) */


/* This is a GNU libc extension.  */
/* The above constants are not adequate for computation using `long double's.
   Therefore we provide as an extension constants with similar names as a
   GNU extension.  Provide enough digits for the 128-bit IEEE quad.  */
#define M_El		2.7182818284590452353602874713526625L  /* e */
#define M_LOG2El	1.4426950408889634073599246810018922L  /* log_2 e */
#define M_LOG10El	0.4342944819032518276511289189166051L  /* log_10 e */
#define M_LN2l		0.6931471805599453094172321214581766L  /* log_e 2 */
#define M_LN10l	2.3025850929940456840179914546843642L  /* log_e 10 */
#define M_PIl		3.1415926535897932384626433832795029L  /* pi */
#define M_PI_2l	1.5707963267948966192313216916397514L  /* pi/2 */
#define M_PI_4l	0.7853981633974483096156608458198757L  /* pi/4 */
#define M_1_PIl	0.3183098861837906715377675267450287L  /* 1/pi */
#define M_2_PIl	0.6366197723675813430755350534900574L  /* 2/pi */
#define M_2_SQRTPIl	1.1283791670955125738961589031215452L  /* 2/sqrt(pi) */
#define M_SQRT2l	1.4142135623730950488016887242096981L  /* sqrt(2) */
#define M_SQRT1_2l	0.7071067811865475244008443621048490L  /* 1/sqrt(2) */


/* Categories for floating point numbers.  */
#define FP_NAN 0
#define FP_INFINITE 1
#define FP_ZERO 2
#define FP_SUBNORMAL 3
#define FP_NORMAL 4

extern int fpclassify (double __x)  __attribute__ ((__const__));

/* Values returned by ilogb for 0 and NaN respectively.  */
#define FP_ILOGB0 0x80000000
#define FP_ILOGBNAN 0x80000000

/* Return non-zero if X is not +-Inf or NaN.  */
#define isfinite(x) (finite (x))

/* Return nonzero if x is neither zero, subnormal, Inf or NaN.  */
#define isnormal(x) (fpclassify (x) == FP_NORMAL)

/* Return non-zero if x > y.  */
#define isgreater(x, y) (! isunordered (x, y) && x > y)

/* Return non-zero if x >= y.  */
#define isgreaterequal(x, y) (! isunordered (x, y) && x >= y)

/* Return non-zero if x < y.  */
#define isless(x, y) (! isunordered (x, y) && x < y)

/* Return non-zero if x <= y.  */
#define islessequal(x, y) (! isunordered (x, y) && x <= y)

/* Return non-zero if either x < y or y < x.  */
#define islessgreater(x, y) (! isunordered (x, y) && (x < y || y < x))

/* Return non-zero if arguments are unordered.  */
#define isunordered(x, y) \
 (fpclassify (x) == FP_NAN || fpclassify(y) == FP_NAN)

/* Return (x*y) + z.  */
extern double fma (double __x, double __y,
		   double __z) __attribute__ ((__const__));

/* Return the value of smaller magnitude.  */
extern double fmin (double __x, double __y) __attribute__ ((__const__));

/* Return the value of greater magnitude.  */
extern double fmax (double __x, double __y) __attribute__ ((__const__));

/* Return the difference between x and y.  */
extern double fdim (double __x, double __y) __attribute__ ((__const__));

/* Compute base-2 logarithm of x.  */
extern double log2 (double __x) __attribute__ ((__const__));

/* Round x to the nearest integral.  */
extern long int lrint (double __x) __attribute__ ((__const__));
#ifdef __GNUC__
extern long long int llrint (double __x) __attribute__ ((__const__));
#endif

/* Round x to integral value.  */
extern double nearbyint (double __x) __attribute__ ((__const__));

/* Round x to integral value, but not to integral larger than x.  */
extern double trunc (double __x) __attribute__ ((__const__));

/* Compute remainder of X and Y and put in *QUO a value with sign of x/y
   and magnitude congruent `mod 2^n' to the magnitude of the integral
   quotient x/y, with n >= 3.  */
extern double remquo (double __x, double __y, int *__quo);

extern int signbit (double __x) __attribute__ ((__const__));

#ifdef __UNIXLIB_INTERNALS
extern void __sincos (double __x, double *__sinx, double *__cosx);
#endif

__END_DECLS

#endif
