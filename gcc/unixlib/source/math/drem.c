/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/math/c/drem,v $
 * $Date: 1999/11/16 13:26:49 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#include <math.h>
#include <unixlib/math.h>
#include <unixlib/types.h>

double
drem (double x, double y)
{
  return remainder (x, y);
}
