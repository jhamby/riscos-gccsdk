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
 * m_cpu.c
 */

#include "config.h"
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
#include "main.h"
#include "m_cpu.h"
#include "option.h"
#include "put.h"
#include "targetcpu.h"

/**
 * Implements CODE16.
 * What follows is in pre-UAL notation.
 */
bool
c_code16 (void)
{
  error (ErrorError, "Thumb code is not yet supported"); /* FIXME */
  return false;
}

/**
 * Implements THUMB.
 * What follows in in UAL notation.
 */
bool
c_thumb (void)
{
  error (ErrorError, "Thumb code is not yet supported"); /* FIXME */
  return false;
}

/**
 * Implements THUMBX.
 * What follows is in UAL notation.
 */
bool
c_thumbx (void)
{
  error (ErrorError, "Thumb-2EE code is not yet supported"); /* FIXME */
  return false;
}

/**
 * Implements ARM / CODE32.
 * What follows can be in UAL or pre-UAL notation.
 */
bool
c_code32 (void)
{
  /* Ignore */
  return false;
}

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
m_nop (void)
{
  switch (Target_GetArch ())
    {
      case ARCH_ARMv6K:
      case ARCH_ARMv6T2:
      case ARCH_ARMv7:
	{
	  ARMWord cc = optionCond ();
	  if (cc == optionError)
	    return true;
	  Put_Ins (0x0320F000 | cc);
	  break;
	}
      default:
	if (!Input_IsEndOfKeyword ())
	  return true;
	Put_Ins (0xE1A00000); /* MOV R0, R0 */
	break;
    }
  return false;
}

/** DATA dst = lhs <op> rhs **/

static void
dstlhsrhs (ARMWord ir)
{
  ARMWord op = getCpuReg ();
  ir |= DST_OP (op);
  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%sdst", InsertCommaAfter);
  op = getCpuReg ();
  ir |= LHS_OP (op);
  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%slhs", InsertCommaAfter);
  ir = getRhs (false, true, ir);
  Put_Ins (ir);
}

static void
dstrhs (ARMWord ir)
{
  ARMWord op = getCpuReg ();
  ir |= DST_OP (op);
  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%sdst", InsertCommaAfter);
  ir = getRhs (false, true, ir);
  Put_Ins (ir);
}


/**
 * Implements ADC.
 */
bool
m_adc (void)
{
  ARMWord cc = optionCondS ();
  if (cc == optionError)
    return true;
  dstlhsrhs (cc | M_ADC);
  return false;
}

/**
 * Implements ADD.
 */
bool
m_add (void)
{
  ARMWord cc = optionCondS ();
  if (cc == optionError)
    return true;
  dstlhsrhs (cc | M_ADD);
  return false;
}

/**
 * Implements AND.
 */
bool
m_and (void)
{
  ARMWord cc = optionCondS ();
  if (cc == optionError)
    return true;
  dstlhsrhs (cc | M_AND);
  return false;
}

/**
 * Implements BIC.
 */
bool
m_bic (void)
{
  ARMWord cc = optionCondS ();
  if (cc == optionError)
    return true;
  dstlhsrhs (cc | M_BIC);
  return false;
}

/**
 * Implements EOR.
 */
bool
m_eor (void)
{
  ARMWord cc = optionCondS ();
  if (cc == optionError)
    return true;
  dstlhsrhs (cc | M_EOR);
  return false;
}

/**
 * Implements MOV.
 */
bool
m_mov (void)
{
  ARMWord cc = optionCondS ();
  if (cc == optionError)
    return true;
  dstrhs (cc | M_MOV);
  return false;
}

/**
 * Implements MVN.
 */
bool
m_mvn (void)
{
  ARMWord cc = optionCondS ();
  if (cc == optionError)
    return true;
  dstrhs (cc | M_MVN);
  return false;
}

/**
 * Implements ORR.
 */
bool
m_orr (void)
{
  ARMWord cc = optionCondS ();
  if (cc == optionError)
    return true;
  dstlhsrhs (cc | M_ORR);
  return false;
}

/**
 * Implements RSB.
 */
bool
m_rsb (void)
{
  ARMWord cc = optionCondS ();
  if (cc == optionError)
    return true;
  dstlhsrhs (cc | M_RSB);
  return false;
}

