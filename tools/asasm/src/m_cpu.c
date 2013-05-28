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
#include <assert.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif

#include "error.h"
#include "expr.h"
#include "fix.h"
#include "get.h"
#include "global.h"
#include "input.h"
#include "it.h"
#include "main.h"
#include "m_cpu.h"
#include "option.h"
#include "put.h"
#include "state.h"
#include "targetcpu.h"

typedef enum
{
  eIsMOVT,
  eIsMOVW,
  eIsMOV32
} MOV_Type_e;

/** DATA none (or optional register) **/

/**
 * Implements NOP.
 * Pre-UAL:
 *   NOP (no condition code !)
 *     MOV R0, R0 (ARM code)
 *     MOV R8, R8 (Thumb code)
 * UAL and with target ARMv6K, ARMv6T2 and ARMv7
 *   NOP<cond> : own encoding allowing a condition code
 */
bool
m_nop (bool doLowerCase)
{
  if (Target_CheckCPUFeature (kCPUExt_v6K, false)
      || Target_CheckCPUFeature (kCPUExt_v6T2, false))
    {
      ARMWord cc = Option_Cond (doLowerCase);
      if (cc == kOption_NotRecognized)
	return true;

      InstrWidth_e instrWidth = Option_GetInstrWidth (doLowerCase);
      if (instrWidth == eInstrWidth_Unrecognized)
	return true;

      InstrType_e instrState = State_GetInstrType ();
      IT_ApplyCond (cc, instrState != eInstrType_ARM); 

      if (instrState == eInstrType_ARM)
	Put_Ins (4, 0x0320F000 | cc);
      else if (instrWidth == eInstrWidth_Enforce32bit)
	{
	  Target_CheckCPUFeature (kCPUExt_v6T2, true);
	  Put_Ins (2, 0xF3AF);
	  Put_Ins (2, 0x8000);
	}
      else
	{
	  Target_CheckCPUFeature (kCPUExt_v4T, true);
	  Put_Ins (2, 0xBF00);
	}
    }
  else
    {
      if (!Input_IsEndOfKeyword ())
	return true;
      InstrType_e instrState = State_GetInstrType ();
      if (instrState == eInstrType_ARM)
	Put_Ins (4, 0xE1A00000); /* MOV R0, R0 */
      else
	Put_Ins (2, 0x46c0); /* MOV R8, R8 */
    }

  return false;
}

/**
 * Implements UND.
 *   UND{cond}{.W} {#expr}
 */
bool
m_und (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  InstrWidth_e instrWidth = Option_GetInstrWidth (doLowerCase);
  if (instrWidth == eInstrWidth_Unrecognized)
    return true;

  Input_SkipWS ();
  unsigned intValue;
  if (!Input_IsEolOrCommentStart ())
    {
      if (!Input_Match ('#', false))
	Error (ErrorError, "Missing #");
      const Value *im = Expr_BuildAndEval (ValueInt);
      if (im->Tag != ValueInt)
	{
	  Error (ErrorError, "Failed to evaluate immediate constant");
	  intValue = 0;
	}
      else
	intValue = (unsigned)im->Data.Int.i;
    }
  else
    intValue = 0;

  InstrType_e instrState = State_GetInstrType ();
  IT_ApplyCond (cc, instrState != eInstrType_ARM);

  unsigned maxValue;
  if (instrState == eInstrType_ARM)
    maxValue = 65536;
  else
    {
      if (instrWidth == eInstrWidth_NotSpecified)
	instrWidth = intValue < 256 ? eInstrWidth_Enforce16bit : eInstrWidth_Enforce32bit; 
      maxValue = instrWidth == eInstrWidth_Enforce16bit ? 256 : 4096;
    }
  if (intValue >= maxValue)
    {
      Error (ErrorError, "Expression value %u is too big to be encoded (max value is %u)",
	     intValue, maxValue - 1);
      intValue = maxValue - 1;
    }

  if (instrState == eInstrType_ARM)
    Put_Ins (4, 0x07F000F0 | cc | ((intValue & 0xFFF0)<<4) | (intValue & 0xF));
  else if (instrWidth == eInstrWidth_Enforce32bit)
    {
      Target_CheckCPUFeature (kCPUExt_v6T2, true);
      Put_Ins (4, 0xF7F0A0F0 | ((intValue & 0xF00)<<8) | ((intValue & 0xF0)<<4) | (intValue & 0xF));
    }
  else
    {
      Target_CheckCPUFeature (kCPUExt_v4T, true);
      Put_Ins (2, 0xDE00 | intValue);
    }
  
  return false;
}

/** DATA dst = lhs <op> rhs **/

static bool
dstlhsrhs (ARMWord ir)
{
  unsigned op = Get_CPUReg ();
  ir |= DST_OP (op);
  Input_SkipWS ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%sdst", InsertCommaAfter);
  op = Get_CPUReg ();
  ir |= LHS_OP (op);
  Input_SkipWS ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%slhs", InsertCommaAfter);
  ir = Get_RHS (true, true, ir);
  Put_Ins (4, ir);
  return false;
}

static bool
dstrhs (ARMWord ir)
{
  unsigned op = Get_CPUReg ();
  ir |= DST_OP (op);
  Input_SkipWS ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%sdst", InsertCommaAfter);
  ir = Get_RHS (true, true, ir);
  Put_Ins (4, ir);
  return false;
}


/**
 * Implements ADC.
 */
