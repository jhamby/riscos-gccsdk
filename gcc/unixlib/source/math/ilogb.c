/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math/ilogb.c,v $
 * $Date: 2000/07/15 14:52:21 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

/* @(#)s_ilogb.c 5.1 93/09/24 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#if defined(LIBM_SCCS) && !defined(lint)
static char rcsid[] = "$NetBSD: s_ilogb.c,v 1.9 1995/05/10 20:47:28 jtc Exp $";
#endif

/* ilogb(double x)
 * return the binary exponent of non-zero x
 * ilogb(0) = 0x80000001
 * ilogb(inf/NaN) = 0x7fffffff (no signal is raised)
 */

#include <math.h>
#include <unixlib/math.h>
#include <unixlib/types.h>

int
ilogb (double x)
{
  __int32_t hx, lx, ix;

  GET_HIGH_WORD (hx, x);
  hx &= 0x7fffffff;
  if (hx < 0x00100000)
    {
      GET_LOW_WORD (lx, x);
      if ((hx | lx) == 0)
	return 0x80000001;	/* ilogb(0) = 0x80000001 */
      else
	/* subnormal x */ if (hx == 0)
	{
	  for (ix = -1043; lx > 0; lx <<= 1)
	    ix -= 1;
	}
      else
	{
	  for (ix = -1022, hx <<= 11; hx > 0; hx <<= 1)
	    ix -= 1;
	}
      return ix;
    }
  else if (hx < 0x7ff00000)
    return (hx >> 20) - 1023;
  else
    return 0x7fffffff;
}