/**
 * Implements RSC.
 */
bool
m_rsc (void)
{
  ARMWord cc = optionCondS ();
  if (cc == optionError)
    return true;
  dstlhsrhs (cc | M_RSC);
  return false;
}

/**
 * Implements SBC.
 */
bool
m_sbc (void)
{
  ARMWord cc = optionCondS ();
  if (cc == optionError)
    return true;
  dstlhsrhs (cc | M_SBC);
  return false;
}

/**
 * Implements SUB.
 */
bool
m_sub (void)
{
  ARMWord cc = optionCondS ();
  if (cc == optionError)
    return true;
  dstlhsrhs (cc | M_SUB);
  return false;
}

/** DATA test **/

static void
lhsrhs (ARMWord ir)
{
  ARMWord op = getCpuReg ();
  ir |= LHS_OP (op);
  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%slhs", InsertCommaAfter);
  ir = getRhs (false, true, ir);
  Put_Ins (ir);
}

/**
 * Implements CMN.
 */
bool
m_cmn (void)
{
  ARMWord cc = optionCondSP ();
  if (cc == optionError)
    return true;
  lhsrhs (cc | M_CMN);
  return false;
}

/**
 * Implements CMP.
 */
bool
m_cmp (void)
{
  ARMWord cc = optionCondSP ();
  if (cc == optionError)
    return true;
  lhsrhs (cc | M_CMP);
  return false;
}

/**
 * Implements TEQ.
 */
bool
m_teq (void)
{
  ARMWord cc = optionCondSP ();
  if (cc == optionError)
    return true;
  lhsrhs (cc | M_TEQ);
  return false;
}

/**
 * Implements TST.
 */
bool
m_tst (void)
{
  ARMWord cc = optionCondSP ();
  if (cc == optionError)
    return true;
  lhsrhs (cc | M_TST);
  return false;
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
  ARMWord regD = getCpuReg ();
  if (regD == 15)
    error (ErrorWarning, "Use of register PC is unpredictable");

  /* Read Rn (lhs).  */
  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%sdst", InsertCommaAfter);
  ARMWord regN = getCpuReg ();
  if (regN == 15)
    error (ErrorWarning, "Use of register PC is unpredictable");

  /* Read Rm (rhs).  */
  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%slhs", InsertCommaAfter);
  ARMWord regM = getCpuReg ();
  if (regM == 15)
    error (ErrorWarning, "Use of register PC is unpredictable");
  if (mulType != Is_eMLS)
    {
      if (regD == regN)
	{
	  if (regN == regM)
	    error (ErrorError, "Destination and left operand are the same register %d", regD);
	  else
	    {
	      error (ErrorInfo, "Changing order of operands in %s to avoid unpredicability", mulType == Is_eMLA ? "MLA" : "MUL");
	      int t = regN;
	      regN = regM;
	      regM = t;
	    }
	}
    }

  if (mulType != Is_eMUL)
    {
      /* Read Ra.  */
      skipblanks ();
      if (!Input_Match (',', true))
	error (ErrorError, "%srhs", InsertCommaAfter);
      ARMWord regA = getCpuReg ();
      if (regA == 15)
	error (ErrorWarning, "Use of register PC is unpredictable");
      ir |= ACC_MUL (regA);
    }
  ir |= DST_MUL (regD);
  ir |= LHS_MUL (regN);
  ir |= RHS_MUL (regM);

  Put_Ins (ir);
}

/**
 * Implements MLA.
 *   MLA{S}<cond> <Rd>, <Rn>, <Rm>, <Ra>
 */
bool
m_mla (void)
{
  ARMWord cc = optionCondS ();
  if (cc == optionError)
    return true;

  Target_NeedAtLeastArch (ARCH_ARMv2);
  onlyregs (Is_eMLA, cc | M_MLA);
  return false;
}

/**
 * Implements MLS.
 *   MLS<cond> <Rd>, <Rn>, <Rm>, <Ra>
 */
bool
m_mls (void)
{
  ARMWord cc = optionCond (); /* Note, no 'S' */
  if (cc == optionError)
    return true;

  Target_NeedAtLeastArch (ARCH_ARMv6T2);
  onlyregs (Is_eMLS, cc | M_MLS);
  return false;
}

