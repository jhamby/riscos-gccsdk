/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/complex.h,v $
 * $Date: 2000/07/15 14:52:10 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifndef __COMPLEX_H
#define __COMPLEX_H 1

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __GNUC__
#define _Complex __complex__

/* Narrowest imaginary unit.  */
#define _Complex_I	(1.0iF)
#else
/* Acorn C doesn't support complex numbers.
   Define enough macros to allow compilation to continue even though
   the functions will never work.  */
#define _Complex /**/
#define __real__ /**/
#define __imag__ /**/
#define __complex__ /**/
#endif


/* Another more descriptive name is `I'.  */
#undef I
#define I _Complex_I

/* Optimization aids.  */
#define CX_LIMITED_RANGE_ON
#define CX_LIMITED_RANGE_OFF
#define CX_LIMITED_RANGE_DEFAULT

/* Trigonometric functions.  */

/* Arc cosine of Z.  */
_Complex double cacos (double _Complex __z);
_Complex double cacosf (float _Complex __z);
_Complex double cacosl (long double _Complex __z);

/* Arc sine of Z.  */
_Complex double casin (double _Complex __z);
/* Arc tangent of Z.  */
_Complex double catan (double _Complex __z);

/* Cosine of Z.  */
_Complex double ccos (double _Complex __z);
/* Sine of Z.  */
_Complex double csin (double _Complex __z);
/* Tangent of Z.  */
_Complex double ctan (double _Complex __z);


/* Hyperbolic functions.  */

/* Hyperbolic arc cosine of Z.  */
_Complex double cacosh (double _Complex __z);
/* Hyperbolic arc sine of Z.  */
_Complex double casinh (double _Complex __z);
/* Hyperbolic arc tangent of Z.  */
_Complex double catanh (double _Complex __z);

/* Hyperbolic cosine of Z.  */
_Complex double ccosh (double _Complex __z);
/* Hyperbolic sine of Z.  */
_Complex double csinh (double _Complex __z);
/* Hyperbolic tangent of Z.  */
_Complex double ctanh (double _Complex __z);


/* Exponential and logarithmic functions.  */

/* Exponential function of Z.  */
_Complex double cexp (double _Complex __z);

/* Natural logarithm of Z.  */
_Complex double clog (double _Complex __z);

/* The base 10 logarithm is not defined by the standard but to implement
   the standard C++ library it is handy.  */
_Complex double clog10 (double _Complex __z);

/* Power functions.  */

/* Return X to the Y power.  */
_Complex double cpow (double _Complex __x, double _Complex __y);

/* Return the square root of Z.  */
_Complex double csqrt (double _Complex __z);


/* Absolute value, conjugates, and projection.  */

/* Absolute value of Z.  */
double cabs (double _Complex __z);

/* Argument value of Z.  */
double carg (double _Complex __z);

/* Complex conjugate of Z.  */
_Complex double conj (double _Complex __z);

/* Projection of Z onto the Riemann sphere.  */
_Complex double cproj (double _Complex __z);


/* Decomposing complex values.  */

/* Imaginary part of Z.  */
double cimag (double _Complex __z);

/* Real part of Z.  */
double creal (double _Complex __z);

#ifdef __cplusplus
}
#endif

#endif
