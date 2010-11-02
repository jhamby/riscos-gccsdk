/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2010 GCCSDK Developers
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * m_cop.c
 */

#include "config.h"
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif

#include "error.h"
#include "expr.h"
#include "get.h"
#include "help_cop.h"
#include "input.h"
#include "main.h"
#include "m_cop.h"
#include "option.h"
#include "put.h"
#include "reloc.h"
#include "targetcpu.h"

static void
coprocessor (bool CopOnly, ARMWord ir, int maxop)	/* p#,cpop,cpdst,cplhs,cprhs {,info} */
{
  int cop = CP_NUMBER (getCopNum ());

  if (cop == CP_NUMBER (1))
    {
      if (option_pedantic)
	error (ErrorInfo, "Coprocessor 1 is the floating point unit. Use a floating point mnemonic if possible");
    }
  else
    cpuWarn (ARM3);
  ir |= cop;
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, "%scoprocessor number", InsertCommaAfter);
  if (maxop > 7)
    ir |= CP_DCODE (help_copInt (maxop, "coprocessor opcode"));
  else
    ir |= CP_RTRAN (help_copInt (maxop, "coprocessor opcode"));
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, "%sdata operand", InsertCommaAfter);
  ir |= CopOnly ? CPDST_OP (getCopReg ()) : CPDST_OP (getCpuReg ());
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, "%sdst", InsertCommaAfter);
  ir |= CPLHS_OP (getCopReg ());
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, "%slhs", InsertCommaAfter);
  ir |= CPRHS_OP (getCopReg ());
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
      ir |= CP_INFO (help_copInt (7, "coprocessor info"));
    }
  putIns (ir);
}

/**
 * Implements CDP.
 * cdp CC p#,cpop,cpdst,cplhs,cprhs {,info}
 */
bool
m_cdp (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;
  coprocessor (true, cc | 0x0e000000, 15);
  return false;
}

/**
 * Implements CDP2.
 */
bool
m_cdp2 (void)
{
  coprocessor (true, 0xfe000000, 15);
  return false;
}

/** REGISTER TRANSFER **/

/**
 * Implements MCR.
 */
bool
m_mcr (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;
  coprocessor (false, cc | 0x0e000010, 7);
  return false;
}

/**
 * Implements MCR2.
 */
bool
m_mcr2 (void)
{
  coprocessor (false, 0xfe000010, 7);
  return false;
}

/**
 * Implements MRC.
 */
bool
m_mrc (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;
  coprocessor (false, cc | 0x0e100010, 7);
  return false;
}

/**
 * Implements MRC2.
 */
bool
m_mrc2 (void)
{
  coprocessor (false, 0xfe100010, 7);
  return false;
}

static void
coprocessorr (ARMWord ir)
{
  ir |= CP_NUMBER (getCopNum ());
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, "%scoprocessor number", InsertCommaAfter);
  ir |= (help_copInt (15, "coprocessor opcode") << 4);
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, "%scoprocessor opcode", InsertCommaAfter);
  ir |= CPDST_OP (getCpuReg ());
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, "%sdst", InsertCommaAfter);
  ir |= CPLHS_OP (getCpuReg ());
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, "%slhs", InsertCommaAfter);
  ir |= CPRHS_OP (getCopReg ());

  putIns (ir);
}

/**
 * Implements MCRR.
 */
bool
m_mcrr (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;
  coprocessorr (cc | 0x0C400000);
  return false;
}

/**
 * Implements MRRC.
 */
bool
m_mrrc (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;
  coprocessorr (cc | 0x0C500000);
  return false;
}
