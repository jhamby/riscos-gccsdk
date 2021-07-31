/* IEEE Floating Point definitions for RISC OS.
   Copyright (c) 2005-2010 UnixLib Developers.  */

#ifndef __MACHINE_IEEE754_H
#define __MACHINE_IEEE754_H 1

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#include <endian.h>

/* There are two models in use: ARM/FPA and ARM/VFP (ARM ARM C2.1.1 and
   C2.1.2).  Each model has the same layout for soft-float and hard-float
   configurations.  */

__BEGIN_DECLS

union ieee754_float
{
  float f;

  /* This is the IEEE 754 single-precision format.  */
  struct
  {
#if __FLOAT_BIT_ORDER == __LITTLE_ENDIAN
    /* ARM/FPA and ARM/VFP.  */
    unsigned int mantissa:23;
    unsigned int exponent:8;
    unsigned int negative:1;
#endif
#if __FLOAT_BIT_ORDER == __BIG_ENDIAN
    unsigned int negative:1;
    unsigned int exponent:8;
    unsigned int mantissa:23;
#endif
  } ieee;

  /* This format makes it easier to see if a NaN is a signalling NaN.  */
  struct
  {
#if __FLOAT_BIT_ORDER == __LITTLE_ENDIAN
    /* ARM/FPA and ARM/VFP.  */
    unsigned int mantissa:22;
    unsigned int quiet_nan:1;
    unsigned int exponent:8;
    unsigned int negative:1;
#endif
#if __FLOAT_BIT_ORDER == __BIG_ENDIAN
    unsigned int negative:1;
    unsigned int exponent:8;
    unsigned int quiet_nan:1;
    unsigned int mantissa:22;
#endif
  } ieee_nan;
};

#define IEEE754_FLOAT_BIAS	0x7f /* Added to exponent.  */


union ieee754_double
{
  double d;

  /* This is the IEEE 754 double-precision format.  */
  struct
  {
#if __FLOAT_WORD_ORDER == __LITTLE_ENDIAN
    /* ARM/VFP.  */
    unsigned int mantissa1:32;
#endif

#if __FLOAT_BIT_ORDER == __LITTLE_ENDIAN
    /* ARM/FPA and ARM/VFP.  */
    unsigned int mantissa0:20;
    unsigned int exponent:11;
    unsigned int negative:1;
#endif
#if __FLOAT_BIT_ORDER == __BIG_ENDIAN
    unsigned int negative:1;
    unsigned int exponent:11;
    unsigned int mantissa0:20;
#endif

#if __FLOAT_WORD_ORDER == __BIG_ENDIAN
    /* ARM/FPA.  */
    unsigned int mantissa1:32;
#endif
  } ieee;
  
  /* This format makes it easier to see if a NaN is a signalling NaN.  */
  struct
  {
#if __FLOAT_WORD_ORDER == __LITTLE_ENDIAN
    /* ARM/VFP.  */
    unsigned int mantissa1:32;
#endif

#if __FLOAT_BIT_ORDER == __LITTLE_ENDIAN
    /* ARM/FPA and ARM/VFP.  */
    unsigned int mantissa0:19;
    unsigned int quiet_nan:1;
    unsigned int exponent:11;
    unsigned int negative:1;
#endif
#if __FLOAT_BIT_ORDER == __BIG_ENDIAN
    unsigned int negative:1;
    unsigned int exponent:11;
    unsigned int quiet_nan:1;
    unsigned int mantissa0:19;
#endif

#if __FLOAT_WORD_ORDER == __BIG_ENDIAN
    /* ARM/FPA.  */
    unsigned int mantissa1:32;
#endif
  } ieee_nan;
};

#define IEEE754_DOUBLE_BIAS	0x3ff /* Added to exponent.  */


extern double __ieee754_exp (double __x);

__END_DECLS

#endif
