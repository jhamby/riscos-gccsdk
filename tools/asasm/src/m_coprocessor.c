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
Get_CoProOpcode (int max, const char *msg)
{
  (void) Input_Match ('#', false);
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


static void
Check_CopNum (bool isTwo, unsigned copNum)
{
  /* FPA uses coprocessor 1 and 2 (the latter only for LFM/SFM).
     NEON/VFP uses coprocessor 10 and 11.  */
  switch (copNum)
    {
      case 1:
      case 2:
	if (option_pedantic)
	  Error (ErrorInfo, "Coprocessor 1 and 2 are FPA floating point unit. Use FPA mnemonics if possible");
	break;
      case 10:
      case 11:
	if (isTwo)
	  Error (ErrorWarning, "Use of coprocessor 10 and 11 is UNDEFINED");
	else if (option_pedantic)
	  Error (ErrorInfo, "Coprocessor 10 and 11 are used for NEON/VFP. Use their mnemonics if possible");
    }
}


/**
 * Parses:
 *   [<cc>][<q>] <coproc>, ["#"]<opcode1>, [<CPd> | <Rt>], <CPn>, <CPm> [,["#"]<opcode2>]
 */
static bool
coprocessor (bool doLowerCase, bool isTwo, bool copOnly, ARMWord ir, int maxop)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  InstrWidth_e instrWidth = Option_GetInstrWidth (doLowerCase);
  if (instrWidth == eInstrWidth_Unrecognized)
    return true;

  unsigned copNum = Get_CopNum ();
  Check_CopNum (isTwo, copNum);
  ir |= CP_NUMBER (copNum);

  Input_SkipWS ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%scoprocessor number", InsertCommaAfter);
  if (maxop > 7)
    ir |= CP_DCODE (Get_CoProOpcode (maxop, "coprocessor opcode"));
  else
    ir |= CP_RTRAN (Get_CoProOpcode (maxop, "coprocessor opcode"));
  Input_SkipWS ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%sdata operand", InsertCommaAfter);
  unsigned reg = copOnly ? Get_CopReg () : Get_CPUReg ();
  ir |= CPDST_OP (reg);
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
    ir |= CP_INFO (Get_CoProOpcode (7, "coprocessor info"));

  InstrType_e instrState = State_GetInstrType ();

  if (!copOnly)
    {
      if (reg == 15)
	Error (ErrorWarning, "The use of R15 is UNPREDICTABLE");
      if (instrState != eInstrType_ARM && reg == 13)
	Error (ErrorWarning, "The use of R13 is UNPREDICTABLE");
    }

  IT_ApplyCond (cc, false, instrState != eInstrType_ARM);

  if (isTwo)
    {
      if (cc != AL)
	{
	  const char *instr = copOnly ? "CDP2" : (ir & 0x00100000) != 0 ? "MRC2" : "MCR2";
	  Error (ErrorError, "%s can not have a non-AL condition code", instr);
	}
      ir |= NV;
    }
  else if (instrState != eInstrType_ARM)
    ir |= AL;
  else
    ir |= cc;

  /* For Thumb2, we need at least ARMv6T2.
     CDP2, MCR2, MRC2 for ARM need at least ARMv5T.  */
  if (instrState != eInstrType_ARM)
    Target_CheckCPUFeature (kCPUExt_v6T2, true);
  else if (isTwo)
    Target_CheckCPUFeature (kCPUExt_v5T, true);

  if (instrState != eInstrType_ARM && instrWidth == eInstrWidth_Enforce16bit)
    Error (ErrorError, "Narrow instruction qualifier for Thumb is not possible");

  Put_Ins (4, ir);
  return false;
}


/**
 * Implements CDP and CDP2.
 *   CDP["2"][<cc>][<q>] <coproc>, ["#"]<opcode1>, <CPd>, <CPn>, <CPm> [, ["#"]<opcode2>]
 */
bool
m_cdp (bool doLowerCase)
{
  bool isTwo = Input_Match ('2', false);
  return coprocessor (doLowerCase, isTwo, true, 0x0e000000, 15);
}


/**
 * Implements MCR and MCR2.
 *   MRC["2"][<cc>][<q>] <coproc>, ["#"]<opcode1>, <Rt>, <CRn>, <CRm> [, ["#"]<opcode2>]
 */
bool
m_mcr (bool doLowerCase)
{
  bool isTwo = Input_Match ('2', false);
  return coprocessor (doLowerCase, isTwo, false, 0x0e000010, 7);
}


/**
 * Implements MRC and MRC2.
 *   MCR["2"][<cc>][<q>] <coproc>, ["#"]<opcode1>, <Rt>, <CRn>, <CRm> [, ["#"]<opcode2>]
 */
bool
m_mrc (bool doLowerCase)
{
  bool isTwo = Input_Match ('2', false);
  return coprocessor (doLowerCase, isTwo, false, 0x0e100010, 7);
}


/**
 * Parses:
 *   [<cc>][<q>] <coproc>, ["#"]<opcode1>, <Rt>, <Rt2>, <CPm>
 */
