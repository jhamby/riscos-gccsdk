/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/incl-local/internal/math.h,v $
 * $Date: 2005/04/14 14:31:13 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#ifndef __INTERNAL_MATH_H
#define __INTERNAL_MATH_H

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

#include <stdint.h>

__BEGIN_DECLS

/* The original fdlibm code used statements like:
	n0 = ((*(int*)&one)>>29)^1;		* index of high word *
	ix0 = *(n0+(int*)&x);			* high word of x *
	ix1 = *((1-n0)+(int*)&x);		* low word of x *
   to dig two 32 bit words out of the 64 bit IEEE floating point
   value.  That is non-ANSI, and, moreover, the gcc instruction
   scheduler gets it wrong.  We instead use the following macros.
   Unlike the original code, we determine the endianness at compile
   time, not at run time; I don't see much benefit to selecting
   endianness at run time.  */

/* A union which permits us to convert between a double and two 32 bit
   ints.  */


typedef union
{
  double value;
  struct
  {
    __uint32_t msw;
    __uint32_t lsw;
  } parts;
} ieee_double_shape_type;

/* Get two 32 bit ints from a double.  */

#define EXTRACT_WORDS(ix0,ix1,d)				\
do {								\
  ieee_double_shape_type ew_u;					\
  ew_u.value = (d);						\
  (ix0) = ew_u.parts.msw;					\
  (ix1) = ew_u.parts.lsw;					\
} while (0)

/* Get the more significant 32 bit int from a double.  */

#define GET_HIGH_WORD(i,d)					\
do {								\
  ieee_double_shape_type gh_u;					\
  gh_u.value = (d);						\
  (i) = gh_u.parts.msw;						\
} while (0)

/* Get the less significant 32 bit int from a double.  */

#define GET_LOW_WORD(i,d)					\
do {								\
  ieee_double_shape_type gl_u;					\
  gl_u.value = (d);						\
  (i) = gl_u.parts.lsw;						\
} while (0)

/* Set a double from two 32 bit ints.  */

#define INSERT_WORDS(d,ix0,ix1)					\
do {								\
  ieee_double_shape_type iw_u;					\
  iw_u.parts.msw = (ix0);					\
  iw_u.parts.lsw = (ix1);					\
  (d) = iw_u.value;						\
} while (0)

/* Set the more significant 32 bits of a double from an int.  */

#define SET_HIGH_WORD(d,v)					\
do {								\
  ieee_double_shape_type sh_u;					\
  sh_u.value = (d);						\
  sh_u.parts.msw = (v);						\
  (d) = sh_u.value;						\
} while (0)

/* Set the less significant 32 bits of a double from an int.  */

#define SET_LOW_WORD(d,v)					\
do {								\
  ieee_double_shape_type sl_u;					\
  sl_u.value = (d);						\
  sl_u.parts.lsw = (v);						\
  (d) = sl_u.value;						\
} while (0)

/* A union which permits us to convert between a float and a 32 bit
   int.  */

typedef union
{
  float value;
  __uint32_t word;
} ieee_float_shape_type;

/* Get a 32 bit int from a float.  */

#define GET_FLOAT_WORD(i,d)					\
do {								\
  ieee_float_shape_type gf_u;					\
  gf_u.value = (d);						\
  (i) = gf_u.word;						\
} while (0)

/* Set a float from a 32 bit int.  */

#define SET_FLOAT_WORD(d,i)					\
do {								\
  ieee_float_shape_type sf_u;					\
  sf_u.word = (i);						\
  (d) = sf_u.value;						\
} while (0)

/* A union which permits us to convert between a long double and
   three 32 bit ints.  */

typedef union
{
  long double value;
  struct
  {
    unsigned int exponent:15;
    unsigned int empty:16;
    unsigned int sign:1;
    __uint32_t msw;
    __uint32_t lsw;
  } parts;
} ieee_long_double_shape_type;

/* Get three 32 bit ints from a double. Hacked for ARM FPA.  */

#define GET_LDOUBLE_WORDS(exp,ix0,ix1,d)			\
do {								\
  ieee_long_double_shape_type ew_u;				\
  ew_u.value = (d);						\
  (exp) = ew_u.parts.exponent | (ew_u.parts.sign << 16);	\
  (ix0) = ew_u.parts.msw;					\
  (ix1) = ew_u.parts.lsw;					\
} while (0)

/* Set a double from two 32 bit ints. Hacked for ARM FPA.  */

#define SET_LDOUBLE_WORDS(d,exp,ix0,ix1)			\
do {								\
  ieee_long_double_shape_type iw_u;				\
  iw_u.parts.sign = (exp) & (1 << 16);				\
  iw_u.parts.exponent = (exp) & ((1 << 15)-1)			\
  iw_u.parts.msw = (ix0);					\
  iw_u.parts.lsw = (ix1);					\
  (d) = iw_u.value;						\
} while (0)


/* Get the more significant 32 bits of a long double mantissa.  */

