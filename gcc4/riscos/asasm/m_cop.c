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

static int
CopInt (int max, const char *msg)
{
  const Value *i = exprBuildAndEval (ValueInt);
  if (i->Tag == ValueInt)
    {
      if (i->Data.Int.i < 0 || i->Data.Int.i > max)
	{
	  error (ErrorError, "%d is not a legal %s", i->Data.Int.i, msg);
	  return 0;
	}
    }
  else
    {
      error (ErrorError, "Illegal expression as %s", msg);
      return 0;
    }
  return i->Data.Int.i;
}

static void
coprocessor (bool CopOnly, ARMWord ir, int maxop)	/* p#,cpop,cpdst,cplhs,cprhs {,info} */
{
  int cop = CP_NUMBER (getCopNum ());

  /* FPA uses coprocessor 1 and 2.  FIXME: why no check on 2 ? */
  if (cop == CP_NUMBER (1))
    {
      if (option_pedantic)
	error (ErrorInfo, "Coprocessor 1 is the floating point unit. Use a floating point mnemonic if possible");
    }
  ir |= cop;

  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%scoprocessor number", InsertCommaAfter);
  if (maxop > 7)
    ir |= CP_DCODE (CopInt (maxop, "coprocessor opcode"));
  else
    ir |= CP_RTRAN (CopInt (maxop, "coprocessor opcode"));
  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%sdata operand", InsertCommaAfter);
  ir |= CopOnly ? CPDST_OP (getCopReg ()) : CPDST_OP (getCpuReg ());
  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%sdst", InsertCommaAfter);
  ir |= CPLHS_OP (getCopReg ());
  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%slhs", InsertCommaAfter);
  ir |= CPRHS_OP (getCopReg ());
  skipblanks ();
  if (Input_Match (',', true))
    ir |= CP_INFO (CopInt (7, "coprocessor info"));
  Put_Ins (ir);
}

/**
 * Implements CDP.
 *   CDP<cond> p#, CPop, CPd, CPn, CPm {,<info>}
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
  Target_NeedAtLeastArch (ARCH_ARMv5);
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
  Target_NeedAtLeastArch (ARCH_ARMv5);
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
  Target_NeedAtLeastArch (ARCH_ARMv5);
  coprocessor (false, 0xfe100010, 7);
  return false;
}

static void
coprocessorr (ARMWord ir)
{
  ir |= CP_NUMBER (getCopNum ());
  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%scoprocessor number", InsertCommaAfter);
  ir |= (CopInt (15, "coprocessor opcode") << 4);
  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%scoprocessor opcode", InsertCommaAfter);
  ir |= CPDST_OP (getCpuReg ());
  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%sdst", InsertCommaAfter);
  ir |= CPLHS_OP (getCpuReg ());
  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%slhs", InsertCommaAfter);
  ir |= CPRHS_OP (getCopReg ());

  Put_Ins (ir);
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
  Target_NeedAtLeastArch (ARCH_ARMv6);
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
  Target_NeedAtLeastArch (ARCH_ARMv6);
  coprocessorr (cc | 0x0C500000);
  return false;
}
