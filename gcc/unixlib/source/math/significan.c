/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/math/c/significan,v $
 * $Date: 1997/10/09 20:00:07 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

/* @(#)s_signif.c 5.1 93/09/24 */
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
static char rcsid[] = "$NetBSD: s_significand.c,v 1.6 1995/05/10 20:48:11 jtc Exp $";
#endif

/*
 * significand(x) computes just
 * 	scalb(x, (double) -ilogb(x)),
 * for exercising the fraction-part(F) IEEE 754-1985 test vector.
 */

#include <math.h>
#include <unixlib/math.h>

double significand(double x)
{
  return scalb (x,(double) -ilogb(x));
}