static bool
coprocessorr (bool doLowerCase, bool isTwo, ARMWord ir)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  InstrWidth_e instrWidth = Option_GetInstrWidth (doLowerCase);
  if (instrWidth == eInstrWidth_Unrecognized)
    return true;

  unsigned copNum = Get_CopNum ();
  Check_CopNum (isTwo, copNum);
  ir |= CP_NUMBER (copNum);

  Input_SkipWS ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%scoprocessor number", InsertCommaAfter);
  ir |= Get_CoProOpcode (15, "coprocessor opcode") << 4;
  Input_SkipWS ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%scoprocessor opcode", InsertCommaAfter);
  unsigned regT = Get_CPUReg ();
  ir |= CPDST_OP (regT);
  Input_SkipWS ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%sdst", InsertCommaAfter);
  unsigned regT2 = Get_CPUReg ();
  ir |= CPLHS_OP (regT2);
  Input_SkipWS ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%slhs", InsertCommaAfter);
  ir |= CPRHS_OP (Get_CopReg ());

  InstrType_e instrState = State_GetInstrType ();

  if (regT == 15 || regT2 == 15)
    Error (ErrorWarning, "The use of R15 is UNPREDICTABLE");
  if (instrState != eInstrType_ARM && (regT == 13 || regT2 == 13))
    Error (ErrorWarning, "The use of R13 is UNPREDICTABLE");

  IT_ApplyCond (cc, false, instrState != eInstrType_ARM);

  if (isTwo)
    {
      if (cc != AL)
	Error (ErrorError, "%s can not have a non-AL condition code", (ir & 0x0FF00000) == 0x0C400000 ? "MCRR2" : "MRRC2");
      ir |= NV;
    }
  else if (instrState != eInstrType_ARM)
    ir |= AL;
  else
    ir |= cc;

  /* For Thumb2, we need at least ARMv6T2.
     MRRC/MCRR for ARM needs at least ARMv5TE,
     MRRC2/MCRR2 for ARM needs at least ARMv6.  */
  if (instrState != eInstrType_ARM)
    Target_CheckCPUFeature (kCPUExt_v6T2, true);
  else
    Target_CheckCPUFeature (isTwo ? kCPUExt_v6 : kCPUExt_v5E, true);

  if (instrState != eInstrType_ARM && instrWidth == eInstrWidth_Enforce16bit)
    Error (ErrorError, "Narrow instruction qualifier for Thumb is not possible");

  Put_Ins (4, ir);
  return false;
}


/**
 * Implements MCRR and MCRR2.
 *   MCRR["2"][<cc>][<q>] <coproc>, ["#"]<opcode1>, <Rt>, <Rt2>, <CPm>
 */
bool
m_mcrr (bool doLowerCase)
{
  bool isTwo = Input_Match ('2', false);
  return coprocessorr (doLowerCase, isTwo, 0x0C400000);
}


/**
 * Implements MRRC and MRRC2.
 *   MRRC["2"][<cc>][<q>] <coproc>, ["#"]<opcode1>, <Rt>, <Rt2>, <CPm>
 */
bool
m_mrrc (bool doLowerCase)
{
  bool isTwo = Input_Match ('2', false);
  return coprocessorr (doLowerCase, isTwo, 0x0C500000);
}


/**
 * For LDC/LDC2/STC/STC2 usage:
 * Parses:
 *   [<cc>]["L"][<q>] <coproc>, <CRd>, <addressing mode 5> (pre-UAL)
 *   ["L"][<cc>][<q>] <coproc>, <CRd>, <addressing mode 5> (UAL)
 */
static bool
dstmem (bool doLowerCase, bool isTwo, ARMWord ir, bool literal)
{
  ARMWord cc = Option_CondL (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  InstrWidth_e instrWidth = Option_GetInstrWidth (doLowerCase);
  if (instrWidth == eInstrWidth_Unrecognized)
    return true;

  unsigned copNum = Get_CopNum ();
  Check_CopNum (isTwo, copNum);
  ir |= CP_NUMBER (copNum);

  Input_SkipWS ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%scoprocessor number", InsertCommaAfter);
  ir |= CPDST_OP (Get_CopReg ());

  InstrType_e instrState = State_GetInstrType ();
  IT_ApplyCond (cc, false, instrState != eInstrType_ARM); 

  ir |= cc & ~NV; /* Copy "L" bit.  */
  if (isTwo)
    {
      if (cc != AL)
	Error (ErrorError, "%s can not have a non-AL condition code", literal ? "LDC2" : "STC2");
      ir |= NV;
    }
  else if (instrState != eInstrType_ARM)
    ir |= AL;
  else
    ir |= cc & NV;

  /* For Thumb2, we need at least ARMv6T2.
     STC2, LDC2 for ARM need at least ARMv5T.  */
  if (instrState != eInstrType_ARM)
    Target_CheckCPUFeature (kCPUExt_v6T2, true);
  else if (isTwo)
    Target_CheckCPUFeature (kCPUExt_v5T, true);

  if (instrState != eInstrType_ARM && instrWidth == eInstrWidth_Enforce16bit)
    Error (ErrorError, "Narrow instruction qualifier for Thumb is not possible");

  HelpCop_Addr (ir, literal, false, instrState != eInstrType_ARM);
  return false;
}


/**
 * Implements LDC and LDC2.
 *   LDC[<cc>]["L"][<q>] <coproc>, <CRd>, <addressing mode 5> (pre-UAL)
 *   LDC["2"]["L"][<cc>][<q>] <coproc>, <CRd>, <addressing mode 5> (UAL)
 */
bool
m_ldc (bool doLowerCase)
{
  bool isTwo = Input_Match ('2', false);
  return dstmem (doLowerCase, isTwo, 0x0c100000, true);
}


/**
 * Implements STC.
 *   STC[<cc>]["L"][<q>] <coproc>, <CRd>, <addressing mode 5> (pre-UAL)
 *   STC["2"]["L"][<cc>][<q>] <coproc>, <CRd>, <addressing mode 5> (UAL)
 */
bool
m_stc (bool doLowerCase)
{
  bool isTwo = Input_Match ('2', false);
  return dstmem (doLowerCase, isTwo, 0x0c000000, false);
}
