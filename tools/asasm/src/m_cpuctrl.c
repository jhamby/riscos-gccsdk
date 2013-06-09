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
#include "reloc.h"
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
  const uint32_t offset = (areaCurrentSymbol->area->curIdx + instrAlign-1) & -instrAlign;
  const uint32_t areaOffset = (areaCurrentSymbol->area->type & AREA_ABS) ? Area_GetBaseAddress (areaCurrentSymbol) : 0;

  if (Expr_Build ())
    Error (ErrorError, "Failed to parse expression");

  if (gPhase == ePassOne)
    {
      Put_Ins (4, 0);
      return false;
    }

  const Value *valP = Expr_Eval (ValueInt | ValueSymbol); /* FIXME: support ValueAddr  ? */
  if (valP->Tag == ValueIllegal
      || (valP->Tag == ValueSymbol && valP->Data.Symbol.factor != 1))
    {
      Error (ErrorError, "Illegal branch expression");
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
      Reloc_CreateAOF (HOW2_INIT | HOW2_INSTR_UNLIM | HOW2_RELATIVE, offset, valP);
      /* BL and BLX use R_ARM_CALL (the linker can change BL to BLX when it
	 notices a jump to Thumb code).
         B and BL<cond> use R_ARM_JUMP24 (a linker can create a veneer to
 	 jump to Thumb code).  */
      Reloc_CreateELF (isBLX || ((cc & LINK_BIT) != 0 && (cc & NV) == AL) ? R_ARM_CALL : R_ARM_JUMP24,
                       offset, valP);

      /* The R_ARM_CALL/R_ARM_JUMP24 ELF reloc needs to happen for a
 	 "B {PC}" instruction, while in AOF this needs to happen for a
	 "B -<area origin>" instruction.  */
      if (!option_aof)
	branchInstrValue += offset;
    }
  branchInstrValue += valP->Data.Symbol.offset;
  
  int mask = isBLX ? 1 : 3;
  if (branchInstrValue & mask)
    Error (ErrorError, "Branch value is not a multiple of %s", isBLX ? "two" : "four");
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
  ARMWord cc = Option_LinkCond (doLowerCase);
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
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  Target_CheckCPUFeature (kCPUExt_v5T, true);

  ARMWord reg = Get_CPURegNoError ();
  if (reg == INVALID_REG)
    { /* BLXcc <target_addr> */
      if (cc != AL)
        Error (ErrorError, "BLX <target_addr> must be unconditional");

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
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  Target_CheckCPUFeature (kCPUExt_v4T, true);

  unsigned dst = Get_CPUReg ();
  if (dst == 15)
    Error (ErrorWarning, "Use of PC with BX is discouraged");

  /* Output R_ARM_V4BX with nul symbol so that linker can create ARMv4
     compatible images by changing this BX instruction into MOV PC, Rx.  */
  if (gPhase == ePassTwo)
    Reloc_CreateELF (R_ARM_V4BX, areaCurrentSymbol->area->curIdx, NULL);

  Put_Ins (4, cc | 0x012fff10 | dst);
  return false;
}

/**
 * Implements BXJ.
 */
