/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/math/c/fdim,v $
 * $Date: 1999/11/16 13:31:17 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: admin $
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
