/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math/lrint.c,v $
 * $Date: 2000/07/15 14:52:22 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#include <math.h>
#include <unixlib/math.h>
#include <unixlib/types.h>

static const double two52[2] =
{
  4.50359962737049600000e+15, /* 0x43300000, 0x00000000 */
 -4.50359962737049600000e+15, /* 0xC3300000, 0x00000000 */
};


long int
lrint (double x)
{
  __int32_t j0;
  __u_int32_t i0,i1;
  volatile double w;
  double t;
  long int result;
  int sx;

  EXTRACT_WORDS (i0, i1, x);
  j0 = ((i0 >> 20) & 0x7ff) - 0x3ff;
  sx = i0 >> 31;
  i0 &= 0xfffff;
  i0 |= 0x100000;

  if (j0 < 20)
    {
      if (j0 < -1)
	return 0;
      else
	{
	  w = two52[sx] + x;
	  t = w - two52[sx];
	  EXTRACT_WORDS (i0, i1, t);
	  j0 = ((i0 >> 20) & 0x7ff) - 0x3ff;
	  i0 &= 0xfffff;
	  i0 |= 0x100000;

	  result = i0 >> (20 - j0);
	}
    }
  else if (j0 < (__int32_t) (8 * sizeof (long int)) - 1)
    {
      if (j0 >= 52)
	result = ((long int) i0 << (j0 - 20)) | (i1 << (j0 - 52));
      else
	{
	  w = two52[sx] + x;
	  t = w - two52[sx];
	  EXTRACT_WORDS (i0, i1, t);
	  j0 = ((i0 >> 20) & 0x7ff) - 0x3ff;
	  i0 &= 0xfffff;
	  i0 |= 0x100000;

	  result = ((long int) i0 << (j0 - 20)) | (i1 >> (52 - j0));
	}
    }
  else
    {
      /* The number is too large.  It is left implementation defined
	 what happens.  */
      return (long int) x;
    }

  return sx ? -result : result;
}
