/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math/fmin.c,v $
 * $Date: 2000/07/15 14:52:21 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#include <math.h>

double fmin (double x, double y)
{
  if (islessequal (x, y) || isnan (y))
    return x;

  return y;
}