#define GET_LDOUBLE_MSW(v,d)					\
do {								\
  ieee_long_double_shape_type sh_u;				\
  sh_u.value = (d);						\
  (v) = sh_u.parts.msw;						\
} while (0)

/* Set the more significant 32 bits of a long double mantissa from an int.  */

#define SET_LDOUBLE_MSW(d,v)					\
do {								\
  ieee_long_double_shape_type sh_u;				\
  sh_u.value = (d);						\
  sh_u.parts.msw = (v);						\
  (d) = sh_u.value;						\
} while (0)

/* Get int from the exponent of a long double.  */

#define GET_LDOUBLE_EXP(exp,d)					\
do {								\
  ieee_long_double_shape_type ge_u;				\
  ge_u.value = (d);						\
  (exp) = ge_u.parts.sign_exponent;				\
} while (0)

/* Set exponent of a long double from an int.  */

#define SET_LDOUBLE_EXP(d,exp)					\
do {								\
  ieee_long_double_shape_type se_u;				\
  se_u.value = (d);						\
  se_u.parts.sign_exponent = (exp);				\
  (d) = se_u.value;						\
} while (0)


/* ieee style elementary functions */
extern double __ieee754_sqrt (double);
extern double __ieee754_acos (double);
extern double __ieee754_acosh (double);
extern double __ieee754_log (double);
extern double __ieee754_atanh (double);
extern double __ieee754_asin (double);
extern double __ieee754_atan2 (double,double);
extern double __ieee754_exp (double);
extern double __ieee754_exp2 (double);
extern double __ieee754_exp10 (double);
extern double __ieee754_cosh (double);
extern double __ieee754_fmod (double,double);
extern double __ieee754_pow (double,double);
extern double __ieee754_lgamma_r (double,int *);
extern double __ieee754_gamma_r (double,int *);
extern double __ieee754_lgamma (double);
extern double __ieee754_gamma (double);
extern double __ieee754_log10 (double);
extern double __ieee754_log2 (double);
extern double __ieee754_sinh (double);
extern double __ieee754_hypot (double,double);
extern double __ieee754_j0 (double);
extern double __ieee754_j1 (double);
extern double __ieee754_y0 (double);
extern double __ieee754_y1 (double);
extern double __ieee754_jn (int,double);
extern double __ieee754_yn (int,double);
extern double __ieee754_remainder (double,double);
extern int32_t __ieee754_rem_pio2 (double,double*);
extern double __ieee754_scalb (double,double);

/* fdlibm kernel function */
extern double __kernel_standard (double,double,int);
extern double __kernel_sin (double,double,int);
extern double __kernel_cos (double,double);
extern double __kernel_tan (double,double,int);
extern int    __kernel_rem_pio2 (double*,double*,int,int,int, const int32_t*);

/* internal functions.  */
extern double __copysign (double x, double __y);


/* ieee style elementary float functions */
extern float __ieee754_sqrtf (float);
extern float __ieee754_acosf (float);
extern float __ieee754_acoshf (float);
extern float __ieee754_logf (float);
extern float __ieee754_atanhf (float);
extern float __ieee754_asinf (float);
extern float __ieee754_atan2f (float,float);
extern float __ieee754_expf (float);
extern float __ieee754_exp2f (float);
extern float __ieee754_exp10f (float);
extern float __ieee754_coshf (float);
extern float __ieee754_fmodf (float,float);
extern float __ieee754_powf (float,float);
extern float __ieee754_lgammaf_r (float,int *);
extern float __ieee754_gammaf_r (float,int *);
extern float __ieee754_lgammaf (float);
extern float __ieee754_gammaf (float);
extern float __ieee754_log10f (float);
extern float __ieee754_log2f (float);
extern float __ieee754_sinhf (float);
extern float __ieee754_hypotf (float,float);
extern float __ieee754_j0f (float);
extern float __ieee754_j1f (float);
extern float __ieee754_y0f (float);
extern float __ieee754_y1f (float);
extern float __ieee754_jnf (int,float);
extern float __ieee754_ynf (int,float);
extern float __ieee754_remainderf (float,float);
extern int32_t __ieee754_rem_pio2f (float,float*);
extern float __ieee754_scalbf (float,float);


/* float versions of fdlibm kernel functions */
extern float __kernel_sinf (float,float,int);
extern float __kernel_cosf (float,float);
extern float __kernel_tanf (float,float,int);
extern int   __kernel_rem_pio2f (float*,float*,int,int,int, const int32_t*);

/* internal functions.  */
extern float __copysignf (float x, float __y);

/* Prototypes for functions of the IBM Accurate Mathematical Library.  */
extern double __exp1 (double __x, double __xx, double __error);
extern double __sin (double __x);
extern double __cos (double __x);
extern int __branred (double __x, double *__a, double *__aa);
extern void __doasin (double __x, double __dx, double __v[]);
extern void __dubsin (double __x, double __dx, double __v[]);
extern void __dubcos (double __x, double __dx, double __v[]);
extern double __halfulp (double __x, double __y);
extern double __sin32 (double __x, double __res, double __res1);
extern double __cos32 (double __x, double __res, double __res1);
extern double __mpsin (double __x, double __dx);
extern double __mpcos (double __x, double __dx);
extern double __mpsin1 (double __x);
extern double __mpcos1 (double __x);
extern double __slowexp (double __x);
extern double __slowpow (double __x, double __y, double __z);
extern void __docos (double __x, double __dx, double __v[]);

