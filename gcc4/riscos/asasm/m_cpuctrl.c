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
 * m_cpuctrl.c
 */

#include "config.h"

#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif

#include "area.h"
#include "code.h"
#include "error.h"
#include "expr.h"
#include "fix.h"
#include "get.h"
#include "global.h"
#include "help_cpu.h"
#include "input.h"
#include "main.h"
#include "m_cpu.h"
#include "m_cpuctrl.h"
#include "option.h"
#include "os.h"
#include "phase.h"
#include "put.h"
#include "state.h"
#include "targetcpu.h"
#include "value.h"

/**
 * Shared for B, BL and BLX implementation.
 */
static bool
branch_shared (ARMWord cc, bool isBLX)
{
  /* At this point the current area index can be unaligned for ARM/Thumb
     instructions, upfront correct this index.  */
  const uint32_t instrAlign = State_GetInstrType () == eInstrType_ARM ? 4 : 2;
  const uint32_t offset = (areaCurrentSymbol->area.info->curIdx + instrAlign-1) & -instrAlign;
  const uint32_t areaOffset = (areaCurrentSymbol->area.info->type & AREA_ABS) ? Area_GetBaseAddress (areaCurrentSymbol) : 0;

  exprBuild ();

  if (gPhase == ePassOne)
    {
      Put_Ins (4, 0);
      return false;
    }

  const Value *valP = exprEval (ValueInt | ValueSymbol); /* FIXME: support ValueAddr  ? */
  if (valP->Tag == ValueIllegal
      || (valP->Tag == ValueSymbol && valP->Data.Symbol.factor != 1))
    {
      error (ErrorError, "Illegal branch expression");
      return false;
    }

  /* Switch to ValueSymbol when possible.  */
  Value value;
  if (valP->Tag == ValueInt)
    {
      value = Value_Symbol (areaCurrentSymbol, 1, valP->Data.Int.i - areaOffset);
      valP = &value;
    }

  /* Branch instruction with value 0 means a branch to {PC} + 8 which is
     current area base + branch instruction offset + 8.
     So start to compensate with this value, the result is a value what needs
     to be added to that branch instruction with value 0.  */
  int branchInstrValue = -(offset + 8);

  assert (valP->Tag == ValueSymbol);
  assert (valP->Data.Symbol.factor == 1);
  if (valP->Data.Symbol.symbol != areaCurrentSymbol)
    {
      if (Reloc_Create (HOW2_INIT | HOW2_SIZE | HOW2_RELATIVE, offset, valP) == NULL)
	error (ErrorError, "Relocation failed");

      /* The R_ARM_PC24 ELF reloc needs to happen for a "B {PC}" instruction,
  	 while in AOF this needs to happen for a "B -<area origin>"
	 instruction.  */
      if (!option_aof)
	branchInstrValue += offset;
    }
  branchInstrValue += valP->Data.Symbol.offset;
  
  int mask = isBLX ? 1 : 3;
  if (branchInstrValue & mask)
    error (ErrorError, "Branch value is not a multiple of %s", isBLX ? "two" : "four");
  ARMWord ir = cc | 0x0A000000 | ((branchInstrValue >> 2) & 0xffffff) | (isBLX ? (branchInstrValue & 2) << 23 : 0);
  Put_Ins (4, ir);

  return false;
}

/**
 * Implements B and BL.
 */
bool
m_branch (bool doLowerCase)
{
  ARMWord cc = optionLinkCond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  return branch_shared (cc, false);
}

/**
 * Implements BLX.
 */
bool
m_blx (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  Target_NeedAtLeastArch (ARCH_ARMv5TE);

  ARMWord reg = Get_CPURegNoError ();
  if (reg == INVALID_REG)
    { /* BLXcc <target_addr> */
      if (cc != AL)
        error (ErrorError, "BLX <target_addr> must be unconditional");

      return branch_shared (NV, true);
    }
  else
    Put_Ins (4, cc | 0x012FFF30 | RHS_OP (reg)); /* BLX <Rm> */

  return false;
}

