/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/complex/c/clog,v $
 * $Date: 1999/11/16 13:34:50 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

/* Compute complex natural logarithm.
   Copyright (C) 1997 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@cygnus.com>, 1997.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#include <complex.h>
#include <math.h>
#include <unixlib/math.h>

__complex__ double
clog (__complex__ double x)
{
  __complex__ double result;
  int rcls = fpclassify (__real__ x);
  int icls = fpclassify (__imag__ x);

  if (rcls == FP_ZERO && icls == FP_ZERO)
    {
      /* Real and imaginary part are 0.0.  */
      __imag__ result = signbit (__real__ x) ? M_PI : 0.0;
      __imag__ result = copysign (__imag__ result, __imag__ x);
      /* Yes, the following line raises an exception.  */
      __real__ result = -1.0 / fabs (__real__ x);
    }
  else if (rcls != FP_NAN && icls != FP_NAN)
    {
      /* Neither real nor imaginary part is NaN.  */
      __real__ result = log (hypot (__real__ x, __imag__ x));
      __imag__ result = atan2 (__imag__ x, __real__ x);
    }
  else
    {
      __imag__ result = NAN;
      if (rcls == FP_INFINITE || icls == FP_INFINITE)
	/* Real or imaginary part is infinite.  */
	__real__ result = HUGE_VAL;
      else
	__real__ result = NAN;
    }

  return result;
}