bool
m_bxj (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  Target_CheckCPUFeature (kCPUExt_v5J, true);

  unsigned dst = Get_CPUReg ();
  if (dst == 15)
    Error (ErrorWarning, "Use of PC with BXJ is discouraged");

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
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  Input_SkipWS ();
  ValueTag valueOK = Input_Match ('#', false) ? ValueInt : (ValueInt | ValueString);
  const Value *im = Expr_BuildAndEval (valueOK);
  ARMWord ir = cc | 0x0F000000;
  switch (im->Tag)
    {
      case ValueInt:
	{
	  unsigned swiNum = im->Data.Int.i;
	  if ((swiNum & 0xffffff) != swiNum)
	    Error (ErrorError, "Illegal swi number %d(0x%08x)", swiNum, swiNum);
	  ir |= swiNum;
	  break;
	}

      case ValueString:
	{
#ifdef __riscos__
	  /* ValueString is not NUL terminated.  */
	  char swiname[im->Data.String.len + 1];
	  memcpy (swiname, im->Data.String.s, im->Data.String.len);
	  swiname[im->Data.String.len] = '\0';
	  ir |= OS_SWINameToNum (swiname);
	  if (ir == 0xFFFFFFFF)
	    Error (ErrorError, "Unknown SVC/SWI name");
#else
	  Error (ErrorError, "RISC OS is required to look up the SVC/SWI name");
#endif
	  break;
	}

      default:
	if (gPhase == ePassTwo)
	  Error (ErrorError, "Illegal SVC/SWI expression");
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
  Target_CheckCPUFeature (kCPUExt_v5, true);

  if (Input_Match ('#', false))
    Error (ErrorInfo, "BKPT is always immediate");
  const Value *im = Expr_BuildAndEval (ValueInt);
  int i;
  if (im->Tag != ValueInt)
    {
      Error (ErrorError, "Illegal BKPT expression");
      i = 0;
    }
  else
    i = im->Data.Int.i;
  ARMWord val = Fix_Int (2, i);

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
 *
 * When baseReg == 15 in ABS area :
 *     ADD/SUB Rx, R15, #... + ADD/SUB Rx, Rx, #...
 *  or
 *     MOV/MVN Rx, #... + ADD/SUB Rx, Rx, #...
 *     MOVW Rx, #... (ARMv6T2, ARMv7 only)
 *     MOV32 Rx, #... (ARMv6T2, ARMv7 only)
 *
 * When baseReg < 0 (non-ABS area) :
 *     MOV/MVN Rx, #... + ADD/SUB Rx, Rx, #...
 *     MOVW Rx, #... (ARMv6T2, ARMv7 only)
 *     MOV32 Rx, #... (ARMv6T2, ARMv7 only)
 *
 * When baseReg < 0 (ABS area) :
 *     ADD/SUB Rx, R15, #... + ADD/SUB Rx, Rx, #...
 *  or
 *     MOV/MVN Rx, #... + ADD/SUB Rx, Rx, #...
 *     MOVW Rx, #... (ARMv6T2, ARMv7 only)
 *     MOV32 Rx, #... (ARMv6T2, ARMv7 only)
 * \param baseInstr Base instruction as returned from Option_ADRL with bit 0
 * cleared.
 * \param isADRL true when the input was ADRL, false for ADR.
 * \param canSwitch true when we still can switch between ADR and ADRL, false
 * otherwise.
 * 
 * In an ABS area, there is no distinction between:
 *   ADR(L) Rx, <integer value>
 * and
 *   ADR(L) Rx, <label in ABS area>
 * In principle both can be implemented using MOV/MVN (followed by ADD/SUB for
 * ADRL) and ADD/SUB (followed by ADD/SUB for ADRL).
 * We have a preference for the ADD/SUB as sometimes people use ORG for module
 * creation and only the ADD/SUB flavour will be correct when they were using
 * a label in their ABS area.
 */
static void
ADR_RelocUpdaterCore (uint32_t constant, int baseReg, uint32_t baseInstr,
		      bool isADRL, bool canSwitch)
{
  uint32_t offset = areaCurrentSymbol->area->curIdx;
  uint32_t areaOffset = (areaCurrentSymbol->area->type & AREA_ABS) ? Area_GetBaseAddress (areaCurrentSymbol) : 0;

  /* When we don't have a base register, use PC as base register but only
     for an ABS area.  */
  bool baseRegUnspecified = baseReg < 0;
  if (baseRegUnspecified
      && (areaCurrentSymbol->area->type & AREA_ABS) != 0)
    {
      /* Make constant relative to {PC} + 8.  */
      constant -= areaOffset + offset + 8;
      baseReg = 15;
      baseRegUnspecified = false;
    }
  
  /* FIXME: Can clever use of an ADD/SUB/ORR/BIC mixture cover more constants ? */
  struct
    {
      uint32_t try[4];
      unsigned num;
    } split[6];
  if (!baseRegUnspecified)
    {
      /* ADD/SUB Rx, baseReg, #... [ + ADD/SUB Rx, Rx, #... ] */
      split[0].num = HelpCPU_SplitByImm8s4 (constant, split[0].try); /* ADD [ + ADD ] (all baseReg).  */
      split[1].num = HelpCPU_SplitByImm8s4 (-constant, split[1].try); /* SUB [ + SUB ] (all baseReg).  */
    }
  else
    split[1].num = split[0].num = INT_MAX;
  if (baseRegUnspecified
      || (baseReg == 15 && (areaCurrentSymbol->area->type & AREA_ABS) != 0))
    {
      /* MOV/MVN Rx, #... [ + ADD/SUB Rx, Rx, #... ]  */
      uint32_t absConstant = baseRegUnspecified ? constant : constant + (areaOffset + offset + 8);
      split[2].num = HelpCPU_SplitByImm8s4 (absConstant, split[2].try); /* MOV [ + ADD ].  */
      split[3].num = HelpCPU_SplitByImm8s4 (~absConstant, split[3].try); /* MVN [ + SUB ].  */
    }
  else
    split[3].num = split[2].num = INT_MAX;
  /* MOVW or MOV32 */
  if (Target_CheckCPUFeature (kCPUExt_v6T2, false)
      && (baseRegUnspecified
          || (baseReg == 15 && (areaCurrentSymbol->area->type & AREA_ABS) != 0)))
    {
      uint32_t absConstant = baseRegUnspecified ? constant : constant + (areaOffset + offset + 8);
      split[4].num = absConstant < 0x10000 ? 1 : INT_MAX; /* MOVW.  */
      split[4].try[0] = absConstant & 0xFFFF;
      split[5].num = 2; /* MOV32 can encode all 32-bit constants.  */
      split[5].try[0] = absConstant & 0xFFFF;
      split[5].try[1] = absConstant >> 16;
    }
  else
    split[5].num = split[4].num = INT_MAX;

  unsigned bestIndex = 0, bestScore = split[0].num;
  for (unsigned i = 0; i != sizeof (split)/sizeof (split[0]); ++i)
    {
      if (bestScore > split[i].num)
	{
	  bestScore = split[i].num;
	  bestIndex = i;
	}
    }
  assert (bestScore != INT_MAX && "At least a solution of 4 instr should have been found");

  char toEncode[32]; /* Textual representation what we're trying to encode by preference.  */
  if (baseRegUnspecified)
    sprintf (toEncode, "&%x", constant);
  else
    {
      if (constant > INT32_MAX)
	sprintf (toEncode, "[r%d, #-&%x]", baseReg, ~constant + 1);
      else
	sprintf (toEncode, "[r%d, #&%x]", baseReg, constant);
    }
  if (bestScore >= 3 || (bestScore == 2 && !isADRL))
    {
      if ((areaCurrentSymbol->area->type & AREA_ABS) != 0)
	Error (ErrorError, "%s at area offset 0x%x with base address 0x%x can not be used to encode %s",
	       (isADRL) ? "ADRL" : "ADR", offset, areaOffset, toEncode);
      else
	Error (ErrorError, "%s at area offset 0x%x can not be used to encode %s",
	       (isADRL) ? "ADRL" : "ADR", offset, toEncode);
      bestScore = isADRL ? 2 : 1;
    }

  if (bestScore == 1 && isADRL)
    {
      if (canSwitch)
	{
	  if (areaCurrentSymbol->area->type & AREA_ABS)
	    Error (ErrorWarning, "Using ADR instead of ADRL at area offset 0x%x with base address 0x%x to encode %s",
		   offset, areaOffset, toEncode);
	  else
	    Error (ErrorWarning, "Using ADR instead of ADRL at area offset 0x%x to encode %s",
		   offset, toEncode);
	}
      else
	{
	  if (areaCurrentSymbol->area->type & AREA_ABS)
	    Error (ErrorWarning, "ADR instead of ADRL can be used at area offset 0x%x with base address 0x%x to encode %s",
		   offset, areaOffset, toEncode);
	  else
	    Error (ErrorWarning, "ADR instead of ADRL can be used at area offset 0x%x to encode %s",
		   offset, toEncode);
	}
    }
  else if (bestScore == 2 && !isADRL && canSwitch)
    {
      /* We switch from ADR to ADRL because there is no other option.  */
      if (areaCurrentSymbol->area->type & AREA_ABS)
	Error (ErrorWarning, "Using ADRL instead of ADR at area offset 0x%x with base address 0x%x to encode %s",
	       offset, areaOffset, toEncode);
      else
	Error (ErrorWarning, "Using ADRL instead of ADR at area offset 0x%x to encode %s",
	       offset, toEncode);
    }

  assert (bestScore == 1 || bestScore == 2);
  if (bestIndex < 4)
    {
      ARMWord irop1, irop2;
      switch (bestIndex)
	{
	  case 0:
	    irop2 = irop1 = M_ADD;
	    break;
	  case 1:
	    irop2 = irop1 = M_SUB;
	    break;
	  case 2:
	    irop1 = M_MOV;
	    irop2 = M_ADD;
	    break;
	  case 3:
	    irop1 = M_MVN;
	    irop2 = M_SUB;
	    break;
	}

      if (bestScore == 2 && GET_DST_OP(baseInstr) == 15)
	Error (ErrorError, "ADRL can not be used with register 15 as destination");

      for (unsigned n = 0; n != bestScore; ++n)
	{
	  /* Fix up the base register.  */
	  uint32_t irs = baseInstr | (n == 0 ? irop1 : irop2);
	  if (bestIndex < 2 /* = when ADD/SUB is used */ || n != 0)
	    irs = irs | LHS_OP (n == 0 ? (ARMWord)baseReg : GET_DST_OP(baseInstr));

	  uint32_t i8s4 = HelpCPU_Imm8s4 (split[bestIndex].try[n]);
	  assert (i8s4 != UINT32_MAX);
	  irs |= i8s4;

	  Put_Ins (4, irs);
	}
    }
  else
    {
      assert (bestIndex == 4 || bestIndex == 5);
      uint32_t cc = baseInstr & NV;
      uint32_t destReg = GET_DST_OP(baseInstr);
      Put_Ins_MOVW_MOVT (cc, destReg, split[bestIndex].try[0], false);
      if (bestScore == 2)
	{
	  assert (split[bestIndex].try[1] != 0);
	  Put_Ins_MOVW_MOVT (cc, destReg, split[bestIndex].try[1], true);
	}
    }
  if (bestScore == 1  && isADRL && !canSwitch)
    {
      /* We need to output two instructions.  Use NOP (its value depends
         on architecture support v6K and v6T2).  */
      Put_Ins (4, Target_CheckCPUFeature (kCPUExt_v6K, false)
                    || Target_CheckCPUFeature (kCPUExt_v6T2, false) ? 0xe320F000 : 0xe1a00000);
    }
}


/**
 * Implements ADR / ADRL.
 */
bool
m_adr (bool doLowerCase)
{
  ARMWord ir = Option_ADRL (doLowerCase);
  if (ir == kOption_NotRecognized)
    return true;

  unsigned regD = Get_CPUReg ();
  
  Input_SkipWS ();
  if (!Input_Match (',', false))
    Error (ErrorError, "%sdst", InsertCommaAfter);

  bool isADRL = ir & 1;
  ir &= ~1;
  
  ValueTag tag = (gPhase == ePassOne) ? ValueInt | ValueAddr : ValueInt | ValueAddr | ValueSymbol;
  const Value *valP = Expr_BuildAndEval (tag);

  if (valP->Tag == ValueIllegal
      || (valP->Tag == ValueInt && valP->Data.Int.type != eIntType_PureInt)
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
	Error (ErrorError, "Illegal %s expression", isADRL ? "ADRL" : "ADR");
      return false;
    }

  uint32_t offset = areaCurrentSymbol->area->curIdx;
  uint32_t areaOffset = (areaCurrentSymbol->area->type & AREA_ABS) ? Area_GetBaseAddress (areaCurrentSymbol) : 0;
  /* If we come here during PassOne, we're at liberty to change ADR into ADRL
   * and/or ADRL into ADR.  When we come here at PassTwo, it might be that this
   * is the first time (as during PassOne we were unable to resolve the ADR(L)
   * expression into a ValueInt/ValueAddr) so it is important that for this
   * case we don't do any ADR vs ADRL swapping but also it is important that
   * we're making the same choice for 'canSwitch' for the case where we arrived
   * here during PassOne.
   * The latter means that the first instruction is no longer 0.
   */
  bool canSwitch = gPhase == ePassOne || Put_GetWord (offset) != 0;

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
	   ADRL) or MOVW or MOV32.  */
	constant = valP->Data.Int.i;
	baseReg = -1;
	break;

      case ValueAddr:
	constant = valP->Data.Addr.i;
	baseReg = valP->Data.Addr.r;
	break;

      case ValueSymbol:
	assert (valP->Data.Symbol.factor == 1);
	Reloc_CreateAOF (HOW2_INIT | HOW2_INSTR_UNLIM | HOW2_RELATIVE, offset, valP);
	// FIXME: Reloc_CreateELF() missing.
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
  Input_SkipWS ();

  /* Read "APSR", "CPSR" or "SPSR".  */
  bool isAPSR = false;
  ARMWord saved;
  switch (Input_GetCLower ())
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
        Error (ErrorError, "Not a PSR name (expected 'APSR', 'CPSR' or 'SPSR')");
        return 0;
    }
  if (Input_GetCLower () != 'p'
      || Input_GetCLower () != 's'
      || Input_GetCLower () != 'r')
    {
      Error (ErrorError, "Not a PSR name (expected 'APSR', 'CPSR' or 'SPSR')");
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
	      ok = Input_IsEndOfKeyword () || Input_Look () == ',';
	    }
	  if (!ok)
	    Error (ErrorError, "Expected one of 'nzcvq', 'g' or 'nzcvqg'");
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
	  w[0] = Input_GetCLower ();
	  w[1] = Input_GetCLower ();
	  w[2] = Input_GetCLower ();
	  if (!memcmp (w, "all", sizeof ("all")-1))
	    {
	      giveLegacyWarning = true;
	      saved |= only_all ? 0xF<<16 : 0x9<<16;
	    }
	  else if (only_all)
	    {
	      Error (ErrorError, "Partial PSR access not allowed");
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
		  switch (c = Input_LookLower ())
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
			Error (ErrorError, "PSR mask bit '%c' already specified", c);
		      Input_Skip ();
		      saved |= 1 << p;
		    }
		} while (p);
	    }
	}
      if (giveLegacyWarning)
	Error (ErrorWarning, "The CPSR, CPSR_flg, CPSR_ctl, CPSR_all, SPSR, SPSR_flg, SPSR_ctl and SPSR_all forms of PSR field specification have been superseded by the csxf format");
    }

  return saved;
}

