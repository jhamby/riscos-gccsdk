/*
 * File taken from glibc.
 *  - SCL poison added.
 */
#ifdef __TARGET_SCL__
#  error "SCL build should not use (L)GPL code."
#endif

/* Install given floating-point environment.
   Copyright (C) 1997,98,99,2000,01,02 Free Software Foundation, Inc.
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
#include <fpu_control.h>

int
fesetenv (const fenv_t *envp)
{
#ifndef __SOFTFP__
#ifdef __VFP_FP__
  unsigned int temp;

  _FPU_GETCW (temp);
  temp &= _FPU_RESERVED;

  if (envp == FE_DFL_ENV)
    temp |= _FPU_DEFAULT;
  else if (envp == FE_NOMASK_ENV)
    temp |= _FPU_IEEE;
  else
    temp |= envp->__cw & ~_FPU_RESERVED;

  _FPU_SETCW (temp);

  /* Success.  */
#else
  if (envp == FE_DFL_ENV)
    _FPU_SETCW (_FPU_DEFAULT);
  else
    {
      unsigned long int temp = envp->__cw;
      _FPU_SETCW (temp);
    }

  /* Success.  */
#endif
#endif
  return 0;
}
