/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/math/c/fmax,v $
 * $Date: 1999/11/16 13:31:17 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#include <math.h>

double fmax (double x, double y)
{
  if (isgreaterequal (x, y) || isnan (y))
    return x;

  return y;
}