/**
 * Implements MSR.
 */
bool
m_msr (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  Target_CheckCPUFeature (kCPUExt_v3, true);

  cc |= getpsr (false) | 0x0120F000;
  Input_SkipWS ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%slhs", InsertCommaAfter);
  if (Input_Match ('#', false))
    {
      const Value *im = Expr_BuildAndEval (ValueInt);
      if (im->Tag == ValueInt)
	{
	  cc |= 0x02000000;
	  cc |= Fix_Imm8s4 (cc, im->Data.Int.i); /* FIXME: no rotator support (see getRhs()) ? */
	}
      else
	Error (ErrorError, "Illegal immediate expression");
      if (cc & ((1<<17) | (1<<18)))
	Error (ErrorWarning, "Writing immediate value to status or extension field of CPSR/SPSR is inadvisable");
    }
  else
    cc |= Get_CPUReg ();
  Put_Ins (4, cc);
  return false;
}

/**
 * Implements MRS.
 */
bool
m_mrs (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  Target_CheckCPUFeature (kCPUExt_v3, true);

  cc |= Get_CPUReg () << 12 | 0x01000000;
  Input_SkipWS ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%slhs", InsertCommaAfter);
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
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  Target_CheckCPUFeature (kCPUExt_v6K, true);
  
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
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  Target_CheckCPUFeature (kCPUExt_v6K, true);
  
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
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  Target_CheckCPUFeature (kCPUExt_v6K, true);
  
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
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  Put_Ins (4, cc | 0x0320F001);
  return false;
}


