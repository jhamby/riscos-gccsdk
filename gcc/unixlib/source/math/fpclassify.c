/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/math/c/fpclassify,v $
 * $Date: 1999/11/16 13:31:17 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#include <math.h>
#include <unixlib/math.h>
#include <unixlib/types.h>

int fpclassify (double x)
{
  __u_int32_t hx, lx;
  int retval = FP_NORMAL;

  EXTRACT_WORDS (hx, lx, x);
  lx |= hx & 0xfffff;
  hx &= 0x7ff00000;
  if ((hx | lx) == 0)
    retval = FP_ZERO;
  else if (hx == 0)
    retval = FP_SUBNORMAL;
  else if (hx == 0x7ff00000)
    retval = (lx != 0) ? FP_NAN : FP_INFINITE;

  return retval;
}