/**
 * Implements BX.
 */
bool
m_bx (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  Target_NeedAtLeastArch (ARCH_ARMv5TE);

  int dst = getCpuReg ();
  if (dst == 15)
    error (ErrorWarning, "Use of PC with BX is discouraged");

  Put_Ins (4, cc | 0x012fff10 | dst);
  return false;
}

/**
 * Implements BXJ.
 */
bool
m_bxj (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  Target_NeedAtLeastArch (ARCH_ARMv5TEJ);

  int dst = getCpuReg ();
  if (dst == 15)
    error (ErrorWarning, "Use of PC with BXJ is discouraged");

  Put_Ins (4, cc | 0x012fff20 | dst);
  return false;
}

/**
 * Implements SVC / SWI.
 *   SVC/SWI #<24 bit int>
 *   SVC/SWI <24 bit int>
 *   SVC/SWI <string>
 */
bool
m_swi (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  skipblanks ();
  ValueTag valueOK = Input_Match ('#', false) ? ValueInt : (ValueInt | ValueString);
  const Value *im = exprBuildAndEval (valueOK);
  ARMWord ir = cc | 0x0F000000;
  switch (im->Tag)
    {
      case ValueInt:
	ir |= fixSwi (0, im->Data.Int.i);
	break;

      case ValueString:
	{
#ifdef __riscos__
	  /* ValueString is not NUL terminated.  */
	  char swiname[im->Data.String.len + 1];
	  memcpy (swiname, im->Data.String.s, im->Data.String.len);
	  swiname[im->Data.String.len] = '\0';
	  ir |= switonum (swiname);
	  if (ir == 0xFFFFFFFF)
	    error (ErrorError, "Unknown SVC/SWI name");
#else
	  error (ErrorError, "RISC OS is required to look up the SVC/SWI name");
#endif
	}
	break;

      default:
	if (gPhase == ePassTwo)
	  error (ErrorError, "Illegal SVC/SWI expression");
	break;
    }
  Put_Ins (4, ir);
  return false;
}

/**
 * Implements BKPT.
 */
bool
m_bkpt (void)
{
  Target_NeedAtLeastArch (ARCH_ARMv5TE);

  if (Input_Match ('#', false))
    error (ErrorInfo, "BKPT is always immediate");
  const Value *im = exprBuildAndEval (ValueInt);
  int i;
  if (im->Tag != ValueInt)
    {
      error (ErrorError, "Illegal BKPT expression");
      i = 0;
    }
  else
    i = im->Data.Int.i;
  ARMWord val = Fix_Int (NULL, 0, 2, i);

  ARMWord ir = 0xE1200070 | ((val & 0xFFF0) << 4) | (val & 0xF);
  Put_Ins (4, ir);
  return false;
}

/**
 * \param constant Constant value relative to base register (R0 .. R15), or
 * absolute when base register is < 0.
 * \param baseReg Either 0 .. 15 for R0 ... R15 (relative constant).  When < 0,
 * there is no base register specified and constant is absolute.
 * When baseReg == 0 .. 14, 15 in non-ABS area :
 *     ADD/SUB Rx, baseReg, #... + ADD/SUB Rx, Rx, #...
 * When baseReg == 15 in ABS area :
 *     ADD/SUB Rx, R15, #... + ADD/SUB Rx, Rx, #...
 *  or
 *     MOV/MVN Rx, #... + ADD/SUB Rx, Rx, #...
 * When baseReg < 0 (non-ABS area) :
 *     MOV/MVN Rx, #... + ADD/SUB Rx, Rx, #...
 * When baseReg < 0 (ABS area) :
 *     ADD/SUB Rx, R15, #... + ADD/SUB Rx, Rx, #...
 *  or
 *     MOV/MVN Rx, #... + ADD/SUB Rx, Rx, #...
 * \param baseInstr Base instruction as returned from optionAdrL with bit 0
 * cleared.
 * \param isADRL true when the input was ADRL, false for ADR.
 * \param canSwitch true when we still can switch between ADR and ADRL, false
 * otherwise.
 */
