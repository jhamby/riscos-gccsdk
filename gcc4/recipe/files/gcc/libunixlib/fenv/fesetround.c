/*
 * File taken from glibc 2.15.
 *  - SCL poison added.
 */

#ifdef __TARGET_SCL__
#  error "SCL build should not use (L)GPL code."
#endif

/* Set current rounding direction.
   Copyright (C) 1997, 1998 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#include <fenv.h>

int
fesetround (int round)
{
#if !defined(__SOFTFP__) && defined(__VFP_FP__)
  fpu_control_t temp;

  switch (round)
  {
    case FE_TONEAREST:
    case FE_UPWARD:
    case FE_DOWNWARD:
    case FE_TOWARDZERO:
      _FPU_GETCW (temp);
      temp = (temp & ~FE_TOWARDZERO) | round;
      _FPU_SETCW (temp);
      return 0;
    default:
      return 1;
  }
#else
  /* We only support FE_TONEAREST, so there is no need for any work.  */
  return (round == FE_TONEAREST)?0:1;
#endif
}
