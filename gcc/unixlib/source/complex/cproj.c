/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/complex/cproj.c,v $
 * $Date: 2000/07/15 14:52:19 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

/* Compute projection of complex double value to Riemann sphere.
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
cproj (__complex__ double x)
{
  __complex__ double res;

  if (!isfinite (__real__ x) || !isfinite (__imag__ x))
    {
      __real__ res = INFINITY;
      __imag__ res = copysign (0.0, __imag__ x);
    }
  else
    {
      double den = __real__ x * __real__ x + __imag__ x * __imag__ x + 1.0;

      __real__ res = (2.0 * __real__ x) / den;
      __imag__ res = (2.0 * __imag__ x) / den;
    }

  return res;
}
