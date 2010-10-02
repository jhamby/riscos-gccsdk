/* gdtoa/FP math compatibility for ARM/FPA and ARM/VFP formats.
   Copyright (c) 2005-2010 UnixLib Developers.
   Written by Nick Burrett <nick@sqrt.co.uk>, May 2005.  */

#include <endian.h>

/* Similar to ieee754.h.  */

union IEEEf2bits
{
  float	f;
  struct
  {
#if __FLOAT_BIT_ORDER == __LITTLE_ENDIAN
    /* ARM/FPA and ARM/VFP.  */
    unsigned int man:23;
    unsigned int exp:8;
    unsigned int sign:1;
#endif
#if __FLOAT_BIT_ORDER == __BIG_ENDIAN
    unsigned int sign:1;
    unsigned int exp:8;
    unsigned int man:23;
#endif
  } bits;
};

#define	DBL_MANH_SIZE 20
#define	DBL_MANL_SIZE 32

union IEEEd2bits
{
  double d;
  struct
  {
#if __FLOAT_WORD_ORDER == __LITTLE_ENDIAN
    /* ARM/VFP.  */
    unsigned int manl:32;
#endif

#if __FLOAT_BIT_ORDER == __LITTLE_ENDIAN
    /* ARM/FPA and ARM/VFP.  */
    unsigned int manh:20;
    unsigned int exp:11;
    unsigned int sign:1;
#endif
#if __FLOAT_BIT_ORDER == __BIG_ENDIAN
    unsigned int sign:1;
    unsigned int exp:11;
    unsigned int manh:20;
#endif

#if __FLOAT_WORD_ORDER == __BIG_ENDIAN
    /* ARM/FPA.  */
    unsigned int manl:32;
#endif
  } bits;
};