static void
ADR_RelocUpdaterCore (int constant, int baseReg, uint32_t baseInstr,
		      bool isADRL, bool canSwitch)
{
  uint32_t offset = areaCurrentSymbol->area.info->curIdx;
  uint32_t areaOffset = (areaCurrentSymbol->area.info->type & AREA_ABS) ? Area_GetBaseAddress (areaCurrentSymbol) : 0;

  bool baseRegUnspecified = baseReg < 0;
  if (baseRegUnspecified)
    {
      /* Make constant relative to {PC} + 8.  */
      constant -= areaOffset + offset + 8;
      baseReg = 15;
    }
  
  /* FIXME: Can clever use of an ADD/SUB/ORR/BIC mixture cover more constants ? */
  struct
    {
      unsigned int try[4];
      int num;
    } split[4];
  if (baseRegUnspecified
      || (baseReg == 15 && (areaCurrentSymbol->area.info->type & AREA_ABS) != 0))
    {
      /* MOV/MVN Rx, #... [ + ADD/SUB Rx, Rx, #... ]  */
      int absConstant = constant + (areaOffset + offset + 8);
      split[0].num = Help_SplitByImm8s4 (absConstant, split[0].try); /* MOV [ + ADD ].  */
      split[1].num = Help_SplitByImm8s4 (~absConstant, split[1].try); /* MVN [ + SUB ].  */
    }
  else
    split[1].num = split[0].num = INT_MAX;
  /* FIXME: we can use MOVW as well.  */
  if (!baseRegUnspecified || (areaCurrentSymbol->area.info->type & AREA_ABS) != 0)
    {
      /* ADD/SUB Rx, baseReg, #... [ + ADD/SUB Rx, Rx, #... ] */
      split[2].num = Help_SplitByImm8s4 (constant, split[2].try); /* ADD [ + ADD ] (all baseReg).  */
      split[3].num = Help_SplitByImm8s4 (-constant, split[3].try); /* SUB [ + SUB ] (all baseReg).  */
    }
  else
    split[3].num = split[2].num = INT_MAX;

  int bestIndex = 0, bestScore = split[0].num;
  for (int i = 0; i != 4; ++i)
    {
      if (bestScore > split[i].num)
	{
	  bestScore = split[i].num;
	  bestIndex = i;
	}
    }
  assert (bestScore != INT_MAX && "At least a solution of 4 instr should have been found");

  if (bestScore >= 3
      || (bestScore == 2 && !isADRL /* FIXME: && !canSwitch*/))
    {
      if (areaCurrentSymbol->area.info->type & AREA_ABS)
	error (ErrorError, "%s at area offset 0x%x with base address 0x%x can not be used to encode [r%d, #0x%x]",
	       (isADRL) ? "ADRL" : "ADR", offset, areaOffset, baseReg, constant);
      else
	error (ErrorError, "%s at area offset 0x%x can not be used to encode [r%d, #0x%x]",
	       (isADRL) ? "ADRL" : "ADR", offset, baseReg, constant);
      bestScore = isADRL ? 2 : 1;
    }

  if (bestScore == 1 && isADRL)
    {
      if (areaCurrentSymbol->area.info->type & AREA_ABS)
	error (ErrorWarning, "ADR instead of ADRL can be used at area offset 0x%x with base address 0x%x to encode [r%d, #0x%x]",
	       offset, areaOffset, baseReg, constant);
      else
	error (ErrorWarning, "ADR instead of ADRL can be used at area offset 0x%x to encode [r%d, #0x%x]",
	       offset, baseReg, constant);
      /* We could switch to ADR (when canSwitch is true) but I don't think this is always really wanted.  */
      bestScore = 2;
      split[bestIndex].try[1] = 0;
    }
  else if (bestScore == 2 && !isADRL)
    {
      assert (0); /* FIXME: */
      assert (canSwitch);
      /* We switch from ADR to ADRL because there is no other option.  */
      if (areaCurrentSymbol->area.info->type & AREA_ABS)
	error (ErrorWarning, "Using ADRL instead of ADR at area offset 0x%x with base address 0x%x to encode [r%d, #0x%x]",
	       offset, areaOffset, baseReg, constant);
      else
	error (ErrorWarning, "Using ADRL instead of ADR at area offset 0x%x to encode [r%d, #0x%x]",
	       offset, baseReg, constant);
    }

  ARMWord irop1, irop2;
  switch (bestIndex)
    {
      case 0:
	irop1 = M_MOV;
	irop2 = M_ADD;
	break;
      case 1:
	irop1 = M_MVN;
	irop2 = M_SUB;
	break;
      case 2:
	irop2 = irop1 = M_ADD;
	break;
      case 3:
	irop2 = irop1 = M_SUB;
	break;
      default:
	assert (0);
	break;
    }

  if (bestScore == 2 && GET_DST_OP(baseInstr) == 15)
    error (ErrorError, "ADRL can not be used with register 15 as destination");

  for (int n = 0; n != bestScore; ++n)
    {
      /* Fix up the base register.  */
      ARMWord irs = baseInstr | (n == 0 ? irop1 : irop2);
      if (bestIndex >= 2 /* = when ADD/SUB is used */ || n != 0)
        irs = irs | LHS_OP (n == 0 ? (ARMWord)baseReg : GET_DST_OP(baseInstr));

      int i8s4 = help_cpuImm8s4 (split[bestIndex].try[n]);
      assert (i8s4 != -1);
      irs |= i8s4;

      Put_Ins (4, irs);
    }
}

