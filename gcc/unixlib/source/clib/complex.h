/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/complex.h,v $
 * $Date: 2003/07/29 23:04:27 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

/* ISO C99: 7.3 Complex arithmetic <complex.h> */

#ifndef __COMPLEX_H
#define __COMPLEX_H 1

#include <unixlib/features.h>

__BEGIN_DECLS

#ifdef __GNUC__
#define _Complex __complex__
#define complex _Complex

/* Narrowest imaginary unit.  */
#define _Complex_I	(__extension__ 1.0iF)
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
extern _Complex double cacos (double _Complex __z)
     __THROW __attribute__ ((__const__));
extern _Complex float cacosf (float _Complex __z)
     __THROW __attribute__ ((__const__));
extern _Complex long double cacosl (long double _Complex __z)
     __THROW __attribute__ ((__const__));

/* Arc sine of Z.  */
extern _Complex double casin (double _Complex __z)
     __THROW __attribute__ ((__const__));

/* Arc tangent of Z.  */
extern _Complex double catan (double _Complex __z)
     __THROW __attribute__ ((__const__));

/* Cosine of Z.  */
extern _Complex double ccos (double _Complex __z)
     __THROW __attribute__ ((__const__));

/* Sine of Z.  */
extern _Complex double csin (double _Complex __z)
     __THROW __attribute__ ((__const__));

/* Tangent of Z.  */
extern _Complex double ctan (double _Complex __z)
     __THROW __attribute__ ((__const__));


/* Hyperbolic functions.  */

/* Hyperbolic arc cosine of Z.  */
extern _Complex double cacosh (double _Complex __z)
     __THROW __attribute__ ((__const__));

/* Hyperbolic arc sine of Z.  */
extern _Complex double casinh (double _Complex __z)
     __THROW __attribute__ ((__const__));

/* Hyperbolic arc tangent of Z.  */
extern _Complex double catanh (double _Complex __z)
     __THROW __attribute__ ((__const__));

/* Hyperbolic cosine of Z.  */
extern _Complex double ccosh (double _Complex __z)
     __THROW __attribute__ ((__const__));

/* Hyperbolic sine of Z.  */
extern _Complex double csinh (double _Complex __z)
     __THROW __attribute__ ((__const__));

/* Hyperbolic tangent of Z.  */
extern _Complex double ctanh (double _Complex __z)
     __THROW __attribute__ ((__const__));

/* Exponential and logarithmic functions.  */

/* Exponential function of Z.  */
extern _Complex double cexp (double _Complex __z)
     __THROW __attribute__ ((__const__));

/* Natural logarithm of Z.  */
extern _Complex double clog (double _Complex __z)
     __THROW __attribute__ ((__const__));

/* The base 10 logarithm is not defined by the standard but to implement
   the standard C++ library it is handy.  */
extern _Complex double clog10 (double _Complex __z)
     __THROW __attribute__ ((__const__));

/* Power functions.  */

/* Return X to the Y power.  */
extern _Complex double cpow (double _Complex __x, double _Complex __y)
     __THROW __attribute__ ((__const__));

/* Return the square root of Z.  */
extern _Complex double csqrt (double _Complex __z)
     __THROW __attribute__ ((__const__));


/* Absolute value, conjugates, and projection.  */

/* Absolute value of Z.  */
extern double cabs (double _Complex __z)
     __THROW __attribute__ ((__const__));

/* Argument value of Z.  */
extern double carg (double _Complex __z)
     __THROW __attribute__ ((__const__));

/* Complex conjugate of Z.  */
extern _Complex double conj (double _Complex __z)
     __THROW __attribute__ ((__const__));

/* Projection of Z onto the Riemann sphere.  */
extern _Complex double cproj (double _Complex __z)
     __THROW __attribute__ ((__const__));


/* Decomposing complex values.  */

/* Imaginary part of Z.  */
extern double cimag (double _Complex __z)
     __THROW __attribute__ ((__const__));

/* Real part of Z.  */
extern double creal (double _Complex __z)
     __THROW __attribute__ ((__const__));

__END_DECLS

#endif
