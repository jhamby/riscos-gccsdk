/* gdtoa/FP math compatibility for ARM/FPA formats.
   Copyright (c) 2005 UnixLib Developers.
   Written by Nick Burrett <nick@sqrt.co.uk>, May 2005.  */

#include <endian.h>

union IEEEf2bits
{
  float	f;
  struct
  {
#if __FLOAT_BIT_ORDER == __LITTLE_ENDIAN
    /* ARM/FPA format and ARM/VFP format.  */
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
    /* ARM/VFP format.  */
    unsigned int manl:32;
#endif
#if __FLOAT_BIT_ORDER == __LITTLE_ENDIAN
    /* ARM/FPA format.  */
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
    /* ARM/FPA format.  */
    unsigned int manl:32;
#endif
  } bits;
};

union IEEEl2bits
{
  long double e;
  struct
  {
#if __FLOAT_WORD_ORDER == __LITTLE_ENDIAN
    /* ARM/VFP format.  */
    unsigned int manl:32;
    unsigned int manh:32;
#endif

#if __FLOAT_BIT_ORDER == __LITTLE_ENDIAN
    /* ARM/FPA format.  */
    unsigned int exp:15;
    unsigned int junk:16;
    unsigned int sign:1;
#endif
#if __FLOAT_BIT_ORDER == __BIG_ENDIAN
    unsigned int sign:1;
    unsigned int junk:16;
    unsigned int exp:15;
#endif

#if __FLOAT_WORD_ORDER == __BIG_ENDIAN
    /* ARM/FPA format.  */
    unsigned int manh:32;
    unsigned int manl:32;
#endif
  } bits;
};

#define	mask_nbit_l(u)	((u).bits.manh &= 0x7fffffff)

#define	LDBL_MANH_SIZE	32
#define	LDBL_MANL_SIZE	32

#define	LDBL_TO_ARRAY32(u, a) do {			\
	(a)[0] = (uint32_t)(u).bits.manl;		\
	(a)[1] = (uint32_t)(u).bits.manh;		\
} while(0)