/**
 * Implements CPS.
 *   CPS<effect>[<q>] <iflags>[", #" <mode>]
 *   CPS[<q>] #<mode>
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
  bool effectSpecified = Input_LookN (0) == (doLowerCase ? 'i' : 'I');
  bool effectEnable;
  if (effectSpecified)
    {
      effectEnable = Input_LookN (1) == (doLowerCase ? 'e' : 'E');
      if (!effectEnable && Input_LookN (1) != (doLowerCase ? 'd' : 'D'))
	return true;
      Input_SkipN (2);
    }

  InstrWidth_e instrWidth = Option_GetInstrWidth (doLowerCase);
  if (instrWidth == eInstrWidth_Unrecognized)
    return true;

  Input_SkipWS ();
  bool readMode;
  ARMWord iflags = 0;
  if (effectSpecified)
    {
      /* Read iflags.  */
      while (!Input_IsEolOrCommentStart ()
             && !(readMode = Input_Match (',', true)))
	{
	  const ARMWord iflagsBefore = iflags;
	  switch (Input_LookLower ())
	    {
	      case 'a':
		iflags |= 1<<2;
		break;
	      case 'i':
		iflags |= 1<<1;
		break;
	      case 'f':
		iflags |= 1<<0;
		break;
	      default:
		Error (ErrorError, "Unknown interrupt mask bit specification");
		(void) Input_Rest ();
		break;
	    }
	  if (iflagsBefore == iflags)
	    Error (ErrorWarning, "Interrupt mask bit already specified");
	  Input_Skip ();
	  Input_SkipWS ();
	}
    }
  else
    readMode = true;

  ARMWord mode = 0;
  if (readMode)
    {
      if (!Input_Match ('#', true))
	{
	  readMode = false;
	  Error (ErrorError, "%s needs a mode specified", "CPS");
	}
      else
	{
	  const Value *val = Expr_BuildAndEval (ValueInt);
          if (val->Tag != ValueInt)
	    {
	      readMode = false;
	      Error (ErrorError, "Illegal immediate expression");
	    }
	  else
	    {
	      mode = val->Data.Int.i;
	      if (!Option_IsValidARMMode (mode))
		{
		  Error (ErrorWarning, "Mode 0x%x is not a valid ARM mode", mode);
		  mode &= 0x1F;
		}
	    }
	}
    }

  InstrType_e instrState = State_GetInstrType ();
  IT_ApplyCond (AL, instrState != eInstrType_ARM);

  /* We need Thumb2 when mode is specified.  */
  if (instrState != eInstrType_ARM && instrWidth == eInstrWidth_NotSpecified)
    instrWidth = readMode ? eInstrWidth_Enforce32bit : eInstrWidth_Enforce16bit;
  else if (instrWidth == eInstrWidth_Enforce16bit && readMode)
    {
      Error (ErrorError, "Narrow instruction qualifier for Thumb can not have mode specified");
      instrWidth = eInstrWidth_Enforce32bit;
    }

  if (effectSpecified && iflags == 0)
    Error (ErrorWarning, "Enabling/disabling interrupt without interrupt mask bits specified is UNPREDICTABLE");

  if (instrState == eInstrType_ARM)
    {
      Target_CheckCPUFeature (kCPUExt_v6, true);
      ARMWord imod;
      if (effectSpecified)
	imod = effectEnable ? (2<<18) : (3<<18);
      else
	imod = 0<<18;
      Put_Ins (4, 0xF1000000 | imod | (readMode<<17) | (iflags<<6) | mode);
    }
  else if (instrWidth == eInstrWidth_Enforce16bit)
    {
      Target_CheckCPUFeature (kCPUExt_v6, true);
      assert (!readMode);
      Put_Ins (2, 0xB660 | (!effectEnable<<4) | iflags);
    }
  else
    {
      Target_CheckCPUFeature (kCPUExt_v6T2, true);
      ARMWord imod;
      if (effectSpecified)
	imod = effectEnable ? (2<<9) : (3<<9);
      else
	imod = 0<<9;
      Put_Ins (4, 0xF3af8000 | imod | (readMode<<8) | (iflags<<5) | mode);
    }

  return false;
}