/**
 * Implements ADR / ADRL.
 */
bool
m_adr (bool doLowerCase)
{
  ARMWord ir = optionAdrL (doLowerCase);
  if (ir == kOption_NotRecognized)
    return true;

  ARMWord regD = getCpuReg ();
  
  skipblanks ();
  if (!Input_Match (',', false))
    error (ErrorError, "%sdst", InsertCommaAfter);

  bool isADRL = ir & 1;
  ir &= ~1;
  
  ValueTag tag = (gPhase == ePassOne) ? ValueInt | ValueAddr : ValueInt | ValueAddr | ValueSymbol;
  const Value *valP = exprBuildAndEval (tag);

  if (valP->Tag == ValueIllegal
      || (valP->Tag == ValueSymbol && valP->Data.Symbol.factor != 1))
    {
      if (gPhase == ePassOne)
	{
	  /* We have unresolved symbols.  Wait until pass two to do the work.
	     This means also that when ADRL is used, we can't go back to ADR
	     anymore.  */
	  Put_Ins (4, 0);
	  if (isADRL)
	    Put_Ins (4, 0);
	}
      else
	error (ErrorError, "Illegal %s expression", isADRL ? "ADRL" : "ADR");
      return false;
    }

  uint32_t offset = areaCurrentSymbol->area.info->curIdx;
  uint32_t areaOffset = (areaCurrentSymbol->area.info->type & AREA_ABS) ? Area_GetBaseAddress (areaCurrentSymbol) : 0;
  /* If we come here during PassOne, we're at liberty to change ADR into ADRL
   * and/or ADRL into ADR.  When we come here at PassTwo, it might be that this
   * is the first time (as during PassOne we were unable to resolve the ADR(L)
   * expression into a ValueInt/ValueAddr) so it is important that for this
   * case we don't do any ADR vs ADRL swapping but also it is important that
   * we're making the same choice for 'canSwitch' for the case where we arrived
   * here during PassOne.
   * The latter means that the first instruction is no longer 0.
   */
  bool canSwitch = gPhase == ePassOne || GetWord (offset) != 0;

  /* Switch from current area symbol to [r15, #...].  */
  Value value;
  if (valP->Tag == ValueSymbol && valP->Data.Symbol.symbol == areaCurrentSymbol)
    {
      value = Value_Addr (15, valP->Data.Symbol.offset - (offset + 8));
      valP = &value;
    }

  int constant, baseReg;
  switch (valP->Tag)
    {
      case ValueInt:
	/* Absolute value : results in MOV/MVN (followed by ADD/SUB in case of
	   ADRL).  */
	constant = valP->Data.Int.i;
	baseReg = -1;
	break;

      case ValueAddr:
	constant = valP->Data.Addr.i;
	baseReg = valP->Data.Addr.r;
	break;

      case ValueSymbol:
	assert (valP->Data.Symbol.factor == 1);
	if (Reloc_Create (HOW2_INIT | HOW2_SIZE | HOW2_RELATIVE, offset, valP) == NULL)
	  error (ErrorError, "Relocation failed");
	constant = valP->Data.Symbol.offset - (areaOffset + offset + 8);
	baseReg = 15;
	break;

      default:
	assert (0);
	break;
    }
  ADR_RelocUpdaterCore (constant, baseReg, ir | DST_OP (regD) | IMM_RHS,
			isADRL, canSwitch);

  return false;
}