bool
m_adc (bool doLowerCase)
{
  ARMWord cc = Option_CondS (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  return dstlhsrhs (cc | M_ADC);
}

/**
 * Implements ADD.
 */
bool
m_add (bool doLowerCase)
{
  ARMWord cc = Option_CondS (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  return dstlhsrhs (cc | M_ADD);
}

/**
 * Implements AND.
 */
bool
m_and (bool doLowerCase)
{
  ARMWord cc = Option_CondS (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  return dstlhsrhs (cc | M_AND);
}

/**
 * Implements BIC.
 */
bool
m_bic (bool doLowerCase)
{
  ARMWord cc = Option_CondS (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  return dstlhsrhs (cc | M_BIC);
}

/**
 * Implements EOR.
 */
bool
m_eor (bool doLowerCase)
{
  ARMWord cc = Option_CondS (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  return dstlhsrhs (cc | M_EOR);
}


/**
 *   MOVW<c> <Rd>, #<imm16>
 *   MOVT<c> <Rd>, #<imm16>
 *   MOV32<c> <Rd>, #<imm32>
 */
static bool
m_movw_movt_mov32 (bool doLowerCase, MOV_Type_e movType)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  unsigned destReg = Get_CPUReg ();
  Input_SkipWS ();
  if (!Input_Match (',', true))
    {
      Error (ErrorError, "%sdst", InsertCommaAfter);
      return false;
    }
  if (!Input_Match ('#', false))
    {
      Error (ErrorError, "Missing immediate constant");
      return false;
    }
  uint32_t immValue = 0;
  const Value *im = Expr_BuildAndEval (ValueInt); /* FIXME: *** NEED ValueSymbol & ValueCode */
  switch (im->Tag)
    {
      case ValueInt:
	if (im->Data.Int.type == eIntType_PureInt)
	  {
	    immValue = im->Data.Int.i;
	    break;
	  }
	/* Fall through.  */

      default:
	/* During pass one, we discard any errors of the evaluation as it
	   might contain unresolved symbols.  Wait until during pass two.  */
	if (gPhase != ePassOne)
	  Error (ErrorError, "Illegal immediate expression");
	break;
    }

  switch (movType)
    {
      case eIsMOVT:
      case eIsMOVW:
	if (immValue >= 0x10000)
	  {
	    Error (ErrorError, "Value 0x%x is out of range", immValue);
	    immValue &= 0xFFFF;
	  }
	Put_Ins_MOVW_MOVT (cc, destReg, immValue, movType == eIsMOVT);
	break;

      case eIsMOV32:
	Put_Ins_MOVW_MOVT (cc, destReg, immValue & 0xFFFF, false); /* MOVW */
	Put_Ins_MOVW_MOVT (cc, destReg, immValue >> 16, true); /* MOVT */
	break;
    }

  return false;
}


/**
 * Implements MOV, MOVW, MOVT and MOV32.
 * MOVW/MOVT/MOV32 are only supported for ARMv6T2 and ARMv7. 
 */
bool
m_mov (bool doLowerCase)
{
  if (Target_CheckCPUFeature (kCPUExt_v6T2, false))
    {
      /* MOVW ? */
      if (Input_Match (doLowerCase ? 'w' : 'W', false))
	return m_movw_movt_mov32 (doLowerCase, eIsMOVW);
      /* MOVT ? */
      if (Input_Match (doLowerCase ? 't' : 'T', false))
	return m_movw_movt_mov32 (doLowerCase, eIsMOVT);
      /* MOV32 ? */
      if (Input_LookN (0) == '3' && Input_LookN (1) == '2')
	{
	  Input_SkipN (2);
	  return m_movw_movt_mov32 (doLowerCase, eIsMOV32);
	}
    }

  ARMWord cc = Option_CondS (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  return dstrhs (cc | M_MOV);
}

/**
 * Implements MVN.
 */
bool
m_mvn (bool doLowerCase)
{
  ARMWord cc = Option_CondS (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  return dstrhs (cc | M_MVN);
}

/**
 * Implements ORR.
 */
bool
m_orr (bool doLowerCase)
{
  ARMWord cc = Option_CondS (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  return dstlhsrhs (cc | M_ORR);
}

/**
 * Implements RSB.
 */
bool
m_rsb (bool doLowerCase)
{
  ARMWord cc = Option_CondS (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  return dstlhsrhs (cc | M_RSB);
}

/**
 * Implements RSC.
 */
bool
m_rsc (bool doLowerCase)
{
  ARMWord cc = Option_CondS (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  return dstlhsrhs (cc | M_RSC);
}

/**
 * Implements SBC.
 */
bool
m_sbc (bool doLowerCase)
{
  ARMWord cc = Option_CondS (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  return dstlhsrhs (cc | M_SBC);
}

/**
 * Implements SUB.
 */
bool
m_sub (bool doLowerCase)
{
  ARMWord cc = Option_CondS (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  return dstlhsrhs (cc | M_SUB);
}

/** DATA test **/

static bool
lhsrhs (ARMWord ir)
{
  unsigned op = Get_CPUReg ();
  ir |= LHS_OP (op);
  Input_SkipWS ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%slhs", InsertCommaAfter);
  ir = Get_RHS (true, true, ir);
  Put_Ins (4, ir);
  return false;
}

/**
 * Implements CMN.
 */
bool
m_cmn (bool doLowerCase)
{
  ARMWord cc = Option_CondSP (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  return lhsrhs (cc | M_CMN);
}

/**
 * Implements CMP.
 */
bool
m_cmp (bool doLowerCase)
{
  ARMWord cc = Option_CondSP (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  return lhsrhs (cc | M_CMP);
}

/**
 * Implements TEQ.
 */
bool
m_teq (bool doLowerCase)
{
  ARMWord cc = Option_CondSP (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  return lhsrhs (cc | M_TEQ);
}

/**
 * Implements TST.
 */
bool
m_tst (bool doLowerCase)
{
  ARMWord cc = Option_CondSP (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  return lhsrhs (cc | M_TST);
}

/** DATA 1a **/

/**
 * Used for MUL, MLA and MLS.
 */
typedef enum { Is_eMUL, Is_eMLA, Is_eMLS } MulFlavour_e;
static void
onlyregs (MulFlavour_e mulType, ARMWord ir)
{
  /* Read Rd.  */
  unsigned regD = Get_CPUReg ();
  if (regD == 15)
    Error (ErrorWarning, "Use of register PC is unpredictable");

  /* Read Rn (lhs).  */
  Input_SkipWS ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%sdst", InsertCommaAfter);
  unsigned regN = Get_CPUReg ();
  if (regN == 15)
    Error (ErrorWarning, "Use of register PC is unpredictable");

  /* Read Rm (rhs).  */
  Input_SkipWS ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%slhs", InsertCommaAfter);
  unsigned regM = Get_CPUReg ();
  if (regM == 15)
    Error (ErrorWarning, "Use of register PC is unpredictable");
  if (mulType != Is_eMLS)
    {
      if (regD == regN && !Target_CheckCPUFeature (kCPUExt_v6, false))
	{
	  if (regN == regM)
	    Error (ErrorError, "Destination and left operand are the same register %d", regD);
	  else
	    {
	      Error (ErrorInfo, "Changing order of operands in %s to avoid unpredicability", mulType == Is_eMLA ? "MLA" : "MUL");
	      int t = regN;
	      regN = regM;
	      regM = t;
	    }
	}
    }

  if (mulType != Is_eMUL)
    {
      /* Read Ra.  */
      Input_SkipWS ();
      if (!Input_Match (',', true))
	Error (ErrorError, "%srhs", InsertCommaAfter);
      unsigned regA = Get_CPUReg ();
      if (regA == 15)
	Error (ErrorWarning, "Use of register PC is unpredictable");
      ir |= ACC_MUL (regA);
    }
  ir |= DST_MUL (regD);
  ir |= LHS_MUL (regN);
  ir |= RHS_MUL (regM);

  Put_Ins (4, ir);
}

/**
 * Implements MLA.
 *   MLA{S}<cond> <Rd>, <Rn>, <Rm>, <Ra>
 */
bool
m_mla (bool doLowerCase)
{
  ARMWord cc = Option_CondS (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  Target_CheckCPUFeature (kCPUExt_v2, true);
  onlyregs (Is_eMLA, cc | M_MLA);
  return false;
}

/**
 * Implements MLS.
 *   MLS<cond> <Rd>, <Rn>, <Rm>, <Ra>
 */
bool
m_mls (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase); /* Note, no 'S' */
  if (cc == kOption_NotRecognized)
    return true;

  Target_CheckCPUFeature (kCPUExt_v6T2, true);
  onlyregs (Is_eMLS, cc | M_MLS);
  return false;
}

/**
 * Implements MUL.
 *   MUL{S}<cond> <Rd>, <Rn>, <Rm>, <Ra>
 */
bool
m_mul (bool doLowerCase)
{
  ARMWord cc = Option_CondS (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  Target_CheckCPUFeature (kCPUExt_v2, true);
  onlyregs (Is_eMUL, cc | M_MUL);
  return false;
}


/**
 * Used for implementing SMUL*, SMLA*, UMUL*, UMLA.
 */
static void
l_onlyregs (ARMWord ir, const char *op)
{
  /* This bit only set for smulxx */
  bool issmull = !(ir & 0x01000000);
  bool issmlaxy = (ir & 0x00600000) == 0;
  bool issmlalxy = (ir & 0x00600000) == 0x00400000;
  bool issmlawy = (ir & 0x00600020) == 0x00200000;

  unsigned dstl;
  if (issmull)
    {
      Target_CheckCPUFeature (kCPUExt_v4, true);
      dstl = Get_CPUReg ();
      if (!Input_Match (',', true))
        Error (ErrorError, "%sdst_h", InsertCommaAfter);
    }
  else
    {
      Target_CheckCPUFeature (kCPUExt_v5ExP, true);
      if (issmlalxy)
        {
          dstl = Get_CPUReg ();
	  if (!Input_Match (',', true))
            Error (ErrorError, "%sdst_l", InsertCommaAfter);
        }
      else
        dstl = 0;
    }

  unsigned dsth = Get_CPUReg ();
  Input_SkipWS ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%sdst_l", InsertCommaAfter);
  unsigned lhs = Get_CPUReg ();
  Input_SkipWS ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%slhs", InsertCommaAfter);
  unsigned rhs = Get_CPUReg ();
  if (issmull)
    {
      if (dstl == dsth)
        Error (ErrorError, "Destination high and low are the same register %d", dstl);
      if ((dstl == lhs || dsth == lhs) && !Target_CheckCPUFeature (kCPUExt_v6, false))
        {
          if (dstl == rhs || dsth == rhs) 
            Error (ErrorError, "Left operand register %d also occurs in destination", lhs);
          else
	    {
	      if (option_fussy)
	        Error (ErrorInfo, "Changing order of operands in %s", op);
	      int t = lhs;
	      lhs = rhs;
	      rhs = t;
	    }
        }
     }
  else
    {
      if (dsth == 15 || lhs == 15 || rhs == 15)
	Error (ErrorError, "Cannot use R15 with %s", op);
    }
  if (!issmull && (issmlaxy || issmlawy))
    {
      if (!Input_Match (',', true))
        Error (ErrorError, "%sdst_l", InsertCommaAfter);
      dstl = Get_CPUReg ();
      if (dstl == 15)
        Error (ErrorError, "Cannot use R15 with %s", op);
    }

  ir |= dstl << 12 | dsth << 16 | lhs | rhs << 8;
  Put_Ins (4, ir);
}

/**
 * Implements SMULL.
 */
bool
m_smull (bool doLowerCase)
{
  ARMWord cc = Option_CondS (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  Target_CheckCPUFeature (kArchExt_v3M, true);
  l_onlyregs (cc | M_SMULL, "SMULL");
  return false;
}

/**
 * Implements SMULBB.
 */
bool
m_smulbb (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  Target_CheckCPUFeature (kCPUExt_v5ExP, true);
  l_onlyregs (cc | M_SMULBB, "SMULBB");
  return false;
}

/**
 * Implements SMULBT.
 */
bool
m_smulbt (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  Target_CheckCPUFeature (kCPUExt_v5ExP, true);
  l_onlyregs (cc | M_SMULBT, "SMULBT");
  return false;
}

/**
 * Implements SMULTB.
 */
bool
m_smultb (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  Target_CheckCPUFeature (kCPUExt_v5ExP, true);
  l_onlyregs (cc | M_SMULTB, "SMULTB");
  return false;
}

/**
 * Implements SMULTT.
 */
bool
m_smultt (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  Target_CheckCPUFeature (kCPUExt_v5ExP, true);
  l_onlyregs (cc | M_SMULTT, "SMULTT");
  return false;
}

/**
 * Implements SMULWB.
 */
bool
m_smulwb (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  Target_CheckCPUFeature (kCPUExt_v5ExP, true);
  l_onlyregs (cc | M_SMULWB, "SMULWB");
  return false;
}

/**
 * Implements SMULWT.
 */
bool
m_smulwt (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  Target_CheckCPUFeature (kCPUExt_v5ExP, true);
  l_onlyregs (cc | M_SMULWT, "SMULWT");
  return false;
}

/**
 * Implements SMLAL.
 */
bool
m_smlal (bool doLowerCase)
{
  ARMWord cc = Option_CondS (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  Target_CheckCPUFeature (kCPUExt_v3M, true);
  l_onlyregs (cc | M_SMLAL, "SMLAL");
  return false;
}

/**
 * Implements SMLALBB.
 */
bool
m_smlalbb (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  Target_CheckCPUFeature (kCPUExt_v5ExP, true);
  l_onlyregs (cc | M_SMLALBB, "SMLALBB");
  return false;
}

/**
 * Implements SMLALBT.
 */
bool
m_smlalbt (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  Target_CheckCPUFeature (kCPUExt_v5ExP, true);
  l_onlyregs (cc | M_SMLALBT, "SMLALBT");
  return false;
}

/**
 * Implements SMLALTB.
 */
bool
m_smlaltb (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  Target_CheckCPUFeature (kCPUExt_v5ExP, true);
  l_onlyregs (cc | M_SMLALTB, "SMLALTB");
  return false;
}

/**
 * Implements SMLALTT.
 */
bool
m_smlaltt (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  Target_CheckCPUFeature (kCPUExt_v5ExP, true);
  l_onlyregs (cc | M_SMLALTT, "SMLALTT");
  return false;
}

/**
 * Implements SMLABB.
 */
bool
m_smlabb (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  Target_CheckCPUFeature (kCPUExt_v5ExP, true);
  l_onlyregs (cc | M_SMLABB, "SMLABB");
  return false;
}

/**
 * Implements SMLABT.
 */
bool
m_smlabt (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  Target_CheckCPUFeature (kCPUExt_v5ExP, true);
  l_onlyregs (cc | M_SMLABT, "SMLABT");
  return false;
}

/**
 * Implements SMLATB.
 */
bool
m_smlatb (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  Target_CheckCPUFeature (kCPUExt_v5ExP, true);
  l_onlyregs (cc | M_SMLATB, "SMLATB");
  return false;
}

/**
 * Implements SMLATT.
 */
bool
m_smlatt (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  Target_CheckCPUFeature (kCPUExt_v5ExP, true);
  l_onlyregs (cc | M_SMLATT, "SMLATT");
  return false;
}

/**
 * Implements SMLAWB.
 */
bool
m_smlawb (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  Target_CheckCPUFeature (kCPUExt_v5ExP, true);
  l_onlyregs (cc | M_SMLAWB, "SMLAWB");
  return false;
}

/**
 * Implements SMLAWT.
 */
bool
m_smlawt (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  Target_CheckCPUFeature (kCPUExt_v5ExP, true);
  l_onlyregs (cc | M_SMLAWT, "SMLAWT");
  return false;
}

/**
 * Implements UMULL.
 */
bool
m_umull (bool doLowerCase)
{
  ARMWord cc = Option_CondS (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  Target_CheckCPUFeature (kCPUExt_v3M, true);
  l_onlyregs (cc | M_UMULL, "UMULL");
  return false;
}

/**
 * Implements UMLAL.
 */
bool
m_umlal (bool doLowerCase)
{
  ARMWord cc = Option_CondS (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  Target_CheckCPUFeature (kCPUExt_v3M, true);
  l_onlyregs (cc | M_UMLAL, "UMLAL");
  return false;
}


/**
 * Implements CLZ.
 */
bool
m_clz (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  Target_CheckCPUFeature (kCPUExt_v5, true);

  ARMWord ir = cc | M_CLZ;

  unsigned dst = Get_CPUReg ();
  ir |= DST_OP (dst);
  Input_SkipWS ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%slhs", InsertCommaAfter);

  unsigned rhs = Get_CPUReg ();
  ir |= RHS_OP (rhs);

  if (dst == 15 || rhs == 15)
    Error (ErrorError, "Use of R15 in CLZ is unpredictable");

  Put_Ins (4, ir);
  return false;
}

static void
q_onlyregs (ARMWord ir, const char *op)
{
  Target_CheckCPUFeature (kCPUExt_v5ExP, true);

  Input_SkipWS ();

  unsigned dst = Get_CPUReg ();
  Input_SkipWS ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%sdst", InsertCommaAfter);
  unsigned lhs = Get_CPUReg ();
  Input_SkipWS ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%slhs", InsertCommaAfter);
  unsigned rhs = Get_CPUReg ();
  if (dst == 15 || lhs == 15 || rhs == 15)
    Error (ErrorError, "Cannot use R15 with %s", op);

  ir |= dst << 12 | lhs | rhs << 16;
  Put_Ins (4, ir);
}

/**
 * Implements QADD.
 */
bool
m_qadd (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  q_onlyregs (cc | M_QADD, "QADD");
  return false;
}

/**
 * Implements QDADD.
 */
bool
m_qdadd (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  q_onlyregs (cc | M_QDADD, "QDADD");
  return false;
}

/**
 * Implements QDSUB.
 */
bool
m_qdsub (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  q_onlyregs (cc | M_QDSUB, "QDSUB");
  return false;
}

/**
 * Implements QSUB.
 */
bool
m_qsub (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  q_onlyregs (cc | M_QSUB, "QSUB");
  return false;
}

static bool
UALShift (ARMWord shiftType, bool doLowerCase)
{
  ARMWord cc = Option_SCond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  unsigned regD = Get_CPUReg ();
  if (regD == INVALID_REG)
    return false;
  if (!Input_Match (',', true))
    {
      Error (ErrorError, "Missing ,");
      return false;
    }
  ARMWord regM = Get_CPURegNoError ();
  ARMWord regS;
  if (regM == INVALID_REG)
    {
      regM = regD;
      regS = INVALID_REG;
    }
  else
    {
      if (!Input_Match (',', true))
	{
	  regS = regM;
	  regM = regD;
	}
      else
	regS = Get_CPURegNoError ();
    }

  cc |= M_MOV | DST_OP (regD) | regM;
  if (regS == INVALID_REG)
    {
      if (!Input_Match ('#', false))
	{
	  Error (ErrorError, "Missing immediate constant");
	  return false;
	}
      const Value *im = Expr_BuildAndEval (ValueInt);
      if (im->Tag != ValueInt)
	{
	  Error (ErrorError, "Failed to evaluate immediate constant");
	  return false;
	}
      Put_Ins (4, cc | Fix_ShiftImm (shiftType, im->Data.Int.i));
    }
  else
    Put_Ins (4, cc | SHIFT_REG (regS) | SHIFT_OP (shiftType));
  
  return false;
}

/**
 * Implements ASR.
 *   ASR<S><CC> {Rd,} Rm, #<num> : UAL version of MOV<CC><S> Rd, Rm, ASR#<num>
 *   ASR<S><CC> {Rd,} Rm, Rs : UAL version of MOV<CC><S> Rd, Rm, ASR Rs
 */
bool
m_asr (bool doLowerCase)
{
  return UALShift (ASR, doLowerCase);
}

/**
 * Implements LSL.
 *   LSL<S><CC> {Rd,} Rm, #<num> : UAL version of MOV<CC><S> Rd, Rm, LSL#<num>
 *   LSL<S><CC> {Rd,} Rm, Rn : UAL version of MOV<CC><S> Rd, Rm, LSL Rs
 */
bool
m_lsl (bool doLowerCase)
{
  return UALShift (LSL, doLowerCase);
}

/**
 * Implements LSR.
 *   LSR<S><CC> {Rd,} Rm, #<num> : UAL version of MOV<CC><S> Rd, Rm, LSR#<num>
 *   LSR<S><CC> {Rd,} Rm, Rs : UAL version of MOV<CC><S> Rd, Rm, LSR Rs
 */
bool
m_lsr (bool doLowerCase)
{
  return UALShift (LSR, doLowerCase);
}

/**
 * Implements ROR.
 *   ROR<S><CC> {Rd,} Rm, #<num> : UAL version of MOV<CC><S> Rd, Rm, ROR#<num>
 *   ROR<S><CC> {Rd,} Rm, Rs : UAL version of MOV<CC><S> Rd, Rm, ROR Rs
 */
bool
m_ror (bool doLowerCase)
{
  return UALShift (ROR, doLowerCase);
}

/**
 * Implements RRX.
 *   RRX<S><CC> {Rd,} Rm : UAL version of MOV<CC><S> Rd, Rm, RRX
 */
bool
m_rrx (bool doLowerCase)
{
  ARMWord cc = Option_SCond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  unsigned regD = Get_CPUReg ();
  if (regD == INVALID_REG)
    return false;
  Input_SkipWS ();
  unsigned regM;
  if (Input_Match (',', true))
    {
      regM = Get_CPUReg ();
      if (regM == INVALID_REG)
	return false;
    }
  else
    regM = regD;

  Put_Ins (4, cc | M_MOV | DST_OP (regD) | regM | SHIFT_OP (RRX));
  
  return false;
}

typedef enum
{
  eIsBFC,
  eIsBFI,
  eIsSBFX,
  eIsUBFX
} BitFieldType_e;

/**
 * Implements BFC, BFI, SBFX and UBFX.
 */
static bool
core_bitfield_instr (bool doLowerCase, BitFieldType_e bitFieldType)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  InstrWidth_e instrWidth = Option_GetInstrWidth (doLowerCase);
  if (instrWidth == eInstrWidth_Unrecognized)
    return true;

  /* Only ARM and 32 bit Thumb is possible for BFC/BFI/SBFX/UBFX.  */
  if (instrWidth == eInstrWidth_Enforce16bit)
    Error (ErrorError, "Narrow instruction qualifier for Thumb is not possible");
    
  Target_CheckCPUFeature (kCPUExt_v6T2, true); /* Only ARMv6T2 and ARMv7.  */

  unsigned rd = Get_CPUReg ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%sRd", InsertCommaAfter);
  unsigned rn;
  if (bitFieldType != eIsBFC)
    {
      /* BFI, SBFX and UBFX.  */
      rn = Get_CPUReg ();
      if (!Input_Match (',', true))
	Error (ErrorError, "%sRd", InsertCommaAfter);

      if (rn == 15)
	Error (ErrorWarning, "Use of PC for Rn makes BFI behave like BFC");
    }
  else
    rn = 15;
  if (!Input_Match ('#', false))
    Error (ErrorError, "Missing #");
  unsigned lsb;
  const Value *lsbValue = Expr_BuildAndEval (ValueInt);
  if (lsbValue->Tag != ValueInt)
    {
      Error (ErrorError, "Failed to evaluate immediate constant");
      lsb = 0;
    }
  else
    lsb = lsbValue->Data.Int.i;
  if (!Input_Match (',', true))
    Error (ErrorError, "%sRd", InsertCommaAfter);
  if (!Input_Match ('#', false))
    Error (ErrorError, "Missing #");
  unsigned width;
  const Value *widthValue = Expr_BuildAndEval (ValueInt);
  if (widthValue->Tag != ValueInt)
    {
      Error (ErrorError, "Failed to evaluate immediate constant");
      width = 1;
    }
  else
    width = widthValue->Data.Int.i;

  if (/* lsb < 0 || */ lsb > 32)
    {
      Error (ErrorError, "lsb value needs be in range 0 to 31 (incl)");
      lsb = 0;
    }
  if (width < 1 || width > 32 - lsb)
    {
      Error (ErrorError, "width value needs to be in range 1 to 32 - lsb (incl)");
      width = 1;
    }

  InstrType_e instrState = State_GetInstrType ();
  IT_ApplyCond (cc, instrState != eInstrType_ARM); 

  unsigned widthInd;
  ARMWord baseInstr;
  switch (bitFieldType)
    {
      case eIsBFC:
      case eIsBFI:
	{
	  widthInd = lsb + width - 1;
	  if (instrState == eInstrType_ARM)
	    {
	      if (rd == 15)
		Error (ErrorWarning, "Use of PC is unpredictable");
	      if (rd == 13 || rn == 13)
		Error (ErrorWarning, "Use of R13 as Rd or Rn is deprecated");
	      baseInstr = 0x07C00010;
	    }
	  else
	    {
	      if (rd == 13 || rd == 15 || rn == 13)
		Error (ErrorWarning, "Use of R13 or PC is unpredictable");
	      baseInstr = 0xF3600000;
	    }
	  break;
	}

      case eIsSBFX:
      case eIsUBFX:
	{
	  widthInd = width - 1;
	  if (instrState == eInstrType_ARM)
	    {
	      if (rd == 15 || rn == 15)
		Error (ErrorWarning, "Use of PC in unpredictable");
	      if (rd == 13 || rn == 13)
		Error (ErrorWarning, "Use of R13 as Rd or Rn is deprecated");
	      baseInstr = bitFieldType == eIsSBFX ? 0x07A00050 : 0x07E00050;
	    }
	  else
	    {
	      if (rd == 13 || rd == 15 || rn == 13 || rn == 15)
		Error (ErrorWarning, "Use of R13 or PC is unpredictable");
	      baseInstr = bitFieldType == eIsSBFX ? 0xF3400000 : 0xF3C00000;
	    }
	  break;
	}
    }

  if (instrState == eInstrType_ARM)
    Put_Ins (4, baseInstr | cc | (widthInd << 16) | (rd << 12) | (lsb << 7) | rn);
  else
    Put_Ins (4, baseInstr | (rn << 16) | ((lsb & 0x1C) << 12) | (rd << 8) | ((lsb & 3) << 6) | widthInd);
  
  return false;
}


/**
 * Implements BFC
 *   BFC{<c>}{<q>} <Rd>, #<lsb>, #<width>
 */
bool
m_bfc (bool doLowerCase)
{
  return core_bitfield_instr (doLowerCase, eIsBFC);
}


/**
 * Implements BFI
 *   BFI{<c>}{<q>} <Rd>, <Rn>, #<lsb>, #<width>
 */
bool
m_bfi (bool doLowerCase)
{
  return core_bitfield_instr (doLowerCase, eIsBFI);
}


/**
 * Implements SBFX
 *   SBFX{<c>}{<q>} <Rd>, <Rn>, #<lsb>, #<width>
 */
bool
m_sbfx (bool doLowerCase)
{
  return core_bitfield_instr (doLowerCase, eIsSBFX);
}


/**
 * Implements UBFX
 *   UBFX{<c>}{<q>} <Rd>, <Rn>, #<lsb>, #<width>
 */
bool
m_ubfx (bool doLowerCase)
{
  return core_bitfield_instr (doLowerCase, eIsUBFX);
}


/**
 * Implements PKHBT and PKHTB
 *   PKHBT{<c>}{<q>} {<Rd>}, <Rn>, <Rm>{, LSL #<imm>}
 *   PKHTB{<c>}{<q>} {<Rd>}, <Rn>, <Rm>{, ASR #<imm>}
 */
bool
m_pkh (bool doLowerCase)
{
  bool isTB;
  if (Input_MatchString (doLowerCase ? "tb" : "TB"))
    isTB = true;
  else if (Input_MatchString (doLowerCase ? "bt" : "BT"))
    isTB = false;
  else
    return true;

  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  InstrWidth_e instrWidth = Option_GetInstrWidth (doLowerCase);
  if (instrWidth == eInstrWidth_Unrecognized)
    return true;

  if (instrWidth == eInstrWidth_Enforce16bit)
    {
      Error (ErrorError, "Narrow instruction qualifier for Thumb is not possible");
      instrWidth = eInstrWidth_NotSpecified;
    }

  InstrType_e instrState = State_GetInstrType ();

  /* These ARM instructions are available in ARMv6 and above.
     These 32-bit Thumb instructions are available in ARMv6T2 and above. For
     the ARMv7-M architecture, they are only available in an ARMv7E-M
     implementation.  */
  uint64_t cpuFeatures;
  if (instrState == eInstrType_ARM)
    cpuFeatures = kCPUExt_v6;
  else if ((Target_GetCPUFeatures () & kCPUExt_v7M) == 0)
    cpuFeatures = kCPUExt_v6T2;
  else
    cpuFeatures = kCPUExt_v6_DSP;
  Target_CheckCPUFeature (cpuFeatures, true);

  ARMWord regs[3];
  const unsigned maxNumRegs = 3;
  unsigned regIndex = 0;
  bool pendingComma = false;
  while (regIndex != maxNumRegs)
    {
      regs[regIndex] = Get_CPURegNoError ();
      if (regs[regIndex] == INVALID_REG)
	break;
      ++regIndex;
      Input_SkipWS ();
      pendingComma = Input_Match (',', true);
      if (!pendingComma)
	break;
    }
  unsigned shift = 0;
  if (regIndex < maxNumRegs - 1)
    {
      Error (ErrorError, "Not enough registers specified");
      while (regIndex != maxNumRegs)
	regs[regIndex++] = 0;
    }
  else if (pendingComma)
    {
      /* We expect to parse a LSL#<value> (isTB = false) or ASR#<value>
	 (isTB = true).  */
      Input_SkipWS ();
      const char * const shiftType = isTB ? "ASR" : "LSL";
      if (!Input_MatchString (shiftType))
	Error (ErrorError, "%s expected", shiftType);
      else
	{
	  Input_SkipWS ();
	  if (!Input_Match ('#', false))
	    Error (ErrorError, "%s #<value> expected", shiftType);
	  else
	    {
	      const Value *shiftValue = Expr_BuildAndEval (ValueInt);
	      /* TB : shift value from 1 to 32 (incl).
		 BT: shift value from 0 to 31 (incl).  */
	      if (shiftValue->Tag != ValueInt
	          || (isTB && (shiftValue->Data.Int.i < 1 || shiftValue->Data.Int.i > 32))
	          || (!isTB && (shiftValue->Data.Int.i < 0 || shiftValue->Data.Int.i > 31)))
		Error (ErrorError, "Wrong %s #<value>", shiftType);
	      else
		shift = shiftValue->Data.Int.i;
	    }
	}
    }

  /* Determine <Rn>, <Rm> and <Rd>. 
     PKHTB{<c>}{<q>} {<Rd>,} <Rn>, <Rm> becomes
     PKHBT{<c>}{<q>} {<Rd>,} <Rm>, <Rn> (i.e. <Rn> and <Rm> swapped).  */
  const ARMWord rm = regs[isTB && shift == 0 ? regIndex - 2 : regIndex - 1];
  const ARMWord rn = regs[isTB && shift == 0 ? regIndex - 1 : regIndex - 2];
  regIndex -= 2;
  const ARMWord rd = regIndex == 0 ? rn : regs[--regIndex];
  assert (regIndex == 0);

  if (isTB)
    {
      if (shift == 0)
	isTB = false;
      else if (shift == 32)
	shift = 0;
    }

  IT_ApplyCond (cc, instrState != eInstrType_ARM);

  if (instrState == eInstrType_ARM)
    {
      if (rd == 15 || rn == 15 || rm == 15)
	Error (ErrorWarning, "Use of PC for Rd, Rn or Rm is unpredictable");
      const ARMWord baseInstr = 0x06800010;
      Put_Ins (4, baseInstr | cc | (rn << 16) | (rd << 12) | (shift << 7) | (isTB ? (1<<6) : 0) | rm);
    }
  else
    {
      if (rd == 15 || rd == 13 || rn == 15 || rn == 13 || rm == 15 || rm == 13)
	Error (ErrorWarning, "Use of R13 or PC for Rd, Rn or Rm is unpredictable");
      const ARMWord baseInstr = 0xEAC00000;
      Put_Ins (4, baseInstr | (rn << 16) | ((shift & 0x1C) << 10) | (rd << 8) | ((shift & 3) << 6) | (isTB ? (1<<5) : 0) | rm);
    }

  return false;
}


typedef enum
{
  eIsByte,
  eIsByte16,
  eIsHalfword
} Extend_e;

/**
 * Implements SXT/SXTA/UXT/UXTA.
 */
static bool
core_sxt_uxt (bool doLowerCase, bool isLXT)
{
  bool isAcc = Input_Match (doLowerCase ? 'a' : 'A', false);
  Extend_e extend;
  if (Input_MatchString (doLowerCase ? "b16" : "B16"))
    extend = eIsByte16;
  else if (Input_Match (doLowerCase ? 'b' : 'B', false))
    extend = eIsByte;
  else if (Input_Match (doLowerCase ? 'h' : 'H', false))
    extend = eIsHalfword;
  else
    return true;

  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  InstrWidth_e instrWidth = Option_GetInstrWidth (doLowerCase);
  if (instrWidth == eInstrWidth_Unrecognized)
    return true;

  /* Narrow instruction qualifier is not possible for accumulate versions
     and the B16 non accumulate one.  */
  if (instrWidth == eInstrWidth_Enforce16bit && (isAcc || extend == eIsByte16))
    {
      Error (ErrorError, "Narrow instruction qualifier for Thumb is not possible");
      instrWidth = eInstrWidth_NotSpecified;
    }

  ARMWord regs[3];
  unsigned maxNumRegs = isAcc ? 3 : 2;
  unsigned regIndex = 0;
  bool pendingComma = false;
  while (regIndex != maxNumRegs)
    {
      regs[regIndex] = Get_CPURegNoError ();
      if (regs[regIndex] == INVALID_REG)
	break;
      ++regIndex;
      pendingComma = Input_Match (',', true);
      if (!pendingComma)
	break;
    }
  unsigned ror = 0;
  if (regIndex < maxNumRegs - 1)
    {
      Error (ErrorError, "Not enough registers specified");
      while (regIndex != maxNumRegs)
	regs[regIndex++] = 0;
    }
  else if (pendingComma)
    {
      /* We expect to parse a ROR#<value>.  */
      if (!Input_MatchString ("ROR"))
	Error (ErrorError, "%s expected", "ROR");
      else
	{
	  Input_SkipWS ();
	  if (!Input_Match ('#', false))
	    Error (ErrorError, "%s #<value> expected", "ROR");
	  else
	    {
	      const Value *rorValue = Expr_BuildAndEval (ValueInt);
	      if (rorValue->Tag != ValueInt
	          || (rorValue->Data.Int.i & ~0x18) != 0)
		Error (ErrorError, "Wrong %s #<value>", "ROR");
	      else
		ror = rorValue->Data.Int.i >> 3;
	    }
	}
    }

  const ARMWord rm = regs[--regIndex];
  const ARMWord rn = isAcc ? regs[--regIndex] : 15;
  const ARMWord rd = regIndex == 0 ? regs[regIndex] : regs[--regIndex];
  assert (regIndex == 0);

  InstrType_e instrState = State_GetInstrType ();
  IT_ApplyCond (cc, instrState != eInstrType_ARM);

  /* Determine 16bit or 32bit Thumb.  */
  if (instrState != eInstrType_ARM && instrWidth == eInstrWidth_NotSpecified)
    instrWidth = !isAcc && extend != eIsByte16 && ror == 0 && rd < 8 && rm < 8 ? eInstrWidth_Enforce16bit : eInstrWidth_Enforce32bit;
  if (instrWidth == eInstrWidth_Enforce16bit && ror != 0)
    {
      Error (ErrorError, "16 bit Thumb can only encode ROR 0 value");
      ror = 0;
    }

  if (instrState == eInstrType_ARM)
    {
      if (rd == 15 || rm == 15)
	Error (ErrorWarning, "Use of PC for Rd or Rm is unpredictable");
      ARMWord baseInstr;
      switch (extend)
	{
	  case eIsByte:
	    baseInstr = 0x06A00070;
	    break;
	  case eIsByte16:
	    baseInstr = 0x06800070;
	    break;
	  case eIsHalfword:
	    baseInstr = 0x06b00070;
	    break;
	}
      Put_Ins (4, baseInstr | cc | (isLXT ? (1<<22) : 0) | (rn << 16) | (rd << 12) | (ror << 10) | rm);
    }
  else if (instrWidth == eInstrWidth_Enforce16bit)
    {
      assert (extend == eIsByte || extend == eIsHalfword);
      Put_Ins (2, 0xB200 | (rm << 3) | rd | (extend == eIsByte ? 0x40 : 0x00) | (isLXT ? 0x80 : 0x00));
    }
  else
    {
      Target_CheckCPUFeature (kCPUExt_v6T2, true);
      if (rd == 13 || rd == 15 || rn == 13 || rm == 15 || rm == 15)
	Error (ErrorWarning, "Use of R13 or PC for Rd, Rn or Rm is unpredictable");
      ARMWord baseInstr;
      switch (extend)
	{
	  case eIsByte:
	    baseInstr = 0xFA40F080;
	    break;
	  case eIsByte16:
	    baseInstr = 0xFA20F080;
	    break;
	  case eIsHalfword:
	    baseInstr = 0xFA00F080;
	    break;
	}
      Put_Ins (4, baseInstr | (isLXT ? (1<<20) : 0) | (rn << 16) | (rd << 8) | (ror << 4) | rm);
    }

  return false;
}

/**
 * Implements 
 *   SXT{"A"}{"B"|"B16"|"H"}{<c>}{<q>} {<Rd>,} <Rn> {, <Rm>} {, <rotation>}
 */
bool
m_sxt (bool doLowerCase)
{
  return core_sxt_uxt (doLowerCase, false);
}


/**
 * Implements 
 *   UXT{"A"}{"B"|"B16"|"H"}{<c>}{<q>} {<Rd>,} <Rn> {, <Rm>} {, <rotation>}
 */
bool
m_uxt (bool doLowerCase)
{
  return core_sxt_uxt (doLowerCase, true);
}
