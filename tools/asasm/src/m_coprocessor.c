/*
 * AsAsm an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2013 GCCSDK Developers
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
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
#include "it.h"
#include "main.h"
#include "m_coprocessor.h"
#include "option.h"
#include "put.h"
#include "reloc.h"
#include "state.h"
#include "targetcpu.h"

static int
CopInt (int max, const char *msg)
{
  const Value *i = Expr_BuildAndEval (ValueInt);
  if (i->Tag == ValueInt)
    {
      if (i->Data.Int.i < 0 || i->Data.Int.i > max)
	{
	  Error (ErrorError, "%d is not a legal %s", i->Data.Int.i, msg);
	  return 0;
	}
    }
  else
    {
      Error (ErrorError, "Illegal expression as %s", msg);
      return 0;
    }
  return i->Data.Int.i;
}

/**
 * Parses: p#, CPopc1, CPd, CPn, CPm {,{#}<CPopc2>}
 */
static bool
coprocessor (const char *descr, bool doLowerCase, bool two,
             bool CopOnly, ARMWord ir, int maxop)
{
  InstrWidth_e instrWidth = Option_GetInstrWidth (doLowerCase);
  if (instrWidth == eInstrWidth_Unrecognized)
    return true;

  InstrType_e instrState = State_GetInstrType ();
  IT_ApplyCond (ir, false, instrState != eInstrType_ARM); 

  /* For Thumb2, we need at least ARMv6T2.
     CDP2, MCR2, MRC2 for ARM need at least ARMv5T.  */
  if (instrState != eInstrType_ARM)
    Target_CheckCPUFeature (kCPUExt_v6T2, true);
  else if (two)
    Target_CheckCPUFeature (kCPUExt_v5T, true);

  int cop = CP_NUMBER (Get_CopNum ());

  /* FPA uses coprocessor 1 and 2 (the latter only for LFM/SFM).  */
  if (cop == CP_NUMBER (1) || cop == CP_NUMBER (2))
    {
      if (option_pedantic)
	Error (ErrorInfo, "Coprocessor 1 and 2 are FPA floating point unit. Use FPA mnemonics if possible");
    }
  ir |= cop;

  Input_SkipWS ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%scoprocessor number", InsertCommaAfter);
  if (maxop > 7)
    ir |= CP_DCODE (CopInt (maxop, "coprocessor opcode"));
  else
    ir |= CP_RTRAN (CopInt (maxop, "coprocessor opcode"));
  Input_SkipWS ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%sdata operand", InsertCommaAfter);
  ir |= CPDST_OP (CopOnly ? Get_CopReg () : Get_CPUReg ());
  Input_SkipWS ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%sdst", InsertCommaAfter);
  ir |= CPLHS_OP (Get_CopReg ());
  Input_SkipWS ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%slhs", InsertCommaAfter);
  ir |= CPRHS_OP (Get_CopReg ());
  Input_SkipWS ();
  if (Input_Match (',', true))
    {
      (void) Input_Match ('#', true);
      ir |= CP_INFO (CopInt (7, "coprocessor info"));
    }

  if (instrState != eInstrType_ARM && instrWidth == eInstrWidth_Enforce16bit)
    Error (ErrorError, "Instruction %s is not supported for Thumb", descr);

  /* Instructions bits for ARM and Thumb2 are the same.  Some cc fiddling
     is still needed.  */
  if (two)
    ir |= 0xf0000000;
  else if (instrState != eInstrType_ARM)
    ir = (ir & 0x0fffffff) | 0xe0000000;
  Put_Ins (4, ir);
  return false;
}

/**
 * Implements CDP and CDP2.
 *   CDP<cond> p#, CPopc1, CPd, CPn, CPm {,{#}<CPopc2>}
 */
bool
m_cdp (bool doLowerCase)
{
  if (Input_Match ('2', false))
    return coprocessor ("CDP2", doLowerCase, true, true, 0xe0000000 | 0x0e000000, 15);

  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  return coprocessor ("CDP", doLowerCase, false, true, cc | 0x0e000000, 15);
}

/** REGISTER TRANSFER **/

/**
 * Implements MCR and MCR2.
 */
bool
m_mcr (bool doLowerCase)
{
  if (Input_Match ('2', false))
    return coprocessor ("MCR2", doLowerCase, true, false, 0xe0000000 | 0x0e000010, 7);

  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  return coprocessor ("MCR", doLowerCase, false, false, cc | 0x0e000010, 7);
}

/**
 * Implements MRC and MRC2.
 */
bool
m_mrc (bool doLowerCase)
{
  if (Input_Match ('2', false))
    return coprocessor ("MRC2", doLowerCase, true, false, 0xe0000000 | 0x0e100010, 7);
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  return coprocessor ("MRC", doLowerCase, false, false, cc | 0x0e100010, 7);
}

