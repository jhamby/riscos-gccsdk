/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/complex/c/csqrt,v $
 * $Date: 1999/11/16 13:34:50 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

/* Complex square root of double value.
   Copyright (C) 1997, 1998 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Based on an algorithm by Stephen L. Moshier <moshier@world.std.com>.
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
csqrt (__complex__ double x)
{
  __complex__ double res;
  int rcls = fpclassify (__real__ x);
  int icls = fpclassify (__imag__ x);

  if (rcls <= FP_INFINITE || icls <= FP_INFINITE)
    {
      if (icls == FP_INFINITE)
	{
	  __real__ res = HUGE_VAL;
	  __imag__ res = __imag__ x;
	}
      else if (rcls == FP_INFINITE)
	{
	  if (__real__ x < 0.0)
	    {
	      __real__ res = icls == FP_NAN ? NAN : 0;
	      __imag__ res = copysign (HUGE_VAL, __imag__ x);
	    }
	  else
	    {
	      __real__ res = __real__ x;
	      __imag__ res = (icls == FP_NAN
			      ? NAN : copysign (0.0, __imag__ x));
	    }
	}
      else
	{
	  __real__ res = NAN;
	  __imag__ res = NAN;
	}
    }
  else
    {
      if (icls == FP_ZERO)
	{
	  if (__real__ x < 0.0)
	    {
	      __real__ res = 0.0;
	      __imag__ res = copysign (sqrt (-__real__ x), __imag__ x);
	    }
	  else
	    {
	      __real__ res = fabs (sqrt (__real__ x));
	      __imag__ res = copysign (0.0, __imag__ x);
	    }
	}
      else if (rcls == FP_ZERO)
	{
	  double r = sqrt (0.5 * fabs (__imag__ x));

	  __real__ res = copysign (r, __imag__ x);
	  __imag__ res = r;
	}
      else
	{
	  double d, r, s;

	  d = hypot (__real__ x, __imag__ x);
	  /* Use the identity   2  Re res  Im res = Im x
	     to avoid cancellation error in  d +/- Re x.  */
	  if (__real__ x > 0)
	    {
	      r = sqrt (0.5 * d + 0.5 * __real__ x);
	      s = (0.5 * __imag__ x) / r;
	    }
	  else
	    {
	      s = sqrt (0.5 * d - 0.5 * __real__ x);
	      r = fabs ((0.5 * __imag__ x) / s);
	    }

	  __real__ res = r;
	  __imag__ res = copysign (s, __imag__ x);
	}
    }

  return res;
}
