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
#include <endian.h>
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

#if __FLOAT_WORD_ORDER == __BIG_ENDIAN

typedef union
{
  double value;
  struct
  {
    __uint32_t msw;
    __uint32_t lsw;
  } parts;
} ieee_double_shape_type;

#endif

#if __FLOAT_WORD_ORDER == __LITTLE_ENDIAN

typedef union
{
  double value;
  struct
  {
    __uint32_t lsw;
    __uint32_t msw;
  } parts;
} ieee_double_shape_type;

#endif

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

#if __FLOAT_WORD_ORDER == __BIG_ENDIAN

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

#endif

#if __FLOAT_WORD_ORDER == __LITTLE_ENDIAN

typedef union
{
  long double value;
  struct
  {
    __uint32_t lsw;
    __uint32_t msw;
    unsigned int exponent:15;
    unsigned int sign:1;
    unsigned int empty:16;
  } parts;
} ieee_long_double_shape_type;

#endif

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

__END_DECLS

#endif