/**
 * Implements MUL.
 *   MUL{S}<cond> <Rd>, <Rn>, <Rm>, <Ra>
 */
bool
m_mul (void)
{
  ARMWord cc = optionCondS ();
  if (cc == optionError)
    return true;

  Target_NeedAtLeastArch (ARCH_ARMv2);
  onlyregs (Is_eMUL, cc | M_MUL);
  return false;
}

static void
l_onlyregs (ARMWord ir, const char *op)
{
  /* This bit only set for smulxx */
  bool issmull = !(ir & 0x01000000);
  bool issmlaxy = (ir & 0x00600000) == 0;
  bool issmlalxy = (ir & 0x00600000) == 0x00400000;
  bool issmlawy = (ir & 0x00600020) == 0x00200000;

  ARMWord dstl;
  if (issmull)
    {
      Target_NeedAtLeastArch (ARCH_ARMv4);
      dstl = getCpuReg ();
      if (!Input_Match (',', true))
        error (ErrorError, "%sdst_h", InsertCommaAfter);
    }
  else
    {
      Target_NeedAtLeastArch (ARCH_ARMv5TE);
      if (issmlalxy)
        {
          dstl = getCpuReg ();
	  if (!Input_Match (',', true))
            error (ErrorError, "%sdst_l", InsertCommaAfter);
        }
      else
        dstl = 0;
    }

  ARMWord dsth = getCpuReg ();
  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%sdst_l", InsertCommaAfter);
  ARMWord lhs = getCpuReg ();
  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%slhs", InsertCommaAfter);
  ARMWord rhs = getCpuReg ();
  if (issmull)
    {
      if (dstl == dsth)
        error (ErrorError, "Destination high and low are the same register %d", dstl);
      if (dstl == lhs || dsth == lhs)
        {
          if (dstl == rhs || dsth == rhs)
            error (ErrorError, "Left operand register %d also occurs in destination", lhs);
          else
	    {
	      if (option_fussy)
	        error (ErrorInfo, "Changing order of operands in %s", op);
	      int t = lhs;
	      lhs = rhs;
	      rhs = t;
	    }
        }
     }
  else
     {
       if (dsth == 15 || lhs == 15 || rhs == 15)
         error (ErrorError, "Cannot use R15 with %s", op);
     }
  if (!issmull && (issmlaxy || issmlawy))
    {
      if (!Input_Match (',', true))
        error (ErrorError, "%sdst_l", InsertCommaAfter);
      dstl = getCpuReg ();
      if (dstl == 15)
        error (ErrorError, "Cannot use R15 with %s", op);
    }

  ir |= dstl << 12 | dsth << 16 | lhs | rhs << 8;
  Put_Ins (ir);
}

/**
 * Implements SMULL.
 */
bool
m_smull (void)
{
  ARMWord cc = optionCondS ();
  if (cc == optionError)
    return true;
  Target_NeedAtLeastArch (ARCH_ARMv3M);
  l_onlyregs (cc | M_SMULL, "SMULL");
  return false;
}

/**
 * Implements SMULBB.
 */
bool
m_smulbb (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;
  Target_NeedAtLeastArch (ARCH_ARMv5TE);
  l_onlyregs (cc | M_SMULBB, "SMULBB");
  return false;
}

/**
 * Implements SMULBT.
 */
bool
m_smulbt (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;
  Target_NeedAtLeastArch (ARCH_ARMv5TE);
  l_onlyregs (cc | M_SMULBT, "SMULBT");
  return false;
}

/**
 * Implements SMULTB.
 */
bool
m_smultb (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;
  Target_NeedAtLeastArch (ARCH_ARMv5TE);
  l_onlyregs (cc | M_SMULTB, "SMULTB");
  return false;
}

/**
 * Implements SMULTT.
 */
bool
m_smultt (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;
  Target_NeedAtLeastArch (ARCH_ARMv5TE);
  l_onlyregs (cc | M_SMULTT, "SMULTT");
  return false;
}

/**
 * Implements SMULWB.
 */
bool
m_smulwb (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;
  Target_NeedAtLeastArch (ARCH_ARMv5TE);
  l_onlyregs (cc | M_SMULWB, "SMULWB");
  return false;
}

/**
 * Implements SMULWT.
 */
