/****************************************************************************
 *
 * $Source$
 * $Date$
 * $Revision$
 * $State$
 * $Author$
 *
 ***************************************************************************/

#include <math.h>
#include <unixlib/math.h>
#include <unixlib/types.h>

static const double zero = 0.0;


double
remquo (double x, double y, int *quo)
{
  __int32_t hx,hy;
  __uint32_t sx,lx,ly;
  int cquo, qs;

  EXTRACT_WORDS (hx, lx, x);
  EXTRACT_WORDS (hy, ly, y);
  sx = hx & 0x80000000;
  qs = sx ^ (hy & 0x80000000);
  hy &= 0x7fffffff;
  hx &= 0x7fffffff;

  /* Purge off exception values.  */
  if ((hy | ly) == 0)
    return (x * y) / (x * y); 			/* y = 0 */
  if ((hx >= 0x7ff00000)			/* x not finite */
      || ((hy >= 0x7ff00000)			/* p is NaN */
	  && (((hy - 0x7ff00000) | ly) != 0)))
    return (x * y) / (x * y);

  if (hy <= 0x7fbfffff)
    x = fmod (x, 8 * y);		/* now x < 8y */

  if (((hx - hy) | (lx - ly)) == 0)
    {
      *quo = qs ? -1 : 1;
      return zero * x;
    }

  x  = fabs (x);
  y  = fabs (y);
  cquo = 0;

  if (x >= 4 * y)
    {
      x -= 4 * y;
      cquo += 4;
    }
  if (x >= 2 * y)
    {
      x -= 2 * y;
      cquo += 2;
    }

  if (hy < 0x00200000)
    {
      if (x + x > y)
	{
	  x -= y;
	  ++cquo;
	  if (x + x >= y)
	    {
	      x -= y;
	      ++cquo;
	    }
	}
    }
  else
    {
      double y_half = 0.5 * y;
      if (x > y_half)
	{
	  x -= y;
	  ++cquo;
	  if (x >= y_half)
	    {
	      x -= y;
	      ++cquo;
	    }
	}
    }

  *quo = qs ? -cquo : cquo;

  if (sx)
    x = -x;
  return x;
}