/* PSR access */

static ARMWord
getpsr (bool only_all)
{
  skipblanks ();

  /* Read "APSR", "CPSR" or "SPSR".  */
  bool isAPSR = false;
  ARMWord saved;
  switch (inputGetLower ())
    {
      case 'a':
	isAPSR = true;
	/* Fall through.  */
      case 'c':
        saved = 0;
        break;
      case 's':
        saved = 1 << 22;
        break;
      default:
        error (ErrorError, "Not a PSR name (expected 'APSR', 'CPSR' or 'SPSR')");
        return 0;
    }
  if (inputGetLower () != 'p'
      || inputGetLower () != 's'
      || inputGetLower () != 'r')
    {
      error (ErrorError, "Not a PSR name (expected 'APSR', 'CPSR' or 'SPSR')");
      return 0;
    }

  if (isAPSR)
    {
      if (only_all)
	saved |= 0xF<<16;
      else
	{
	  /* Process "APSR_<bits> with <bits> = one of 'nzcvq', 'g' or 'nzcvqg'.  */
	  bool ok;
	  if ((ok = Input_Match ('_', false)) != false)
	    {
	      if (Input_MatchString ("nzcvq"))
		saved |= 1<<19;
	      if (Input_Match ('g', false))
		saved |= 1<<18;
	      ok = Input_IsEndOfKeyword () || inputLook () == ',';
	    }
	  if (!ok)
	    error (ErrorError, "Expected one of 'nzcvq', 'g' or 'nzcvqg'");
	}
    }
  else
    {
      bool giveLegacyWarning;
      if (!Input_Match ('_', false))
	{
	  giveLegacyWarning = !only_all;
	  saved |= only_all ? 0xF<<16 : 0x9<<16;
	}
      else
	{
	  const char * const inputMark = Input_GetMark ();
	  char w[3];
	  w[0] = inputGetLower ();
	  w[1] = inputGetLower ();
	  w[2] = inputGetLower ();
	  if (!memcmp (w, "all", sizeof ("all")-1))
	    {
	      giveLegacyWarning = true;
	      saved |= only_all ? 0xF<<16 : 0x9<<16;
	    }
	  else if (only_all)
	    {
	      error (ErrorError, "Partial PSR access not allowed");
	      return 0;
	    }
	  else if (!memcmp (w, "ctl", sizeof ("ctl")-1))
	    {
	      giveLegacyWarning = true;
	      saved |= 0x1<<16;
	    }
	  else if (!memcmp (w, "flg", sizeof ("flg")-1))
	    {
	      giveLegacyWarning = true;
	      saved |= 0x8<<16;
	    }
	  else
	    {
	      Input_RollBackToMark (inputMark);
	      giveLegacyWarning = false;

	      int p;
	      do
		{
		  char c;
		  switch (c = inputLookLower ())
		    {
		      case 'c':
			p = 16;
			break;
		      case 'x':
			p = 17;
			break;
		      case 's':
			p = 18;
			break;
		      case 'f':
			p = 19;
			break;
		      default:
			p = 0;
			break;
		    }
		  if (p)
		    {
		      if (saved & (1 << p))
			error (ErrorError, "PSR mask bit '%c' already specified", c);
		      inputSkip ();
		      saved |= 1 << p;
		    }
		} while (p);
	    }
	}
      if (giveLegacyWarning)
	error (ErrorWarning, "The CPSR, CPSR_flg, CPSR_ctl, CPSR_all, SPSR, SPSR_flg, SPSR_ctl and SPSR_all forms of PSR field specification have been superseded by the csxf format");
    }

  return saved;
}