static void
coprocessorr (ARMWord ir)
{
  ir |= CP_NUMBER (Get_CopNum ());
  Input_SkipWS ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%scoprocessor number", InsertCommaAfter);
  ir |= (CopInt (15, "coprocessor opcode") << 4);
  Input_SkipWS ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%scoprocessor opcode", InsertCommaAfter);
  ir |= CPDST_OP (Get_CPUReg ());
  Input_SkipWS ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%sdst", InsertCommaAfter);
  ir |= CPLHS_OP (Get_CPUReg ());
  Input_SkipWS ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%slhs", InsertCommaAfter);
  ir |= CPRHS_OP (Get_CopReg ());

  Put_Ins (4, ir);
}

/**
 * Implements MCRR.
 */
bool
m_mcrr (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  Target_CheckCPUFeature (kCPUExt_v5E, true);
  coprocessorr (cc | 0x0C400000);
  return false;
}

/**
 * Implements MRRC.
 */
bool
m_mrrc (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  Target_CheckCPUFeature (kCPUExt_v5E, true);
  coprocessorr (cc | 0x0C500000);
  return false;
}


static bool
dstmem (const char *descr, bool doLowerCase, bool two,
        ARMWord ir, bool literal)
{
  InstrWidth_e instrWidth = Option_GetInstrWidth (doLowerCase);
  if (instrWidth == eInstrWidth_Unrecognized)
    return true;

  InstrType_e instrState = State_GetInstrType ();
  IT_ApplyCond (ir, false, instrState != eInstrType_ARM); 

  /* For Thumb2, we need at least ARMv6T2.
     CDP2, MCR2, MRC2 for ARM need at least ARMv5T.  */
  if (instrState != eInstrType_ARM)
    Target_CheckCPUFeature (kCPUExt_v6T2, true);
  else if (two)
    Target_CheckCPUFeature (kCPUExt_v5T, true);

  int cop = CP_NUMBER (Get_CopNum ());

  /* FPA uses coprocessor 1 and 2 (the latter only for LFM/SFM).  */
  if (cop == CP_NUMBER (1) || cop == CP_NUMBER (2))
    {
      if (option_pedantic)
	Error (ErrorInfo, "Coprocessor 1 and 2 are FPA floating point unit. Use FPA mnemonics if possible");
    }
  ir |= cop;

  Input_SkipWS ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%scoprocessor number", InsertCommaAfter);
  ir |= CPDST_OP (Get_CopReg ());

  if (instrState != eInstrType_ARM && instrWidth == eInstrWidth_Enforce16bit)
    Error (ErrorError, "Instruction %s is not supported for Thumb", descr);

  /* Instructions bits for ARM and Thumb2 are the same.  Some cc fiddling
     is still needed.  */
  if (two)
    ir |= 0xf0000000;
  else if (instrState != eInstrType_ARM)
    ir = (ir & 0x0fffffff) | 0xe0000000;
  HelpCop_Addr (ir, literal, false, instrState != eInstrType_ARM);
  return false;
}

/**
 * Implements LDC and LDC2.
 * LDC{<cond>}{L} <coproc>, <CRd>, <addressing mode 5> (pre-UAL)
 * LDC{L}{<cond>} <coproc>, <CRd>, <addressing mode 5> (UAL)
 * LDC2{L} <coproc>, <CRd>, <addressing mode 5>
 */
bool
m_ldc (bool doLowerCase)
{
  if (Input_Match ('2', false))
    return dstmem ("LDC2", doLowerCase, true,
                   0xe0000000 | (Input_Match (doLowerCase ? 'l' : 'L', false) ? N_FLAG : 0) | 0x0c100000, true);

  ARMWord cc = Option_CondL (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  return dstmem ("LDC", doLowerCase, false, cc | 0x0c100000, true);
}

/**
 * Implements STC.
 * STC{<cond>}{L} <coproc>, <CRd>, <addressing mode 5> (pre-UAL)
 * STC{L}{<cond>} <coproc>, <CRd>, <addressing mode 5> (pre-UAL)
 * STC2{L} <coproc>, <CRd>, <addressing mode 5>
 */
bool
m_stc (bool doLowerCase)
{
  if (Input_Match ('2', false))
    return dstmem ("STC2", doLowerCase, true,
                   0xe0000000 | (Input_Match (doLowerCase ? 'l' : 'L', false) ? N_FLAG : 0) | 0x0c000000, false);

  ARMWord cc = Option_CondL (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  return dstmem ("STC", doLowerCase, false, cc | 0x0c000000, false);
}
