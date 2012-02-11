/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2012 GCCSDK Developers
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
Rslt_e
m_nop (bool doLowerCase)
{
  switch (Target_GetArch ())
    {
      case ARCH_ARMv6K:
      case ARCH_ARMv6T2:
      case ARCH_ARMv7:
	{
	  ARMWord cc = optionCond (doLowerCase);
	  if (cc == optionError)
	    return eRslt_NotRecognized;
	  Put_Ins (0x0320F000 | cc);
	  break;
	}

      default:
	if (!Input_IsEndOfKeyword ())
	  return eRslt_NotRecognized;
	Put_Ins (0xE1A00000); /* MOV R0, R0 */
	break;
    }
  return eRslt_ARM;
}

/** DATA dst = lhs <op> rhs **/

static Rslt_e
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
  ir = getRhs (true, true, ir);
  Put_Ins (ir);
  return eRslt_ARM;
}

static Rslt_e
dstrhs (ARMWord ir)
{
  ARMWord op = getCpuReg ();
  ir |= DST_OP (op);
  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%sdst", InsertCommaAfter);
  ir = getRhs (true, true, ir);
  Put_Ins (ir);
  return eRslt_ARM;
}


/**
 * Implements ADC.
 */
Rslt_e
m_adc (bool doLowerCase)
{
  ARMWord cc = optionCondS (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  return dstlhsrhs (cc | M_ADC);
}

/**
 * Implements ADD.
 */
Rslt_e
m_add (bool doLowerCase)
{
  ARMWord cc = optionCondS (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  return dstlhsrhs (cc | M_ADD);
}

/**
 * Implements AND.
 */
Rslt_e
m_and (bool doLowerCase)
{
  ARMWord cc = optionCondS (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  return dstlhsrhs (cc | M_AND);
}

/**
 * Implements BIC.
 */
Rslt_e
m_bic (bool doLowerCase)
{
  ARMWord cc = optionCondS (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  return dstlhsrhs (cc | M_BIC);
}

/**
 * Implements EOR.
 */
Rslt_e
m_eor (bool doLowerCase)
{
  ARMWord cc = optionCondS (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  return dstlhsrhs (cc | M_EOR);
}

/**
 * Implements MOV.
 */
Rslt_e
m_mov (bool doLowerCase)
{
  ARMWord cc = optionCondS (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  return dstrhs (cc | M_MOV);
}

/**
 * Implements MVN.
 */
Rslt_e
m_mvn (bool doLowerCase)
{
  ARMWord cc = optionCondS (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  return dstrhs (cc | M_MVN);
}

/**
 * Implements ORR.
 */
Rslt_e
m_orr (bool doLowerCase)
{
  ARMWord cc = optionCondS (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  return dstlhsrhs (cc | M_ORR);
}

/**
 * Implements RSB.
 */
Rslt_e
m_rsb (bool doLowerCase)
{
  ARMWord cc = optionCondS (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  return dstlhsrhs (cc | M_RSB);
}

/**
 * Implements RSC.
 */
Rslt_e
m_rsc (bool doLowerCase)
{
  ARMWord cc = optionCondS (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  return dstlhsrhs (cc | M_RSC);
}

/**
 * Implements SBC.
 */
Rslt_e
m_sbc (bool doLowerCase)
{
  ARMWord cc = optionCondS (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  return dstlhsrhs (cc | M_SBC);
}

/**
 * Implements SUB.
 */
Rslt_e
m_sub (bool doLowerCase)
{
  ARMWord cc = optionCondS (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  return dstlhsrhs (cc | M_SUB);
}

/** DATA test **/

static Rslt_e
lhsrhs (ARMWord ir)
{
  ARMWord op = getCpuReg ();
  ir |= LHS_OP (op);
  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%slhs", InsertCommaAfter);
  ir = getRhs (true, true, ir);
  Put_Ins (ir);
  return eRslt_ARM;
}

/**
 * Implements CMN.
 */
Rslt_e
m_cmn (bool doLowerCase)
{
  ARMWord cc = Option_CondSP (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  return lhsrhs (cc | M_CMN);
}

/**
 * Implements CMP.
 */
Rslt_e
m_cmp (bool doLowerCase)
{
  ARMWord cc = Option_CondSP (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  return lhsrhs (cc | M_CMP);
}

/**
 * Implements TEQ.
 */
Rslt_e
m_teq (bool doLowerCase)
{
  ARMWord cc = Option_CondSP (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  return lhsrhs (cc | M_TEQ);
}

/**
 * Implements TST.
 */
Rslt_e
m_tst (bool doLowerCase)
{
  ARMWord cc = Option_CondSP (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
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
Rslt_e
m_mla (bool doLowerCase)
{
  ARMWord cc = optionCondS (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;

  Target_NeedAtLeastArch (ARCH_ARMv2);
  onlyregs (Is_eMLA, cc | M_MLA);
  return eRslt_ARM;
}

/**
 * Implements MLS.
 *   MLS<cond> <Rd>, <Rn>, <Rm>, <Ra>
 */
Rslt_e
m_mls (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase); /* Note, no 'S' */
  if (cc == optionError)
    return eRslt_NotRecognized;

  Target_NeedAtLeastArch (ARCH_ARMv6T2);
  onlyregs (Is_eMLS, cc | M_MLS);
  return eRslt_ARM;
}

/**
 * Implements MUL.
 *   MUL{S}<cond> <Rd>, <Rn>, <Rm>, <Ra>
 */
Rslt_e
m_mul (bool doLowerCase)
{
  ARMWord cc = optionCondS (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;

  Target_NeedAtLeastArch (ARCH_ARMv2);
  onlyregs (Is_eMUL, cc | M_MUL);
  return eRslt_ARM;
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
Rslt_e
m_smull (bool doLowerCase)
{
  ARMWord cc = optionCondS (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  Target_NeedAtLeastArch (ARCH_ARMv3M);
  l_onlyregs (cc | M_SMULL, "SMULL");
  return eRslt_ARM;
}

/**
 * Implements SMULBB.
 */
Rslt_e
m_smulbb (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  Target_NeedAtLeastArch (ARCH_ARMv5TE);
  l_onlyregs (cc | M_SMULBB, "SMULBB");
  return eRslt_ARM;
}

/**
 * Implements SMULBT.
 */
Rslt_e
m_smulbt (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  Target_NeedAtLeastArch (ARCH_ARMv5TE);
  l_onlyregs (cc | M_SMULBT, "SMULBT");
  return eRslt_ARM;
}

/**
 * Implements SMULTB.
 */
Rslt_e
m_smultb (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  Target_NeedAtLeastArch (ARCH_ARMv5TE);
  l_onlyregs (cc | M_SMULTB, "SMULTB");
  return eRslt_ARM;
}

/**
 * Implements SMULTT.
 */
Rslt_e
m_smultt (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  Target_NeedAtLeastArch (ARCH_ARMv5TE);
  l_onlyregs (cc | M_SMULTT, "SMULTT");
  return eRslt_ARM;
}

/**
 * Implements SMULWB.
 */
Rslt_e
m_smulwb (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  Target_NeedAtLeastArch (ARCH_ARMv5TE);
  l_onlyregs (cc | M_SMULWB, "SMULWB");
  return eRslt_ARM;
}

/**
 * Implements SMULWT.
 */
Rslt_e
m_smulwt (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  Target_NeedAtLeastArch (ARCH_ARMv5TE);
  l_onlyregs (cc | M_SMULWT, "SMULWT");
  return eRslt_ARM;
}

/**
 * Implements SMLAL.
 */
Rslt_e
m_smlal (bool doLowerCase)
{
  ARMWord cc = optionCondS (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  Target_NeedAtLeastArch (ARCH_ARMv3M);
  l_onlyregs (cc | M_SMLAL, "SMLAL");
  return eRslt_ARM;
}

/**
 * Implements SMLALBB.
 */
Rslt_e
m_smlalbb (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  Target_NeedAtLeastArch (ARCH_ARMv5TE);
  l_onlyregs (cc | M_SMLALBB, "SMLALBB");
  return eRslt_ARM;
}

/**
 * Implements SMLALBT.
 */
Rslt_e
m_smlalbt (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  Target_NeedAtLeastArch (ARCH_ARMv5TE);
  l_onlyregs (cc | M_SMLALBT, "SMLALBT");
  return eRslt_ARM;
}

/**
 * Implements SMLALTB.
 */
Rslt_e
m_smlaltb (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  Target_NeedAtLeastArch (ARCH_ARMv5TE);
  l_onlyregs (cc | M_SMLALTB, "SMLALTB");
  return eRslt_ARM;
}

/**
 * Implements SMLALTT.
 */
Rslt_e
m_smlaltt (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  Target_NeedAtLeastArch (ARCH_ARMv5TE);
  l_onlyregs (cc | M_SMLALTT, "SMLALTT");
  return eRslt_ARM;
}

/**
 * Implements SMLABB.
 */
Rslt_e
m_smlabb (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  Target_NeedAtLeastArch (ARCH_ARMv5TE);
  l_onlyregs (cc | M_SMLABB, "SMLABB");
  return eRslt_ARM;
}

/**
 * Implements SMLABT.
 */
Rslt_e
m_smlabt (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  Target_NeedAtLeastArch (ARCH_ARMv5TE);
  l_onlyregs (cc | M_SMLABT, "SMLABT");
  return eRslt_ARM;
}

/**
 * Implements SMLATB.
 */
Rslt_e
m_smlatb (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  Target_NeedAtLeastArch (ARCH_ARMv5TE);
  l_onlyregs (cc | M_SMLATB, "SMLATB");
  return eRslt_ARM;
}

/**
 * Implements SMLATT.
 */
Rslt_e
m_smlatt (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  Target_NeedAtLeastArch (ARCH_ARMv5TE);
  l_onlyregs (cc | M_SMLATT, "SMLATT");
  return eRslt_ARM;
}

/**
 * Implements SMLAWB.
 */
Rslt_e
m_smlawb (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  Target_NeedAtLeastArch (ARCH_ARMv5TE);
  l_onlyregs (cc | M_SMLAWB, "SMLAWB");
  return eRslt_ARM;
}

/**
 * Implements SMLAWT.
 */
Rslt_e
m_smlawt (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  Target_NeedAtLeastArch (ARCH_ARMv5TE);
  l_onlyregs (cc | M_SMLAWT, "SMLAWT");
  return eRslt_ARM;
}

/**
 * Implements UMULL.
 */
Rslt_e
m_umull (bool doLowerCase)
{
  ARMWord cc = optionCondS (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  Target_NeedAtLeastArch (ARCH_ARMv3M);
  l_onlyregs (cc | M_UMULL, "UMULL");
  return eRslt_ARM;
}

/**
 * Implements UMLAL.
 */
Rslt_e
m_umlal (bool doLowerCase)
{
  ARMWord cc = optionCondS (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  Target_NeedAtLeastArch (ARCH_ARMv3M);
  l_onlyregs (cc | M_UMLAL, "UMLAL");
  return eRslt_ARM;
}


/**
 * Implements CLZ.
 */
Rslt_e
m_clz (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;

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
  return eRslt_ARM;
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
Rslt_e
m_qadd (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  q_onlyregs (cc | M_QADD, "QADD");
  return eRslt_ARM;
}

/**
 * Implements QDADD.
 */
Rslt_e
m_qdadd (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  q_onlyregs (cc | M_QDADD, "QDADD");
  return eRslt_ARM;
}

/**
 * Implements QDSUB.
 */
Rslt_e
m_qdsub (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  q_onlyregs (cc | M_QDSUB, "QDSUB");
  return eRslt_ARM;
}

/**
 * Implements QSUB.
 */
Rslt_e
m_qsub (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;
  q_onlyregs (cc | M_QSUB, "QSUB");
  return eRslt_ARM;
}

static Rslt_e
UALShift (ARMWord shiftType, bool doLowerCase)
{
  ARMWord cc = Option_SCond (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;

  ARMWord regD = getCpuReg ();
  if (regD == INVALID_REG)
    return eRslt_ARM;
  if (!Input_Match (',', true))
    {
      error (ErrorError, "Missing ,");
      return eRslt_ARM;
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
	  return eRslt_ARM;
	}
      const Value *im = exprBuildAndEval (ValueInt);
      if (im->Tag != ValueInt)
	{
	  error (ErrorError, "Failed to evaluate immediate constant");
	  return eRslt_ARM;
	}
      Put_Ins (cc | Fix_ShiftImm (NULL, 0, shiftType, im->Data.Int.i));
    }
  else
    Put_Ins (cc | SHIFT_REG (regS) | SHIFT_OP (shiftType));
  
  return eRslt_ARM;
}

/**
 * Implements ASR.
 *   ASR<S><CC> {Rd,} Rm, #<num> : UAL version of MOV<CC><S> Rd, Rm, ASR#<num>
 *   ASR<S><CC> {Rd,} Rm, Rs : UAL version of MOV<CC><S> Rd, Rm, ASR Rs
 */
Rslt_e
m_asr (bool doLowerCase)
{
  return UALShift (ASR, doLowerCase);
}

/**
 * Implements LSL.
 *   LSL<S><CC> {Rd,} Rm, #<num> : UAL version of MOV<CC><S> Rd, Rm, LSL#<num>
 *   LSL<S><CC> {Rd,} Rm, Rn : UAL version of MOV<CC><S> Rd, Rm, LSL Rs
 */
Rslt_e
m_lsl (bool doLowerCase)
{
  return UALShift (LSL, doLowerCase);
}

/**
 * Implements LSR.
 *   LSR<S><CC> {Rd,} Rm, #<num> : UAL version of MOV<CC><S> Rd, Rm, LSR#<num>
 *   LSR<S><CC> {Rd,} Rm, Rs : UAL version of MOV<CC><S> Rd, Rm, LSR Rs
 */
Rslt_e
m_lsr (bool doLowerCase)
{
  return UALShift (LSR, doLowerCase);
}

/**
 * Implements ROR.
 *   ROR<S><CC> {Rd,} Rm, #<num> : UAL version of MOV<CC><S> Rd, Rm, ROR#<num>
 *   ROR<S><CC> {Rd,} Rm, Rs : UAL version of MOV<CC><S> Rd, Rm, ROR Rs
 */
Rslt_e
m_ror (bool doLowerCase)
{
  return UALShift (ROR, doLowerCase);
}

/**
 * Implements RRX.
 *   RRX<S><CC> {Rd,} Rm : UAL version of MOV<CC><S> Rd, Rm, RRX
 */
Rslt_e
m_rrx (bool doLowerCase)
{
  ARMWord cc = Option_SCond (doLowerCase);
  if (cc == optionError)
    return eRslt_NotRecognized;

  ARMWord regD = getCpuReg ();
  if (regD == INVALID_REG)
    return eRslt_ARM;
  skipblanks ();
  ARMWord regM;
  if (Input_Match (',', true))
    {
      regM = getCpuReg ();
      if (regM == INVALID_REG)
	return eRslt_ARM;
    }
  else
    regM = regD;

  Put_Ins (cc | M_MOV | DST_OP (regD) | regM | SHIFT_OP (RRX));
  
  return eRslt_ARM;
}
