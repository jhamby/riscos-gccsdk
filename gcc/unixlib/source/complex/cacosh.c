/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/complex/cacosh.c,v $
 * $Date: 2000/07/15 14:52:19 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

/* Return arc hyperbole cosine for double value.
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
cacosh (__complex__ double x)
{
  __complex__ double res;
  int rcls = fpclassify (__real__ x);
  int icls = fpclassify (__imag__ x);

  if (rcls <= FP_INFINITE || icls <= FP_INFINITE)
    {
      if (icls == FP_INFINITE)
	{
	  __real__ res = HUGE_VAL;

	  if (rcls == FP_NAN)
	    __imag__ res = NAN;
	  else
	    __imag__ res = copysign ((rcls == FP_INFINITE
					? (__real__ x < 0.0
					   ? M_PI - M_PI_4 : M_PI_4)
					: M_PI_2), __imag__ x);
	}
      else if (rcls == FP_INFINITE)
	{
	  __real__ res = HUGE_VAL;

	  if (icls >= FP_ZERO)
	    __imag__ res = copysign (signbit (__real__ x) ? M_PI : 0.0,
				     __imag__ x);
	  else
	    __imag__ res = NAN;
	}
      else
	{
	  __real__ res = NAN;
	  __imag__ res = NAN;
	}
    }
  else if (rcls == FP_ZERO && icls == FP_ZERO)
    {
      __real__ res = 0.0;
      __imag__ res = copysign (M_PI_2, __imag__ x);
    }
  else
    {
      __complex__ double y;

      __real__ y = (__real__ x - __imag__ x) * (__real__ x + __imag__ x) - 1.0;
      __imag__ y = 2.0 * __real__ x * __imag__ x;

      y = csqrt (y);

      __real__ y += __real__ x;
      __imag__ y += __imag__ x;

      res = clog (y);
    }

  return res;
}
