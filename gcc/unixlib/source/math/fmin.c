/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/math/c/fmin,v $
 * $Date: 1999/11/16 13:31:17 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#include <math.h>

double fmin (double x, double y)
{
  if (islessequal (x, y) || isnan (y))
    return x;

  return y;
}