/**
 * Implements MSR.
 */
bool
m_msr (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  Target_NeedAtLeastArch (ARCH_ARMv3);

  cc |= getpsr (false) | 0x0120F000;
  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%slhs", InsertCommaAfter);
  if (Input_Match ('#', false))
    {
      const Value *im = exprBuildAndEval (ValueInt);
      if (im->Tag == ValueInt)
	{
	  cc |= 0x02000000;
	  cc |= fixImm8s4 (0, cc, im->Data.Int.i); /* FIXME: no rotator support (see getRhs()) ? */
	}
      else
	error (ErrorError, "Illegal immediate expression");
      if (cc & ((1<<17) | (1<<18)))
	error (ErrorWarning, "Writing immediate value to status or extension field of CPSR/SPSR is inadvisable");
    }
  else
    cc |= getCpuReg ();
  Put_Ins (4, cc);
  return false;
}

/**
 * Implements MRS.
 */
bool
m_mrs (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  Target_NeedAtLeastArch (ARCH_ARMv3);

  cc |= getCpuReg () << 12 | 0x01000000;
  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%slhs", InsertCommaAfter);
  cc |= getpsr (true);
  Put_Ins (4, cc);
  return false;
}


/**
 * Implements SEV.
 *   SEV<cond>
 */
bool
m_sev (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  if (Target_GetArch() != ARCH_ARMv6K)
    Target_NeedAtLeastArch (ARCH_ARMv7);
  
  Put_Ins (4, cc | 0x0320F004);
  return false;
}


/**
 * Implements WFE.
 *   WFE<cond>
 */
bool
m_wfe (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  if (Target_GetArch() != ARCH_ARMv6K)
    Target_NeedAtLeastArch (ARCH_ARMv7);
  
  Put_Ins (4, cc | 0x0320F002);
  return false;
}


/**
 * Implements WFI.
 *   WFI<cond>
 */
bool
m_wfi (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  if (Target_GetArch() != ARCH_ARMv6K)
    Target_NeedAtLeastArch (ARCH_ARMv7);
  
  Put_Ins (4, cc | 0x0320F003);
  return false;
}


/**
 * Implements YIELD.
 *   YIELD<cond>
 */
bool
m_yield (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  Put_Ins (4, cc | 0x0320F001);
  return false;
}


/**
 * Implements CPS.
 *   CPS<effect> <iflags>{, #<mode>}
 *   CPS #<mode>
 * where:
 *   effect   is one of:
 *            IE  Interrupt or abort enable.
 *            ID  Interrupt or abort disable.
 *   iflags   is a sequence of one or more of:
 *            a : Enables or disables imprecise aborts.
 *            i : Enables or disables IRQ interrupts.
 *            f : Enables or disables FIQ interrupts.
 *   mode     specifies the number of the mode to change to.
 */