#if defined (__CC_NORCROFT)
/* The Norcroft compiler doesn't support the idea of symbol aliasing,
   or at least allowing us to pass such aliases to the assembler.  */

#define __acos acos
#define __asin asin
#define __atan atan
#define __atan2 atan2
#define __cos cos
#define __sin sin
#define __tan tan
#define __cosh cosh
#define __sinh sinh
#define __tanh tanh
#define __sincos sincos
#define __acosh acosh
#define __asinh asinh
#define __atanh atanh
#define __exp exp
#define __frexp frexp
#define __ldexp ldexp
#define __log log
#define __log10 log10
#define __modf modf
#define __exp10 exp10
#define __pow10 pow10
#define __expm1 expm1
#define __log1p log1p
#define __logb logb
#define __exp2 exp2
#define __log2 log2
#define __pow pow
#define __sqrt sqrt
#define __hypot hypot
#define __cbrt cbrt
#define __ceil ceil
#define __fabs fabs
#define __floor floor
#define __fmod fmod
#define __isinf isinf
#define __finite finite
#define __drem drem
#define __significand significand
#define __copysign copysign
#define __nan nan
#define __isnan isnan
#define __j0 j0
#define __j1 j1
#define __jn jn
#define __y0 y0
#define __y1 y1
#define __yn yn
#define __erf erf
#define __erfc erfc
#define __lgamma lgamma
#define __tgamma tgamma
#define __gamma gamma
#define __lgamma_r lgamma_r
#define __rint rint
#define __nextafter nextafter
#define __nexttoward nexttoward
#define __remainder remainder
#define __scalbn scalbn
#define __ilogb ilogb
#define __scalbln scalbln
#define __nearbyint nearbyint
#define __round round
#define __trunc trunc
#define __remquo remquo
#define __lrint lrint
#define __lround lround
#define __fdim fdim
#define __fmax fmax
#define __fmin fmin
#define __fma fma
#define __scalb scalb

#define __acosf acosf
#define __asinf asinf
#define __atanf atanf
#define __atan2f atan2f
#define __cosf cosf
#define __sinf sinf
#define __tanf tanf
#define __coshf coshf
#define __sinhf sinhf
#define __tanhf tanhf
#define __sincosf sincosf
#define __acoshf acoshf
#define __asinhf asinhf
#define __atanhf atanhf
#define __expf expf
#define __frexpf frexpf
#define __ldexpf ldexpf
#define __logf logf
#define __log10f log10f
#define __modff modff
#define __exp10f exp10f
#define __pow10f pow10f
#define __expm1f expm1f
#define __log1pf log1pf
#define __logbf logbf
#define __exp2f exp2f
#define __log2f log2f
#define __powf powf
#define __sqrtf sqrtf
#define __hypotf hypotf
#define __cbrtf cbrtf
#define __ceilf ceilf
#define __fabsf fabsf
#define __floorf floorf
#define __fmodf fmodf
#define __isinff isinff
#define __finitef finitef
#define __dremf dremf
#define __significandf significandf
#define __copysignf copysignf
#define __nanf nanf
#define __isnanf isnanf
#define __j0f j0f
#define __j1f j1f
#define __jnf jnf
#define __y0f y0f
#define __y1f y1f
#define __ynf ynf
#define __erff erff
#define __erfcf erfcf
#define __lgammaf lgammaf
#define __tgammaf tgammaf
#define __gammaf gammaf
#define __lgammaf_r lgammaf_r
#define __rintf rintf
#define __nextafterf nextafterf
#define __nexttowardf nexttowardf
#define __remainderf remainderf
#define __scalbnf scalbnf
#define __ilogbf ilogbf
#define __scalblnf scalblnf
#define __nearbyintf nearbyintf
#define __roundf roundf
#define __truncf truncf
#define __remquof remquof
#define __lrintf lrintf
#define __llrintf llrintf
#define __lroundf lroundf
#define __llroundf llroundf
#define __fdimf fdimf
#define __fmaxf fmaxf
#define __fminf fminf
#define __fmaf fmaf


#define __cacos cacos
#define __casin casin
#define __catan catan
#define __ccos ccos
#define __csin csin
#define __ctan ctan
#define __cacosh cacosh
#define __casinh casinh
#define __catanh catanh
#define __ccosh ccosh
#define __csinh csinh
#define __ctanh ctanh
#define __cexp cexp
#define __clog clog
#define __clog10 clog10
#define __cpow cpow
#define __csqrt csqrt
#define __cabs cabs
#define __carg carg
#define __conj conj
#define __cproj cproj
#define __cimag cimag
#define __creal creal

#endif /* __CC_NORCROFT */

__END_DECLS

#endif