bool
m_smulwt (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;
  Target_NeedAtLeastArch (ARCH_ARMv5TE);
  l_onlyregs (cc | M_SMULWT, "SMULWT");
  return false;
}

/**
 * Implements SMLAL.
 */
bool
m_smlal (void)
{
  ARMWord cc = optionCondS ();
  if (cc == optionError)
    return true;
  Target_NeedAtLeastArch (ARCH_ARMv3M);
  l_onlyregs (cc | M_SMLAL, "SMLAL");
  return false;
}

/**
 * Implements SMLALBB.
 */
bool
m_smlalbb (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;
  Target_NeedAtLeastArch (ARCH_ARMv5TE);
  l_onlyregs (cc | M_SMLALBB, "SMLALBB");
  return false;
}

/**
 * Implements SMLALBT.
 */
bool
m_smlalbt (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;
  Target_NeedAtLeastArch (ARCH_ARMv5TE);
  l_onlyregs (cc | M_SMLALBT, "SMLALBT");
  return false;
}

/**
 * Implements SMLALTB.
 */
bool
m_smlaltb (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;
  Target_NeedAtLeastArch (ARCH_ARMv5TE);
  l_onlyregs (cc | M_SMLALTB, "SMLALTB");
  return false;
}

/**
 * Implements SMLALTT.
 */
bool
m_smlaltt (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;
  Target_NeedAtLeastArch (ARCH_ARMv5TE);
  l_onlyregs (cc | M_SMLALTT, "SMLALTT");
  return false;
}

/**
 * Implements SMLABB.
 */
bool
m_smlabb (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;
  Target_NeedAtLeastArch (ARCH_ARMv5TE);
  l_onlyregs (cc | M_SMLABB, "SMLABB");
  return false;
}

/**
 * Implements SMLABT.
 */
bool
m_smlabt (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;
  Target_NeedAtLeastArch (ARCH_ARMv5TE);
  l_onlyregs (cc | M_SMLABT, "SMLABT");
  return false;
}

/**
 * Implements SMLATB.
 */
bool
m_smlatb (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;
  Target_NeedAtLeastArch (ARCH_ARMv5TE);
  l_onlyregs (cc | M_SMLATB, "SMLATB");
  return false;
}

/**
 * Implements SMLATT.
 */
bool
m_smlatt (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;
  Target_NeedAtLeastArch (ARCH_ARMv5TE);
  l_onlyregs (cc | M_SMLATT, "SMLATT");
  return false;
}

/**
 * Implements SMLAWB.
 */
bool
m_smlawb (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;
  Target_NeedAtLeastArch (ARCH_ARMv5TE);
  l_onlyregs (cc | M_SMLAWB, "SMLAWB");
  return false;
}

/**
 * Implements SMLAWT.
 */
bool
m_smlawt (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;
  Target_NeedAtLeastArch (ARCH_ARMv5TE);
  l_onlyregs (cc | M_SMLAWT, "SMLAWT");
  return false;
}

/**
 * Implements UMULL.
 */
bool
m_umull (void)
{
  ARMWord cc = optionCondS ();
  if (cc == optionError)
    return true;
  Target_NeedAtLeastArch (ARCH_ARMv3M);
  l_onlyregs (cc | M_UMULL, "UMULL");
  return false;
}

/**
 * Implements UMLAL.
 */
bool
m_umlal (void)
{
  ARMWord cc = optionCondS ();
  if (cc == optionError)
    return true;
  Target_NeedAtLeastArch (ARCH_ARMv3M);
  l_onlyregs (cc | M_UMLAL, "UMLAL");
  return false;
}


/**
 * Implements CLZ.
 */
bool
m_clz (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;

  Target_NeedAtLeastArch (ARCH_ARMv5);

  ARMWord ir = cc | M_CLZ;

  ARMWord dst = getCpuReg ();
  ir |= DST_OP (dst);
  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%slhs", InsertCommaAfter);

  ARMWord rhs = getCpuReg ();
  ir |= RHS_OP (rhs);

  if (dst == 15 || rhs == 15)
    error (ErrorError, "Use of R15 in CLZ is unpredictable");

  Put_Ins (ir);
  return false;
}