bool
m_cps (bool doLowerCase)
{
  int imod;
  if (Input_IsEndOfKeyword ())
    imod = 0<<18;
  else if (Input_MatchKeyword (doLowerCase ? "id" : "ID"))
    imod = 3<<18;
  else if (Input_MatchKeyword (doLowerCase ? "ie" : "IE"))
    imod = 2<<18;
  else
    return true;
  skipblanks ();

  bool readMode;
  int iflags = 0;
  if (imod)
    {
      /* Read iflags.  */
      if (inputLookLower () == 'a')
	{
	  inputSkip ();
	  iflags |= 1<<8;
	}
      if (inputLookLower () == 'i')
	{
	  inputSkip ();
	  iflags |= 1<<7;
	}
      if (inputLookLower () == 'f')
	{
	  inputSkip ();
	  iflags |= 1<<6;
	}
      if (iflags == 0)
	error (ErrorWarning, "CPS did not have any interrupt disable flags specified");
      skipblanks ();
      readMode = Input_Match (',', true);
    }
  else
    readMode = true;

  int mode = 0;
  if (readMode)
    {
      if (!Input_Match ('#', true))
	{
	  readMode = false;
	  error (ErrorError, "%s needs a mode specified", "CPS");
	}
      else
	{
	  const Value *val = exprBuildAndEval (ValueInt);
          if (val->Tag != ValueInt)
	    {
	      readMode = false;
	      error (ErrorError, "Illegal immediate expression");
	    }
	  else
	    {
	      mode = val->Data.Int.i;
	      if (!Option_IsValidARMMode (mode))
		{
		  error (ErrorWarning, "Mode 0x%x is not a valid ARM mode", mode);
		  mode &= 0x1F;
		}
	    }
	}
    }
  assert(!(((imod == (0<<18) || imod == (1<<18)) && !readMode) || (imod == (1<<18) && readMode)) && "We shouldn't be generating this");
  Put_Ins (4, (0xF << 28) | (1<<24) | imod | (readMode ? (1<<17) : 0) | iflags | mode);
  return false;
}

/**
 * Implements DBG.
 *   DBG<cond> #<option>
 */
bool
m_dbg (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  if (Target_NeedAtLeastArch (ARCH_ARMv7))
    return true;

  skipblanks ();
  if (!Input_Match ('#', false))
    error (ErrorError, "DBG option value missing");
  else
    {
      const Value *im = exprBuildAndEval (ValueInt);
      switch (im->Tag)
	{
	  case ValueInt:
	    {
	      int optValue = im->Data.Int.i;
	      if (optValue < 0 || optValue >= 16)
		error (ErrorError, "DBG option value needs to be between 0 and 15");
	      else
		Put_Ins (4, cc | 0x0320F0F0 | optValue);
	      break;
	    }
	  default:
	    error (ErrorError, "Unknown DBG option value type");
	    break;
	}
    }

  return false;
}


/**
 * Implements SMC (and pre-UAL SMI).
 *   SMC<c> #<imm4>
 */
bool
m_smc (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  /* Note that the security extensions are optional.  */
  if (Target_NeedAtLeastArch (ARCH_ARMv6K))
    return true;

  skipblanks ();
  if (!Input_Match ('#', false))
    error (ErrorError, "SMC value missing");
  else
    {
      const Value *im = exprBuildAndEval (ValueInt);
      switch (im->Tag)
	{
	  case ValueInt:
	    {
	      int smcValue = im->Data.Int.i;
	      if (smcValue < 0 || smcValue >= 16)
		error (ErrorError, "SMC value needs to be between 0 and 15");
	      else
		Put_Ins (4, cc | 0x01600070 | smcValue);
	      break;
	    }
	  default:
	    error (ErrorError, "Unknown SMC value type");
	    break;
	}
    }

  return false;
}
