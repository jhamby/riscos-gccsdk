/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math/fdim.c,v $
 * $Date: 2000/07/15 14:52:21 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#include <math.h>

double
fdim (double x, double y)
{
  int clsx = fpclassify (x);
  int clsy = fpclassify (y);

  if (clsx == FP_NAN || clsy == FP_NAN
      || (y < 0 && clsx == FP_INFINITE && clsy == FP_INFINITE))
    /* Raise invalid flag.  */
    return x - y;

  return x < y ? 0 : x - y;
}