/**
 * Implements DBG.
 *   DBG[<cc>][<q>] ["#"]<option>
 */
bool
m_dbg (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  InstrWidth_e instrWidth = Option_GetInstrWidth (doLowerCase);
  if (instrWidth == eInstrWidth_Unrecognized)
    return true;

  unsigned dbgValue = 0;
  Input_SkipWS ();
  if (Input_IsEolOrCommentStart ())
    Error (ErrorError, "%s value missing", "DBG");
  else
    {
      /* Skip optional '#'.  */
      (void) Input_Match ('#', false);
      const Value *im = Expr_BuildAndEval (ValueInt);
      switch (im->Tag)
	{
	  case ValueInt:
	    dbgValue = im->Data.Int.i;
	    break;
	  default:
	    Error (ErrorError, "Unknown %s value type", "DBG");
	    break;
	}
      if (dbgValue >= 16)
	{
	  Error (ErrorError, "%s value needs to be between 0 and 15 (incl)", "DBG");
	  dbgValue = 0;
	}
    }

  InstrType_e instrState = State_GetInstrType ();
  IT_ApplyCond (cc, instrState != eInstrType_ARM);

  /* DBG executes as a NOP instruction in ARMv6K and ARMv6T2.  */
  uint64_t cpuFeatures = Target_GetCPUFeatures ();
  if ((cpuFeatures & kArchExt_v7) != kArchExt_v7
      && (cpuFeatures & (kCPUExt_v6K | kCPUExt_v6T2)) != 0)
    Error (ErrorWarning, "On ARMv6K and ARMv6T2 DBG will execute as NOP");
  else
    Target_CheckCPUFeature (kArchExt_v7, true);

  if (instrState == eInstrType_ARM)
    Put_Ins (4, cc | 0x0320F0F0 | dbgValue);
  else
    {
      if (instrWidth == eInstrWidth_Enforce16bit)
	Error (ErrorError, "Narrow instruction qualifier for Thumb is not possible");
      Put_Ins (4, 0xf3af80f0 | dbgValue);
    }

  return false;
}