static void
q_onlyregs (ARMWord ir, const char *op)
{
  Target_NeedAtLeastArch (ARCH_ARMv5TE);

  skipblanks ();

  ARMWord dst = getCpuReg ();
  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%sdst", InsertCommaAfter);
  ARMWord lhs = getCpuReg ();
  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%slhs", InsertCommaAfter);
  ARMWord rhs = getCpuReg ();
  if (dst == 15 || lhs == 15 || rhs == 15)
    error (ErrorError, "Cannot use R15 with %s", op);

  ir |= dst << 12 | lhs | rhs << 16;
  Put_Ins (ir);
}

/**
 * Implements QADD.
 */
bool
m_qadd (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;
  q_onlyregs (cc | M_QADD, "QADD");
  return false;
}

/**
 * Implements QDADD.
 */
bool
m_qdadd (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;
  q_onlyregs (cc | M_QDADD, "QDADD");
  return false;
}

/**
 * Implements QDSUB.
 */
bool
m_qdsub (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;
  q_onlyregs (cc | M_QDSUB, "QDSUB");
  return false;
}

/**
 * Implements QSUB.
 */
bool
m_qsub (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;
  q_onlyregs (cc | M_QSUB, "QSUB");
  return false;
}

static bool
UALShift (ARMWord shiftType)
{
  ARMWord cc = Option_SCond ();
  if (cc == optionError)
    return true;

  ARMWord regD = getCpuReg ();
  if (regD == INVALID_REG)
    return false;
  if (!Input_Match (',', true))
    {
      error (ErrorError, "Missing ,");
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
	  error (ErrorError, "Missing immediate constant");
	  return false;
	}
      const Value *im = exprBuildAndEval (ValueInt);
      if (im->Tag != ValueInt)
	{
	  error (ErrorError, "Failed to evaluate immediate constant");
	  return false;
	}
      Put_Ins (cc | Fix_ShiftImm (NULL, 0, shiftType, im->Data.Int.i));
    }
  else
    Put_Ins (cc | SHIFT_REG (regS) | SHIFT_OP (shiftType));
  
  return false;
}

/**
 * Implements ASR.
 *   ASR<S><CC> {Rd,} Rm, #<num> : UAL version of MOV<CC><S> Rd, Rm, ASR#<num>
 *   ASR<S><CC> {Rd,} Rm, Rs : UAL version of MOV<CC><S> Rd, Rm, ASR Rs
 */
bool
m_asr (void)
{
  return UALShift (ASR);
}

/**
 * Implements LSL.
 *   LSL<S><CC> {Rd,} Rm, #<num> : UAL version of MOV<CC><S> Rd, Rm, LSL#<num>
 *   LSL<S><CC> {Rd,} Rm, Rn : UAL version of MOV<CC><S> Rd, Rm, LSL Rs
 */
bool
m_lsl (void)
{
  return UALShift (LSL);
}

/**
 * Implements LSR.
 *   LSR<S><CC> {Rd,} Rm, #<num> : UAL version of MOV<CC><S> Rd, Rm, LSR#<num>
 *   LSR<S><CC> {Rd,} Rm, Rs : UAL version of MOV<CC><S> Rd, Rm, LSR Rs
 */
bool
m_lsr (void)
{
  return UALShift (LSR);
}

/**
 * Implements ROR.
 *   ROR<S><CC> {Rd,} Rm, #<num> : UAL version of MOV<CC><S> Rd, Rm, ROR#<num>
 *   ROR<S><CC> {Rd,} Rm, Rs : UAL version of MOV<CC><S> Rd, Rm, ROR Rs
 */
bool
m_ror (void)
{
  return UALShift (ROR);
}

/**
 * Implements RRX.
 *   RRX<S><CC> {Rd,} Rm : UAL version of MOV<CC><S> Rd, Rm, RRX
 */
bool
m_rrx (void)
{
  ARMWord cc = Option_SCond ();
  if (cc == optionError)
    return true;

  ARMWord regD = getCpuReg ();
  if (regD == INVALID_REG)
    return false;
  skipblanks ();
  ARMWord regM;
  if (Input_Match (',', true))
    {
      regM = getCpuReg ();
      if (regM == INVALID_REG)
	return false;
    }
  else
    regM = regD;

  Put_Ins (cc | M_MOV | DST_OP (regD) | regM | SHIFT_OP (RRX));
  
  return false;
}
