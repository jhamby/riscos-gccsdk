/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math/round.c,v $
 * $Date: 2000/07/15 14:52:22 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

/* Round double to integer away from zero.
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

#include <math.h>
#include <unixlib/math.h>
#include <unixlib/types.h>

static const double huge = 1.0e300;


double
round (double x)
{
  __int32_t i0, j0;
  __u_int32_t i1;

  EXTRACT_WORDS (i0, i1, x);
  j0 = ((i0 >> 20) & 0x7ff) - 0x3ff;
  if (j0 < 20)
    {
      if (j0 < 0)
	{
	  if (huge + x > 0.0)
	    {
	      i0 &= 0x80000000;
	      if (j0 == -1)
		i0 |= 0x3ff00000;
	      i1 = 0;
	    }
	}
      else
	{
	  __u_int32_t i = 0x000fffff >> j0;
	  if (((i0 & i) | i1) == 0)
	    /* X is integral.  */
	    return x;
	  if (huge + x > 0.0)
	    {
	      /* Raise inexact if x != 0.  */
	      i0 += 0x00080000 >> j0;
	      i0 &= ~i;
	      i1 = 0;
	    }
	}
    }
  else if (j0 > 51)
    {
      if (j0 == 0x400)
	/* Inf or NaN.  */
	return x + x;
      else
	return x;
    }
  else
    {
      __u_int32_t i = 0xffffffff >> (j0 - 20);
      if ((i1 & i) == 0)
	/* X is integral.  */
	return x;

      if (huge + x > 0.0)
	{
	  /* Raise inexact if x != 0.  */
	  __u_int32_t j = i1 + (1 << (51 - j0));
	  if (j < i1)
	    i0 += 1;
	  i1 = j;
	}
      i1 &= ~i;
    }

  INSERT_WORDS (x, i0, i1);
  return x;
}