/**
 * Implements SMC/SMI
 *   SMC[<cc>][<q>] ["#"]<imm4>
 *   SMI[<cc>][<q>] ["#"]<imm4>
 */
static bool
core_smc_smi (bool doLowerCase, const char *instr)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  InstrWidth_e instrWidth = Option_GetInstrWidth (doLowerCase);
  if (instrWidth == eInstrWidth_Unrecognized)
    return true;

  unsigned smcValue = 0;
  Input_SkipWS ();
  if (Input_IsEolOrCommentStart ())
    Error (ErrorError, "%s value missing", instr);
  else
    {
      /* Skip optional '#'.  */
      (void) Input_Match ('#', false);
      const Value *im = Expr_BuildAndEval (ValueInt);
      switch (im->Tag)
	{
	  case ValueInt:
	    smcValue = im->Data.Int.i;
	    break;
	  default:
	    Error (ErrorError, "Unknown %s value type", instr);
	    break;
	}
      if (smcValue >= 16)
	{
	  Error (ErrorError, "%s value needs to be between 0 and 15 (incl)", instr);
	  smcValue = 0;
	}
    }

  InstrType_e instrState = State_GetInstrType ();
  IT_ApplyCond (cc, instrState != eInstrType_ARM);

  /* Note that the security extensions are optional.  */
  Target_CheckCPUFeature (kCPUExt_Sec, true);

  if (instrState == eInstrType_ARM)
    Put_Ins (4, cc | 0x01600070 | smcValue);
  else
    {
      if (instrWidth == eInstrWidth_Enforce16bit)
	Error (ErrorError, "Narrow instruction qualifier for Thumb is not possible");
      Put_Ins (4, 0xf7f08000 | (smcValue << 16));
    }

  return false;
}


/**
 * Implements SMC (UAL).
 */
bool
m_smc (bool doLowerCase)
{
  if (State_GetSyntax () == eSyntax_PreUALOnly)
    return true;
  return core_smc_smi (doLowerCase, "SMC");
}


/**
 * Implements SMI (pre-UAL).
 */
bool
m_smi (bool doLowerCase)
{
  if (State_GetSyntax () == eSyntax_UALOnly)
    return true;
  return core_smc_smi (doLowerCase, "SMI");
}