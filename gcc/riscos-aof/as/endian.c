/*
 * convert integers to/from ARM byte-sex.
 *
 * Andy Duplain, August 1992.
 */
#include "sdk-config.h"
#include "endiandef.h"

#define BYTE0SHIFT 24
#define BYTE1SHIFT 16
#define BYTE2SHIFT 8
#define BYTE3SHIFT 0

#if __BYTE_ORDER == __BIG_ENDIAN
/*
 * convert to ARM byte-sex.
 */
unsigned 
armword (unsigned val)
{
  union
    {
      unsigned i;
      char c[4];
    }
  ret;

  ret.c[0] = (val >> BYTE0SHIFT) & 0xff;
  ret.c[1] = (val >> BYTE1SHIFT) & 0xff;
  ret.c[2] = (val >> BYTE2SHIFT) & 0xff;
  ret.c[3] = (val >> BYTE3SHIFT) & 0xff;
  return (ret.i);
}

/*
 * convert from ARM byte-sex.
 */
unsigned 
ourword (unsigned val)
{
  union
  {
    unsigned i;
    char c[4];
  }
  ret;

  ret.c[0] = (val >> BYTE0SHIFT) & 0xff;
  ret.c[1] = (val >> BYTE1SHIFT) & 0xff;
  ret.c[2] = (val >> BYTE2SHIFT) & 0xff;
  ret.c[3] = (val >> BYTE3SHIFT) & 0xff;
  return (ret.i);
}

#endif /*LITTLEENDIAN */
