/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/complex/casin.c,v $
 * $Date: 2000/07/15 14:52:19 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

/* Return arc sine of complex double value.
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
casin (__complex__ double x)
{
  __complex__ double res;

  if (isnan (__real__ x) || isnan (__imag__ x))
    {
      if (__real__ x == 0.0)
	{
	  res = x;
	}
      else if (isinf (__real__ x) || isinf (__imag__ x))
	{
	  __real__ res = NAN;
	  __imag__ res = copysign (HUGE_VAL, __imag__ x);
	}
      else
	{
	  __real__ res = NAN;
	  __imag__ res = NAN;
	}
    }
  else
    {
      __complex__ double y;

      __real__ y = -__imag__ x;
      __imag__ y = __real__ x;

      y = casinh (y);

      __real__ res = __imag__ y;
      __imag__ res = -__real__ y;
    }

  return res;
}
