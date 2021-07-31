/*
 * AsAsm an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2014 GCCSDK Developers
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
#include <stdint.h>

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

typedef struct
{
  bool forwardLabel; /* Can only be true during pass 1 when we can't
    evaluate the label.  Presumebly this is a forward label.  */
  uint32_t instrOffset; /* Aligned instruction offset.  */
  Value relocValue; /* When ValueSymbol, instruction needs to be relocated
    against this Symbol. Can only be set during pass 2.  */
  int32_t branchOffset; /* Branch offset value to be encoded in the
    instruction.  */
} BranchLabel_t;

static BranchLabel_t
GetBranchLabel (bool isBLX)
{
  BranchLabel_t result =
    {
      .forwardLabel = false,
      .instrOffset = Area_CurIdxAligned (),
      .relocValue.Tag = ValueIllegal,
      .branchOffset = 0
    };

  const Value *valP = Expr_BuildAndEval (ValueInt | ValueSymbol);

  Value value;
  if (valP->Tag == ValueSymbol
      && valP->Data.Symbol.factor != 1
      && valP->Data.Symbol.symbol == areaCurrentSymbol
      && (areaCurrentSymbol->attr.area->type & AREA_ABS) != 0)
    {
      uint32_t areaOffset = Area_GetBaseAddress (areaCurrentSymbol);
      value = Value_Symbol (areaCurrentSymbol, 1, valP->Data.Int.i + (valP->Data.Symbol.factor - 1)*areaOffset);
      valP = &value;
    }

  /* Translate jump from an absolute area to an absolute address to a jump
     to a symbol address instead.
     For ELF, only when it jumps into the current aboslute area.  */
  if (valP->Tag == ValueInt
      && (areaCurrentSymbol->attr.area->type & AREA_ABS) != 0
      && (option_aof
	  || (Area_GetBaseAddress (areaCurrentSymbol) <= (uint32_t)valP->Data.Int.i
	      && (uint32_t)valP->Data.Int.i < Area_GetBaseAddress (areaCurrentSymbol) + areaCurrentSymbol->attr.area->maxIdx)))
    {
      uint32_t areaOffset = (areaCurrentSymbol->attr.area->type & AREA_ABS) != 0 ? Area_GetBaseAddress (areaCurrentSymbol) : 0;
      value = Value_Symbol (areaCurrentSymbol, 1, valP->Data.Int.i - areaOffset);
      valP = &value;
    }

  bool stateIsARM = State_GetInstrType () == eInstrType_ARM;

  /* Catch the unsupported cases.  */
  if (valP->Tag == ValueIllegal
      || (valP->Tag == ValueSymbol
	  && (valP->Data.Symbol.factor != 1 || (gPhase == ePassOne && valP->Data.Symbol.symbol != areaCurrentSymbol))))
    {
      if (gPhase != ePassOne)
	{
	  Error (ErrorError, "Illegal branch expression");
	  return result;
	}
      /* Go for "B {PC} + 4/8" until pass two.  */
      value = Value_Symbol (areaCurrentSymbol, 1, result.instrOffset + (stateIsARM ? 8 : 4));
      valP = &value;
      result.forwardLabel = true;
    }

  /* Check for reloc.  */
  if (valP->Tag == ValueInt)
    {
      if (option_aof)
	{
	  /* In AOF, we don't have absolute address relocation possibilities,
	     so make it relative to our area.  */
	  uint32_t areaOffset = (areaCurrentSymbol->attr.area->type & AREA_ABS) != 0 ? Area_GetBaseAddress (areaCurrentSymbol) : 0;
	  value = Value_Symbol (areaCurrentSymbol, 1, valP->Data.Int.i - areaOffset);
	  valP = &value;

	  result.branchOffset += areaOffset;
	}
      else
	result.branchOffset -= valP->Data.Int.i;
      Value_Assign (&result.relocValue,  valP);
    }
  else
    {
      assert (valP->Tag == ValueSymbol && valP->Data.Symbol.factor == 1);
      if (valP->Data.Symbol.symbol != areaCurrentSymbol)
	Value_Assign (&result.relocValue,  valP);
    }

  /* Calculate value to be encoded in branch instruction.  */
  if (result.relocValue.Tag != ValueIllegal)
    {
      /* When there is relocation needed, the ELF relocs R_ARM_CALL,
	 R_ARM_THM_CALL, R_ARM_JUMP24, R_ARM_THM_JUMP6, R_ARM_THM_JUMP8,
	 R_ARM_THM_JUMP11, R_ARM_THM_JUMP19, R_ARM_THM_JUMP24 need to happen for
	 a "B {PC}" instruction, while for AOF Type 2 relocations this needs to
	 happen for a "B <area origin>" instruction.  */
      result.branchOffset -= stateIsARM ? 8 : 4;
      if (option_aof)
	result.branchOffset -= result.instrOffset;
    }
  else
    {
      /* Branch instruction with value 0 means a branch to {PC} + 8 which is
	 current area base + branch instruction offset + 8.
	 So start to compensate with this value, the result is a value what needs
	 to be added to that branch instruction with value 0.  */
      result.branchOffset -= result.instrOffset + (stateIsARM ? 8 : 4);
    }
  /* Transfer specified branch offset.  */
  assert (valP->Tag == ValueInt || valP->Tag == ValueSymbol);
  int32_t branchOffset = valP->Tag == ValueInt ? valP->Data.Int.i : valP->Data.Symbol.offset;
  int32_t targetAddr = !stateIsARM && isBLX ? ((branchOffset & ~2) | (int32_t)(result.instrOffset & 2)) : branchOffset;
  result.branchOffset += targetAddr;

  unsigned mask = (stateIsARM ^ isBLX) ? 3 : 1;
  if ((branchOffset & mask) != 0)
    Error (ErrorWarning, "Branch target is not a multiple of %s", mask == 1 ? "two" : "four");

  return result;
}


/**
 * \return true when given signed value can be encoded in given range.
 */
static inline bool
RangeIsOK (int32_t value, int32_t range)
{
  return value >= -range/2 && value < range/2;
}


/**
 * Implements B and BL.
 *   B["L"][<cc>][<q>] <label>
 */
bool
m_branch (bool doLowerCase)
{
  ARMWord cc = Option_LinkCond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  bool isLink = (cc & LINK_BIT) != 0;
  cc &= ~LINK_BIT;

  InstrWidth_e instrWidth = Option_GetInstrWidth (doLowerCase);
  if (instrWidth == eInstrWidth_Unrecognized)
    return true;

  BranchLabel_t brLabel = GetBranchLabel (false);

  InstrType_e instrState = State_GetInstrType ();

  if (isLink)
    {
      IT_ApplyCond (cc, instrState != eInstrType_ARM, instrState != eInstrType_ARM);
      if (instrState == eInstrType_ARM)
	{
	  if (!RangeIsOK (brLabel.branchOffset, 1<<26))
	    Error (ErrorError, "Branch target too far away");

	  Put_Ins (4, cc | 0x0B000000 | ((brLabel.branchOffset & 0x3fffffc) >> 2));

	  /* FIXME: should emit reloc when label is function and when it marked for export.  */
	  if (brLabel.relocValue.Tag != ValueIllegal)
	    Reloc_CreateELF (R_ARM_CALL, brLabel.instrOffset, &brLabel.relocValue);
	}
      else
	{
	  if (instrWidth == eInstrWidth_Enforce16bit)
	    Error (ErrorError, "Narrow instruction qualifier for Thumb is not possible");

	  /* Do final branch range test after IT_ApplyCond().  */
	  if (!RangeIsOK (brLabel.branchOffset, 1<<25))
	    Error (ErrorError, "Branch target too far away");

	  bool j1 = ((brLabel.branchOffset & (1<<23)) != 0) == (brLabel.branchOffset < 0);
	  bool j2 = ((brLabel.branchOffset & (1<<22)) != 0) == (brLabel.branchOffset < 0);
	  Put_Ins (4, 0xF000D000
			| ((brLabel.branchOffset < 0) << 26)
			| ((brLabel.branchOffset & 0x3ff000) << 4)
			| (j1 << 13)
	  		| (j2 << 11)
	   		| ((brLabel.branchOffset & 0xfff) >> 1));

	  /* FIXME: should emit reloc when label is function and when it marked for export.  */
	  if (brLabel.relocValue.Tag != ValueIllegal)
	    Reloc_CreateELF (R_ARM_THM_CALL, brLabel.instrOffset, &brLabel.relocValue);
	}
    }
  else
    {
      if (instrState == eInstrType_ARM)
	{
	  IT_ApplyCond (cc, false, false);

	  if (!RangeIsOK (brLabel.branchOffset, 1<<26))
	    Error (ErrorError, "Branch target too far away");

	  Put_Ins (4, cc | 0x0A000000 | ((brLabel.branchOffset & 0x3fffffc) >> 2));

	  /* FIXME: should emit reloc when label is function and when it marked for export.  */
	  if (brLabel.relocValue.Tag != ValueIllegal)
	    Reloc_CreateELF (R_ARM_JUMP24, brLabel.instrOffset, &brLabel.relocValue);
	}
      else
	{
	  bool extraITNeededInCaseOfLongRange, useLongRange;
	  if (gPhase == ePassOne)
	    {
	      extraITNeededInCaseOfLongRange = cc != AL && !IT_CanExtendBlock (cc);

	      /* When <q> is not specified:
		   - Pre-v6T2 : always 16-bit Thumb
		   - v6T2 onwards : go for 16-bit Thumb when branch target
		     allows it or when we have a forward label.  */
	      if (instrWidth == eInstrWidth_NotSpecified)
		instrWidth = !Target_CheckCPUFeature (kCPUExt_v6T2, false)
			       || RangeIsOK (brLabel.branchOffset - (extraITNeededInCaseOfLongRange ? 2 : 0), 1<<12) ? eInstrWidth_Enforce16bit : eInstrWidth_Enforce32bit;

	      /* 16-bit [32-bit] Thumb branch with explicit condition code
		 encoded can only jump in a 9 [21] bit range.  Without explicit
		 condition code encoded (i.e. using an IT block) this is a
		 12 [25] bit range but we don't want to generate an extra IT
		 block if it can be avoided.  */
	      int32_t shortRange = instrWidth == eInstrWidth_Enforce16bit ? (1<<9) : (1<<21);
	      useLongRange = cc == AL || IT_CanExtendBlock (cc) || brLabel.forwardLabel || !RangeIsOK (brLabel.branchOffset, shortRange);
	    }
	  else
	    {
	      /* Reverse engineer what we've decided during pass one.  */
	      uint32_t instrOffset = brLabel.instrOffset;
	      ARMWord nextThumb = Put_GetHalfWord (instrOffset);
	      instrOffset += 2;
	      extraITNeededInCaseOfLongRange = (nextThumb & 0xFF00) == 0xBF00;
	      if (extraITNeededInCaseOfLongRange)
		{
		  nextThumb = Put_GetHalfWord (instrOffset);
		  instrOffset += 2;
		}
	      bool useWide;
	      if ((nextThumb & 0xF000) == 0xD000)
		{
		  useLongRange = false;
		  assert (!extraITNeededInCaseOfLongRange);
		  useWide = false;
		}
	      else if ((nextThumb & 0xF000) == 0xE000)
		{
		  useLongRange = true;
		  useWide = false;
		}
	      else
		{
		  assert ((nextThumb & 0xF800) == 0xF000);
		  nextThumb = Put_GetHalfWord (instrOffset);
		  /* instrOffset += 2; */
		  assert ((nextThumb & 0xC000) == 0x8000);
		  if ((nextThumb & 0x1000) != 0)
		    useLongRange = true;
		  else
		    {
		      useLongRange = false;
		      assert (!extraITNeededInCaseOfLongRange);
		    }
		  useWide = true;
		}
	      assert ((instrWidth == eInstrWidth_Enforce16bit && !useWide)
	              || (instrWidth == eInstrWidth_Enforce32bit && useWide)
	              || instrWidth == eInstrWidth_NotSpecified);
	      if (instrWidth == eInstrWidth_NotSpecified)
		instrWidth = useWide ? eInstrWidth_Enforce32bit : eInstrWidth_Enforce16bit;
	    }

	  IT_ApplyCond (useLongRange ? cc : AL, useLongRange, true);
	  if (useLongRange && extraITNeededInCaseOfLongRange)
	    brLabel.branchOffset -= 2;

	  /* Do final branch range test after IT_ApplyCond().  */
	  int32_t shortRange = instrWidth == eInstrWidth_Enforce16bit ? (1<<9) : (1<<21);
	  int32_t longRange = instrWidth == eInstrWidth_Enforce16bit ? (1<<12) : (1<<25);
	  if (!RangeIsOK (brLabel.branchOffset, useLongRange ? longRange : shortRange))
	    Error (ErrorError, "Branch target too far away");

	  Target_CheckCPUFeature (instrWidth == eInstrWidth_Enforce16bit ? kCPUExt_v4T : kCPUExt_v6T2, true);

	  if (instrWidth == eInstrWidth_Enforce16bit)
	    {
	      Put_Ins (2, useLongRange ? 0xE000u | ((brLabel.branchOffset & 0xffe) >> 1)
				       : 0xD000u | (cc >> 20) | ((brLabel.branchOffset & 0x1fe) >> 1));

	      /* FIXME: should emit reloc when label is function and when it marked for export.  */
	      if (brLabel.relocValue.Tag != ValueIllegal)
		Reloc_CreateELF (useLongRange ? R_ARM_THM_JUMP11 : R_ARM_THM_JUMP8,
				 brLabel.instrOffset, &brLabel.relocValue);
	    }
	  else
	    {
	      bool j1 = ((brLabel.branchOffset & (1<<23)) != 0) == (brLabel.branchOffset < 0);
	      bool j2 = ((brLabel.branchOffset & (1<<22)) != 0) == (brLabel.branchOffset < 0);
	      Put_Ins (4, useLongRange ? 0xF0009000
					   | ((brLabel.branchOffset < 0) << 26)
					   | ((brLabel.branchOffset & 0x3ff000) << 4)
					   | (j1 << 13)
					   | (j2 << 11)
					   | ((brLabel.branchOffset & 0x000fff) >> 1)
				       : 0xF0008000
					   | ((brLabel.branchOffset < 0) << 26)
					   | (cc >> 6)
					   | ((brLabel.branchOffset & 0x3f000) << 4)
					   | ((brLabel.branchOffset & (1<<18)) >> (18-13))
					   | ((brLabel.branchOffset & (1<<19)) >> (19-11))
					   | ((brLabel.branchOffset & 0x000fff) >> 1));

	      /* Note, R_ARM_THM_JUMP19 is misleading as it is relocating
		 20 bits.  */
	      /* FIXME: should emit reloc when label is function and when it marked for export.  */
	      if (brLabel.relocValue.Tag != ValueIllegal)
		Reloc_CreateELF (useLongRange ? R_ARM_THM_JUMP24 : R_ARM_THM_JUMP19,
				 brLabel.instrOffset, &brLabel.relocValue);
	    }
	}
    }

  if (brLabel.relocValue.Tag != ValueIllegal)
    Reloc_CreateAOF (HOW2_INIT | HOW2_INSTR_UNLIM | HOW2_RELATIVE,
		     brLabel.instrOffset, &brLabel.relocValue);

  return false;
}


/**
 * Implements CBNZ/CBZ.
 *   CB["N"]Z <Rn>, <label>
 */
bool
m_cbnz_cbz (bool doLowerCase)
{
  bool isCBNZ = Input_Match (doLowerCase ? 'n' : 'N', false);
  if (!Input_Match (doLowerCase ? 'z' : 'Z', false))
    return true;

  InstrWidth_e instrWidth = Option_GetInstrWidth (doLowerCase);
  if (instrWidth == eInstrWidth_Unrecognized)
    return true;

  unsigned rn = Get_CPUReg ();
  if (rn >= 8)
    Error (ErrorError, "CB%sZ can only encode registers R0 ... R7", isCBNZ ? "N" : "");

  Input_SkipWS ();
  if (!Input_Match (',', false))
    Error (ErrorError, "%sdst", InsertCommaAfter);

  BranchLabel_t brLabel = GetBranchLabel (false);

  InstrType_e instrState = State_GetInstrType ();
  IT_ApplyCond (AL, instrState != eInstrType_ARM, instrState != eInstrType_ARM);

  if (instrState == eInstrType_ARM)
    Error (ErrorError, "CB%sZ is not an ARM instruction", isCBNZ ? "N" : "");
  else
    {
      if (instrWidth == eInstrWidth_Enforce32bit)
	Error (ErrorError, "Wide instruction qualifier for Thumb is not possible");
      if (brLabel.branchOffset < 0)
	Error (ErrorError, "CB%sZ can only branch forward from {PC} + 4 onwards", isCBNZ ? "N" : "");
      else if (brLabel.branchOffset >= 128)
	Error (ErrorError, "Branch target too far away");

      Target_CheckCPUFeature (kCPUExt_v6T2, true);

      Put_Ins (2, 0xB100
		    | (isCBNZ << 11)
		    | ((brLabel.branchOffset & 0x40) << (9 - 6))
		    | ((brLabel.branchOffset & 0x3e) << (3 - 1))
		    | rn);

      /* FIXME: should emit reloc when label is function and when it marked for export.  */
      if (brLabel.relocValue.Tag != ValueIllegal)
	{
	  Reloc_CreateAOF (HOW2_INIT | HOW2_INSTR_UNLIM | HOW2_RELATIVE, brLabel.instrOffset,
	                   &brLabel.relocValue);
	  Reloc_CreateELF (R_ARM_THM_JUMP6,
	                   brLabel.instrOffset, &brLabel.relocValue);
	}
    }

  return false;
}


/**
 * Implements BLX.
 *   BLX[<cc>][<q>] <Rm>
 *   BLX[<cc>][<q>] <label>
 */
bool
m_blx (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  InstrWidth_e instrWidth = Option_GetInstrWidth (doLowerCase);
  if (instrWidth == eInstrWidth_Unrecognized)
    return true;

  ARMWord reg = Get_CPURegNoError ();
  if (reg == INVALID_REG)
    {
      /* BLX[<cc>][<q>] <label> */
      BranchLabel_t brLabel = GetBranchLabel (true);

      InstrType_e instrState = State_GetInstrType ();
      IT_ApplyCond (cc, instrState != eInstrType_ARM, instrState != eInstrType_ARM);

      bool j1 = ((brLabel.branchOffset & (1<<23)) != 0) == (brLabel.branchOffset < 0);
      bool j2 = ((brLabel.branchOffset & (1<<22)) != 0) == (brLabel.branchOffset < 0);
      Target_CheckCPUFeature (instrState == eInstrType_ARM || (j1 && j2) ? kCPUExt_v5T : kCPUExt_v6T2, true);

      /* Do final branch range test after IT_ApplyCond().  */
      if (!RangeIsOK (brLabel.branchOffset, instrState == eInstrType_ARM ? (1<<26) : (1<<25)))
	Error (ErrorError, "Branch target too far away");

      if (instrState == eInstrType_ARM)
	{
	  if (cc != AL)
	    Error (ErrorError, "BLX <label> must be unconditional");
	  Put_Ins (4, 0xFA000000
	   		| ((brLabel.branchOffset >> 2) & 0xffffff)
	   		| ((brLabel.branchOffset & 2) << 23));
	}
      else
	{
	  if (instrWidth == eInstrWidth_Enforce16bit)
	    Error (ErrorError, "Narrow instruction qualifier for Thumb is not possible");
	  Put_Ins (4, 0xF000C000
			| ((brLabel.branchOffset < 0) << 26)
			| ((brLabel.branchOffset & 0x3ff000) << 4)
			| (j1 << 13)
			| (j2 << 11)
			| (brLabel.branchOffset & 0xffc) >> 1);
	}
      /* FIXME: should emit reloc when label is function and when it marked for export.  */
      if (brLabel.relocValue.Tag != ValueIllegal)
	{
	  Reloc_CreateAOF (HOW2_INIT | HOW2_INSTR_UNLIM | HOW2_RELATIVE, brLabel.instrOffset,
			   &brLabel.relocValue);
	  Reloc_CreateELF (instrState == eInstrType_ARM ? R_ARM_CALL : R_ARM_THM_CALL,
			   brLabel.instrOffset, &brLabel.relocValue);
	}
    }
  else
    {
      /* BLX[<cc>][<q>] <Rm> */
      InstrType_e instrState = State_GetInstrType ();
      IT_ApplyCond (cc, instrState != eInstrType_ARM, instrState != eInstrType_ARM);

      Target_CheckCPUFeature (kCPUExt_v5T, true);

      if (reg == 15)
	Error (ErrorWarning, "Use of PC with BLX is UNPREDICTABLE");

      if (instrState == eInstrType_ARM)
	Put_Ins (4, cc | 0x012FFF30 | RHS_OP (reg));
      else
	{
	  if (instrWidth == eInstrWidth_Enforce32bit)
	    Error (ErrorError, "Wide instruction qualifier for Thumb is not possible");
	  Put_Ins (2, 0x4780 | (reg << 3));
	}
    }

  return false;
}


/**
 * Implements BX.
 *   BX[<cc>][<q>] <Rm>
 */
bool
m_bx (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  InstrWidth_e instrWidth = Option_GetInstrWidth (doLowerCase);
  if (instrWidth == eInstrWidth_Unrecognized)
    return true;

  unsigned dst = Get_CPUReg ();
  if (dst == 15)
    Error (ErrorWarning, "Use of PC with BX is DEPRECATED");

  InstrType_e instrState = State_GetInstrType ();
  IT_ApplyCond (cc, instrState != eInstrType_ARM, instrState != eInstrType_ARM);

  Target_CheckCPUFeature (kCPUExt_v4T, true);

  if (instrState == eInstrType_ARM)
    {
      /* Output R_ARM_V4BX with nul symbol so that linker can create ARMv4
         compatible images by changing this BX instruction into MOV PC, Rx.
         Only for up to ARMv4T + ARMv5, from ARMv5T onwards we don't do this.  */
      if (!Target_CheckCPUFeature (kCPUExt_v5T, false))
	Reloc_CreateELF (R_ARM_V4BX, areaCurrentSymbol->attr.area->curIdx, NULL);

      Put_Ins (4, cc | 0x012fff10 | dst);
    }
  else
    {
      if (instrWidth == eInstrWidth_Enforce32bit)
	Error (ErrorError, "Wide instruction qualifier for Thumb is not possible");
      Put_Ins (2, 0x4700 | (dst << 3));
    }

  return false;
}


/**
 * Implements BXJ.
 *   BXJ[<cc>][<q>] <Rm>
 */
bool
m_bxj (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  InstrWidth_e instrWidth = Option_GetInstrWidth (doLowerCase);
  if (instrWidth == eInstrWidth_Unrecognized)
    return true;

  unsigned dst = Get_CPUReg ();

  InstrType_e instrState = State_GetInstrType ();
  IT_ApplyCond (cc, instrState != eInstrType_ARM, instrState != eInstrType_ARM);

  if (instrState == eInstrType_ARM)
    {
      Target_CheckCPUFeature (kCPUExt_v5J, true);

      if (dst == 15)
	Error (ErrorWarning, "Use of PC with BXJ is UNPREDICTABLE");

      Put_Ins (4, cc | 0x012fff20 | dst);
    }
  else
    {
      if (instrWidth == eInstrWidth_Enforce16bit)
	Error (ErrorError, "Narrow instruction qualifier for Thumb is not possible");

      Target_CheckCPUFeature (kCPUExt_v6T2, true);

      if (dst == 13 || dst == 15)
	Error (ErrorWarning, "Use of R13 or PC with BXJ is UNPREDICTABLE");
      Put_Ins (4, 0xf3c08f00 | (dst << 16));
    }

  return false;
}


/**
 * Implements SVC / SWI.
 *   SVC/SWI[<cc>][<q>] ["#"]<24 bit int>
 *   SVC/SWI[<cc>][<q>] <string>
 */
bool
m_swi (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  InstrWidth_e instrWidth = Option_GetInstrWidth (doLowerCase);
  if (instrWidth == eInstrWidth_Unrecognized)
    return true;

  ARMWord swiNum = 0;
  Input_SkipWS ();
  ValueTag valueTags = Input_Match ('#', false) ? ValueInt : (ValueInt | ValueString);
  const Value *im = Expr_BuildAndEval (valueTags);
  switch (im->Tag)
    {
      case ValueInt:
	swiNum = im->Data.Int.i;
	break;

      case ValueString:
	{
#ifdef __riscos__
	  /* ValueString is not NUL terminated.  */
	  char swiname[im->Data.String.len + 1];
	  memcpy (swiname, im->Data.String.s, im->Data.String.len);
	  swiname[im->Data.String.len] = '\0';
	  swiNum = OS_SWINameToNum (swiname);
	  if (swiNum == 0xFFFFFFFF)
	    {
	      Error (ErrorError, "Unknown SVC/SWI name");
	      swiNum = 0;
	    }
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

  InstrType_e instrState = State_GetInstrType ();
  IT_ApplyCond (cc, false, instrState != eInstrType_ARM);

  ARMWord maxSwiNum = instrState == eInstrType_ARM ? (1<<24) : (1<<8);
  if (swiNum >= maxSwiNum)
    Error (ErrorError, "SVC/SWI number %d (0x%08x) to high (max is 0x%08x)",
           swiNum, swiNum, maxSwiNum - 1);
  if (instrState == eInstrType_ARM)
    Put_Ins (4, cc | 0x0F000000 | swiNum);
  else
    {
      Target_CheckCPUFeature (kCPUExt_v4T, true);
      if (instrWidth == eInstrWidth_Enforce32bit)
	Error (ErrorError, "Wide instruction qualifier for Thumb is not possible");
      Put_Ins (2, 0xDF00 | swiNum);
    }

  return false;
}


/**
 * Implements BKPT.
 *   BKPT[<cc>][<q>] ["#"]<imm>
 *
 * Note <cc> should be AL or "" otherwise it's unpredictable.
 */
bool
m_bkpt (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  InstrWidth_e instrWidth = Option_GetInstrWidth (doLowerCase);
  if (instrWidth == eInstrWidth_Unrecognized)
    return true;

  Input_SkipWS ();
  (void) Input_Match ('#', false);
  const Value *im = Expr_BuildAndEval (ValueInt);
  int i;
  if (im->Tag != ValueInt)
    {
      Error (ErrorError, "Illegal BKPT expression");
      i = 0;
    }
  else
    i = im->Data.Int.i;

  InstrType_e instrState = State_GetInstrType ();
  IT_ApplyCond (cc, false, instrState != eInstrType_ARM);

  if (cc != AL)
    Error (ErrorWarning, "For BKPT, using condition code different than AL is UNPREDICTABLE");

  Target_CheckCPUFeature (instrState == eInstrType_ARM ? kCPUExt_v5 : kCPUExt_v5T, true);
  if (instrState == eInstrType_ARM)
    {
      ARMWord val = Fix_Int (2, i);
      Put_Ins (4, cc | 0xE1200070 | ((val & 0xFFF0) << 4) | (val & 0xF));
    }
  else
    {
      if (instrWidth == eInstrWidth_Enforce32bit)
	Error (ErrorError, "Wide instruction qualifier for Thumb is not possible");
      ARMWord val = Fix_Int (1, i);
      Put_Ins (2, 0xBE00 | val);
    }

  return false;
}


typedef enum
{
  eADRInstrType_ALU	= 0,
  eADRInstrType_MOVW	= 1,
  eADRInstrType_MOVT	= 2,
  eADRInstrType_NOP	= 3
} ADRInstrType_e;

typedef struct
{
  ADRInstrType_e instrType[2]; /*< See ADRInstrType_e values.  */
  uint8_t numInstr; /*< Number instructions generated. */
} ADRResult_t;

typedef enum
{
  eCS_Yes	= 0, /* We can ignore isADRL and freely switch between
    ADR and ADRL.  */
  eCS_NoBecauseForwardLabel = 1, /* There is a forward label preventing
    us to make a smarter choice between ADR and ADRL than what the user
    wrote.  */
  eCS_NoBecauseReloc = 2 /* We're going to output a reloc for the ADR/ADRL.  */
} CanSwitch_e;

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
 * \param canSwitch See its enum definition.
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
static ADRResult_t
ADR_RelocUpdaterCore (uint32_t constant, int baseReg, uint32_t baseInstr,
		      bool isADRL, CanSwitch_e canSwitch)
{
  uint32_t offset = Area_CurIdxAligned ();
  uint32_t areaOffset = (areaCurrentSymbol->attr.area->type & AREA_ABS) ? Area_GetBaseAddress (areaCurrentSymbol) : 0;

  /* When we don't have a base register, use PC as base register but only
     for an ABS area.  */
  bool baseRegUnspecified = baseReg < 0;
  if (baseRegUnspecified
      && (areaCurrentSymbol->attr.area->type & AREA_ABS) != 0)
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
      || (baseReg == 15 && (areaCurrentSymbol->attr.area->type & AREA_ABS) != 0))
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
          || (baseReg == 15 && (areaCurrentSymbol->attr.area->type & AREA_ABS) != 0)))
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
      if ((areaCurrentSymbol->attr.area->type & AREA_ABS) != 0)
	Error (ErrorError, "%s at area offset 0x%x with base address 0x%x can not be used to encode %s",
	       (isADRL) ? "ADRL" : "ADR", offset, areaOffset, toEncode);
      else
	Error (ErrorError, "%s at area offset 0x%x can not be used to encode %s",
	       (isADRL) ? "ADRL" : "ADR", offset, toEncode);
      bestScore = isADRL ? 2 : 1;
    }

  if (bestScore == 1 && isADRL)
    {
      switch (canSwitch)
	{
	  case eCS_Yes:
	    if (areaCurrentSymbol->attr.area->type & AREA_ABS)
	      Error (ErrorWarning, "Using ADR instead of ADRL at area offset 0x%x with base address 0x%x to encode %s",
		     offset, areaOffset, toEncode);
	    else
	      Error (ErrorWarning, "Using ADR instead of ADRL at area offset 0x%x to encode %s",
		     offset, toEncode);
	    break;

	  case eCS_NoBecauseForwardLabel:
	    /* The second instruction of ADRL is going to be a NOP.  */
	    if (areaCurrentSymbol->attr.area->type & AREA_ABS)
	      Error (ErrorWarning, "ADR instead of ADRL can be used at area offset 0x%x with base address 0x%x to encode %s",
		     offset, areaOffset, toEncode);
	    else
	      Error (ErrorWarning, "ADR instead of ADRL can be used at area offset 0x%x to encode %s",
		     offset, toEncode);
	    break;

	  case eCS_NoBecauseReloc:
	    /* No warning.  */
	    break;
	}
    }
  else if (bestScore == 2 && !isADRL && canSwitch == eCS_Yes)
    {
      /* We switch from ADR to ADRL because there is no other option.  */
      if (areaCurrentSymbol->attr.area->type & AREA_ABS)
	Error (ErrorWarning, "Using ADRL instead of ADR at area offset 0x%x with base address 0x%x to encode %s",
	       offset, areaOffset, toEncode);
      else
	Error (ErrorWarning, "Using ADRL instead of ADR at area offset 0x%x to encode %s",
	       offset, toEncode);
    }

  assert (bestScore == 1 || bestScore == 2);

  if (bestScore == 2 && GET_DST_OP(baseInstr) == 15)
    Error (ErrorError, "ADRL can not be used with register 15 as destination");

  if (bestScore == 1  && isADRL && canSwitch == eCS_NoBecauseReloc)
    {
      bestScore = 2;
      split[bestIndex].try[1] = 0;
    }

  ADRResult_t result = { .numInstr = bestScore };
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
	  result.instrType[n] = eADRInstrType_ALU;
	}
    }
  else
    {
      assert (bestIndex == 4 || bestIndex == 5);
      uint32_t cc = baseInstr & NV;
      uint32_t destReg = GET_DST_OP(baseInstr);
      Put_Ins_MOVW_MOVT (cc, destReg, split[bestIndex].try[0], false);
      result.instrType[0] = eADRInstrType_MOVW;
      if (bestScore == 2)
	{
	  assert (split[bestIndex].try[1] != 0);
	  Put_Ins_MOVW_MOVT (cc, destReg, split[bestIndex].try[1], true);
	  result.instrType[0] = eADRInstrType_MOVT;
	}
    }
  if (bestScore == 1  && isADRL && canSwitch == eCS_NoBecauseForwardLabel)
    {
      /* We need to output two instructions.  Use NOP (its value depends
         on architecture support v6K and v6T2).  */
      Put_Ins (4, Target_CheckCPUFeature (kCPUExt_v6K, false)
                    || Target_CheckCPUFeature (kCPUExt_v6T2, false) ? 0xe320F000 : 0xe1a00000);
      result.instrType[1] = eADRInstrType_NOP;
      result.numInstr = 2;
    }

  return result;
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

  const Value *valP = Expr_BuildAndEval (ValueInt | ValueAddr | ValueSymbol);

  const uint32_t instrOffset = Area_CurIdxAligned ();

  const RelocAndAddend_t relocAddend = Reloc_SplitRelocAndAddend (valP,
								  areaCurrentSymbol,
								  instrOffset,
								  true);

  if (gPhase == ePassOne
      && (valP->Tag == ValueIllegal || relocAddend.relocSymbol.Tag == ValueSymbol))
    {
      /* Two cases:
	   - We have a non evaluatable expression.  Wait until pass two to do
	     the work.  This means that when ADRL is used, we can't go back
	     to ADR anymore.
	   - We have a forward label (not yet seen) or relocation symbol.
	     Equally here, no switch from ADRL to ADR possible.  */
      Put_Ins (4, 0);
      if (isADRL)
	Put_Ins (4, 0);
      return false;
    }
  if (valP->Tag == ValueIllegal
      || (valP->Tag == ValueInt && valP->Data.Int.type != eIntType_PureInt))
    {
      Error (ErrorError, "Illegal %s expression", isADRL ? "ADRL" : "ADR");
      return false;
    }

  valP = &relocAddend.addend;

  /* During PassOne, we're at liberty to change ADR into ADRL and/or ADRL
     into ADR when we have a valid addend and there is no relocation symbol
     (at this point, it is not even sure that a relocation symbol at PassOne
     will be a relocation symbol at PassTwo).  */
  CanSwitch_e canSwitch = gPhase == ePassOne || Put_GetWord (instrOffset) != 0 ? eCS_Yes : eCS_NoBecauseForwardLabel;
  if (relocAddend.relocSymbol.Tag == ValueSymbol)
    canSwitch = eCS_NoBecauseReloc;

  int constant, baseReg;
  switch (valP->Tag)
    {
      case ValueInt:
	/* Absolute value : results in MOV/MVN (followed by ADD/SUB in
	   case of ADRL) or MOVW or MOV32.  */
	constant = valP->Data.Int.i;
	baseReg = -1;
	break;

      case ValueAddr:
	/* Storage map, pc-relative, AREA + BASED.  */
	constant = valP->Data.Addr.i;
	baseReg = valP->Data.Addr.r;
	break;

      default:
	Error (ErrorError, "Illegal %s expression", isADRL ? "ADRL" : "ADR");
	constant = 8; /* FIXME: Thumb support missing.  */
	baseReg = 15;
	break;
    }
  const ADRResult_t result = ADR_RelocUpdaterCore (constant,
						   baseReg, ir | DST_OP (regD) | IMM_RHS,
						   isADRL, canSwitch);
  if (relocAddend.relocSymbol.Tag == ValueSymbol)
    {
      bool isPCrel = baseReg == 15;
      for (unsigned i = 0; i != result.numInstr; ++i)
	{
	  if (result.instrType[i] == eADRInstrType_NOP)
	    continue;

	  uint32_t elfHow;
	  switch (result.instrType[i])
	    {
	      case eADRInstrType_ALU:
		{
		  if (i == 0)
		    {
		      if (i + 1 == result.numInstr)
			elfHow = isPCrel ? R_ARM_ALU_PC_G0 : R_ARM_ALU_SB_G0;
		      else
			elfHow = isPCrel ? R_ARM_ALU_PC_G0_NC : R_ARM_ALU_SB_G0_NC;
		    }
		  else
		    elfHow = isPCrel ? R_ARM_ALU_PC_G1 : R_ARM_ALU_SB_G1;
		  break;
		}

	      case eADRInstrType_MOVW:
		/* If result.numInstr is 1, we should be using R_ARM_MOVW_PREL
		 and R_ARM_MOVW_ABS but that doesn't seem to exist ?! */
		elfHow = isPCrel ? R_ARM_MOVW_PREL_NC : R_ARM_MOVW_ABS_NC;
		break;

	      case eADRInstrType_MOVT:
		elfHow = isPCrel ? R_ARM_MOVT_PREL : R_ARM_MOVT_ABS;
		break;

	      case eADRInstrType_NOP:
		/* Already filtered upfront.  */
		assert (0);
		break;
	    }
	  Reloc_CreateELF (elfHow, instrOffset + 4*i, &relocAddend.relocSymbol);
	}
      uint32_t aofHow = HOW2_INIT | HOW2_INSTR_UNLIM | (isPCrel ? HOW2_RELATIVE : HOW2_BASED);
      Reloc_CreateAOF (aofHow, instrOffset, &relocAddend.relocSymbol);
    }

  return false;
}


static const struct
{
  const char *bankedRegP;
  uint8_t r;
  uint8_t sys;
} oBankedReg[] =
{
  { "r8_usr", 0, 0 },
  { "r9_usr", 0, 1 },
  { "r10_usr", 0, 2 },
  { "r11_usr", 0, 3 },
  { "r12_usr", 0, 4 },
  { "sp_usr", 0, 5 },
  { "lr_usr", 0, 6 },
  { "r8_fiq", 0, 8 + 0 },
  { "r9_fiq", 0, 8 + 1 },
  { "r10_fiq", 0, 8 + 2 },
  { "r11_fiq", 0, 8 + 3 },
  { "r12_fiq", 0, 8 + 4 },
  { "sp_fiq", 0, 8 + 5 },
  { "lr_fiq", 0, 8 + 6 },
  { "lr_irq", 0, 16 + 0 },
  { "sp_irq", 0, 16 + 1 },
  { "lr_svc", 0, 16 + 2 },
  { "sp_svc", 0, 16 + 3 },
  { "lr_abt", 0, 16 + 4 },
  { "sp_abt", 0, 16 + 5 },
  { "lr_und", 0, 16 + 6 },
  { "sp_und", 0, 16 + 7 },
  { "lr_mon", 0, 24 + 4 },
  { "sp_mon", 0, 24 + 5 },
  { "elr_hyp", 0, 24 + 6 },
  { "sp_hyp", 0, 24 + 7 },
  { "spsr_fiq", 1, 8 + 6 },
  { "spsr_irq", 1, 16 + 0 },
  { "spsr_svc", 1, 16 + 2 },
  { "spsr_abt", 1, 16 + 4 },
  { "spsr_und", 1, 16 + 6 },
  { "spsr_mon", 1, 24 + 4 },
  { "spsr_hyp", 1, 24 + 6 },
};

/**
 * \return Index in oBankedReg for given banked register name.  Is SIZE_MAX
 * when not recognized as banked register.
 */
static size_t
GetBankedReg (void)
{
  for (size_t idx = 0; idx != sizeof (oBankedReg)/sizeof (oBankedReg[0]); ++idx)
    {
      if (Input_MatchStringLower (oBankedReg[idx].bankedRegP))
	return idx;
    }
  return SIZE_MAX;
}

typedef struct
{
  size_t bankedReg;
  bool isSPSR; /* Only when bankedReg == SIZE_MAX.  */
  uint8_t mask; /* Only when bankedReg == SIZE_MAX and writeFields was true.  */
} Parse_PSR_t;

/**
 * \param writeFields When true, it's MSR so need to parse bits/fields in
 * APSR_<bits>, CPSR_<fields> and SPSR_<fields>.
 */
static Parse_PSR_t
Parse_PSR (bool writeFields)
{
  Input_SkipWS ();

  Parse_PSR_t result = { .bankedReg = GetBankedReg () };
  if (result.bankedReg != SIZE_MAX)
    return result;
  /* Read "APSR", "CPSR" or "SPSR".  */
  enum { isAPSR, isCPSR, isSPSR } keyword;
  if (Input_MatchStringLower ("apsr"))
    keyword = isAPSR;
  else if (Input_MatchStringLower ("cpsr"))
    keyword = isCPSR;
  else if (Input_MatchStringLower ("spsr"))
    keyword = isSPSR;
  else
    {
      Error (ErrorError, "Unrecognized PSR field");
      return result;
    }
  result.isSPSR = keyword == isSPSR;

  if (keyword == isCPSR || keyword == isSPSR)
    {
      enum { isNone, isAll, isFlg, isCtl, isCXSF } type;
      const char *reqFieldSpecP;
      if (Input_IsEndOfKeyword () || Input_Look () == ',')
	{
	  /* "CPSR" or "SPSR"
	     writeFields true: deprecated, use "CPSR_cf"/"SPSR_cf" instead.  */
	  reqFieldSpecP = writeFields ? "cf" : NULL;
	  type = isNone;
	  result.mask = 0x9;
	}
      else if (Input_MatchString ("_all"))
	{
	  /* "CPSR_all" or "SPSR_all".
	     writeFields false: deprecated, use "CPSR"/"SPSR" instead
	     writeFields true: deprecated, use "CPSR_cf"/"SPSR_cf" instead.  */
	  reqFieldSpecP = writeFields ? "cf" : "";
	  type = isAll;
	  result.mask = 0x9;
	}
      else if (Input_MatchString ("_flg"))
	{
	  /* "CPSR_flg" or "SPSR_flg".
	     writeFields false: illegal
	     writeFields true: deprecated, use "APSR_nzcvq"/"SPSR_f" instead.  */
	  reqFieldSpecP = writeFields ? "f" : NULL;
	  type = isFlg;
	  result.mask = 0x8;
	}
      else if (Input_MatchString ("_ctl"))
	{
	  /* "CPSR_ctl" or "SPSR_ctl".
	     writeFields false: illegal
	     writeFields true: deprecated, use "CPSR_c"/"SPSR_c" instead.  */
	  reqFieldSpecP = writeFields ? "c" : NULL;
	  type = isCtl;
	  result.mask = 0x1;
	}
      else
	{
	  /* "{C|S}PSR_{c}{x}{s}{f}.
	     writeFields false: illegal
	     writeFields true & CPSR case, when possible it's recommended to
	     use APSR_{nzcvq}{g} instead.  */
	  reqFieldSpecP = NULL;
	  type = isCXSF;
	  result.mask = 0;
	  bool ok = Input_Match ('_', false);
	  while (ok)
	    {
	      uint8_t maskPrev = result.mask;
	      if (Input_Match ('c', false))
		result.mask |= 0x1;
	      else if (Input_Match ('x', false))
		result.mask |= 0x2;
	      else if (Input_Match ('s', false))
		result.mask |= 0x4;
	      else if (Input_Match ('f', false))
		result.mask |= 0x8;
	      else
		{
		  ok = Input_IsEndOfKeyword () || Input_Look () == ',';
		  break;
		}
	      if (maskPrev == result.mask)
		Error (ErrorWarning, "PSR field specifier bit has already been specified");
	    }
	  if (!ok)
	    {
	      Error (ErrorError, "Unrecognized PSR field specifier");
	      return result;
	    }
	  if (writeFields && result.mask == 0)
	    Error (ErrorWarning, "Empty PSR field specifier is UNPREDICTABLE");
	}
      if (writeFields
          && keyword == isCPSR
          && (result.mask & 0xC0) != 0 && (result.mask & ~0xC0) == 0)
	Error (ErrorWarning, "Recommended to use APSR_%s%s instead",
	       (result.mask & 0x8) ? "nzcvq" : "",
	       (result.mask & 0x4) ? "g" : "");
      else if (!writeFields && (type != isNone && type != isAll))
	{
	  assert (type == isFlg || type == isCtl || type == isCXSF);
	  const char *s = type == isFlg ? "flg" : type == isCtl ? "ctl" : "cxsf";
	  Error (ErrorError, "PSR field specifier _%s type can not be used for MRS", s);
	}
      else if (reqFieldSpecP)
	Error (ErrorWarning, "PSR field specifier is OBSOLETE, use %s_%s instead",
	       keyword == isCPSR ? "CPSR" : "SPSR", reqFieldSpecP);
    }
  else
    {
      assert (keyword == isAPSR);
      /* Process "APSR_<bits> with <bits> = one of 'nzcvq', 'g' or 'nzcvqg'.  */
      result.mask = 0;
      if (Input_Match ('_', false))
	{
	  if (Input_MatchStringLower ("nzcvq"))
	    result.mask |= 0x8;
	  if (Input_Match ('g', false))
	    result.mask |= 0x4;
	  if (result.mask == 0 || !(Input_IsEndOfKeyword () || Input_Look () == ','))
	    {
	      Error (ErrorError, "Expected one of 'nzcvq', 'g' or 'nzcvqg'");
	      return result;
	    }
	}
      else if (!Input_IsEndOfKeyword () && !Input_Look () == ',')
	{
	  Error (ErrorError, "Unrecognized PSR field specifier");
	  return result;
	}
      if (!writeFields && result.mask != 0)
	Error (ErrorError, "PSR field specifier _%s%s can not be used for MRS",
	       (result.mask & 0x8) ? "nzcvq" : "", (result.mask & 0x4) ? "g" : "");
    }
  
  return result;
}


/**
 * Implements MSR.
 *   MSR[<c>][<q>] <banked_reg>, <Rn>
 *   MSR[<c>][<q>] <spec_reg>, #const
 *   MSR[<c>][<q>] <spec_reg>, <Rn>
 */
bool
m_msr (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  InstrWidth_e instrWidth = Option_GetInstrWidth (doLowerCase);
  if (instrWidth == eInstrWidth_Unrecognized)
    return true;

  const Parse_PSR_t psr = Parse_PSR (true);

  bool isReg = false;
  ARMWord params = 0;
  Input_SkipWS ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%slhs", InsertCommaAfter);
  else if (Input_Match ('#', false))
    {
      const Value *im = Expr_BuildAndEval (ValueInt);
      if (im->Tag == ValueInt)
	params = Fix_Imm8s4 (cc, im->Data.Int.i); /* FIXME: no rotator support (see getRhs()) ? */
      else
	Error (ErrorError, "Illegal immediate expression");
      if (psr.bankedReg != SIZE_MAX && (psr.mask & 0x6) != 0)
	Error (ErrorWarning, "Writing immediate value to status or extension field of CPSR/SPSR is inadvisable");
    }
  else
    {
      params = Get_CPUReg ();
      isReg = true;
    }

  InstrType_e instrState = State_GetInstrType ();
  IT_ApplyCond (cc, false, instrState != eInstrType_ARM);

  if (psr.bankedReg != SIZE_MAX && !isReg)
    {
      Error (ErrorError, "MSR with banked register can only have a register as source");
      params = 0;
      isReg = true;
    }
  if (instrState != eInstrType_ARM && !isReg)
    {
      Error (ErrorError, "MSR with constant can't be encoded in Thumb");
      params = 0;
      isReg = true;
    }

  Target_CheckCPUFeature (psr.bankedReg != SIZE_MAX ? kCPUExt_Virt : instrState == eInstrType_ARM ? kCPUExt_v3 : kCPUExt_v6T2, true);
  if (instrState == eInstrType_ARM)
    {
      if (isReg)
	{
	  if (psr.bankedReg != SIZE_MAX && (params == 13 || params == 15))
	    Error (ErrorWarning, "The use of R13 or PC as Rn is UNPREDICTABLE");
	  if (psr.bankedReg == SIZE_MAX && params == 15)
	    Error (ErrorWarning, "The use of PC as Rn is UNPREDICTABLE");
	}
      Put_Ins (4, cc | (psr.bankedReg != SIZE_MAX ? 0x0120F200u
						      | (oBankedReg[psr.bankedReg].r << 22)
						      | ((oBankedReg[psr.bankedReg].sys & 0xF)<<16)
						      | ((oBankedReg[psr.bankedReg].sys & 0x10)<<(8-4))
						      | params
                                                  : 0x0120F000u
						      | (!isReg << 25)
						      | (psr.isSPSR << 22)
						      | (psr.mask << 16)
						      | params));
    }
  else
    {
      if (instrWidth == eInstrWidth_Enforce16bit)
        Error (ErrorError, "Narrow instruction qualifier for Thumb is not possible");
      assert (isReg);
      if (params == 13 || params == 15)
	Error (ErrorWarning, "The use of R13 or PC as Rn is UNPREDICTABLE");
      Put_Ins (4, psr.bankedReg != SIZE_MAX ? 0xF3808020
						| (oBankedReg[psr.bankedReg].r << 20)
						| (params << 16)
						| ((oBankedReg[psr.bankedReg].sys & 0xF) << 8)
						| ((oBankedReg[psr.bankedReg].sys & 0x10) << (4-4))
					    : 0xF3808000
						| (psr.isSPSR << 20)
						| (params << 16)
						| (psr.mask << 8));
    }

  return false;
}


/**
 * Implements MRS.
 *   MRS[<c>][<q>] <Rd>, <banked_reg> 
 *   MRS[<c>][<q>] <Rn>, <spec_reg>
 */
bool
m_mrs (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  InstrWidth_e instrWidth = Option_GetInstrWidth (doLowerCase);
  if (instrWidth == eInstrWidth_Unrecognized)
    return true;

  ARMWord params = Get_CPUReg ();

  Target_CheckCPUFeature (kCPUExt_v3, true);
  Input_SkipWS ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%slhs", InsertCommaAfter);

  const Parse_PSR_t psr = Parse_PSR (false);

  InstrType_e instrState = State_GetInstrType ();
  IT_ApplyCond (cc, false, instrState != eInstrType_ARM);

  Target_CheckCPUFeature (psr.bankedReg != SIZE_MAX ? kCPUExt_Virt : instrState == eInstrType_ARM ? kCPUExt_v3 : kCPUExt_v6T2, true);
  if (instrState == eInstrType_ARM)
    {
      if (params == 15)
	Error (ErrorWarning, "The use of R13 or PC as Rn is UNPREDICTABLE");
      Put_Ins (4, cc | (psr.bankedReg != SIZE_MAX ? 0x01000200
						      | (oBankedReg[psr.bankedReg].r << 22)
						      | ((oBankedReg[psr.bankedReg].sys & 0xF) << 16)
						      | (params << 12)
						      | ((oBankedReg[psr.bankedReg].sys & 0x10) << (8-4))
						  : 0x010F0000
						      | (psr.isSPSR << 22)
						      | (params << 12)));
    }
  else
    {
      if (instrWidth == eInstrWidth_Enforce16bit)
	Error (ErrorError, "Narrow instruction qualifier for Thumb is not possible");
      if (params == 13 || params == 15)
	Error (ErrorWarning, "The use of PC as Rn is UNPREDICTABLE");
      Put_Ins (4, cc | (psr.bankedReg != SIZE_MAX ? 0xF3E08020
						      | (oBankedReg[psr.bankedReg].r << 20)
						      | ((oBankedReg[psr.bankedReg].sys & 0xF) << 16)
						      | (params << 8)
						      | ((oBankedReg[psr.bankedReg].sys & 0x10) << (4-4))
						  : 0xF3EF8000
						      | (psr.isSPSR << 20)
						      | (params << 8)));
    }

  return false;
}


/**
 * Implements SEV.
 *   SEV[<cc>][<q>]
 */
bool
m_sev (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  InstrWidth_e instrWidth = Option_GetInstrWidth (doLowerCase);
  if (instrWidth == eInstrWidth_Unrecognized)
    return true;

  InstrType_e instrState = State_GetInstrType ();
  IT_ApplyCond (cc, false, instrState != eInstrType_ARM);

  if (instrState != eInstrType_ARM && instrWidth == eInstrWidth_NotSpecified)
    instrWidth = eInstrWidth_Enforce16bit;

  /* SEV executes as a NOP instruction in ARMv6T2.  */
  uint64_t cpuFeatures = Target_GetCPUFeatures ();
  if ((cpuFeatures & kArchExt_v7) != kArchExt_v7
      && (cpuFeatures & kCPUExt_v6T2) != 0)
    Error (ErrorWarning, "On ARMv6T2 %s will execute as NOP", "SEV");
  else
    Target_CheckCPUFeature (instrState == eInstrType_ARM ? kArchExt_v6K : kArchExt_v7, true);

  if (instrState == eInstrType_ARM)
    Put_Ins (4, cc | 0x0320F004);
  else if (instrWidth == eInstrWidth_Enforce16bit)
    Put_Ins (2, 0xBF40);
  else
    Put_Ins (4, 0xF3AF8004);

  return false;
}


/**
 * Implements WFE.
 *   WFE[<cc>][<q>]
 */
bool
m_wfe (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  InstrWidth_e instrWidth = Option_GetInstrWidth (doLowerCase);
  if (instrWidth == eInstrWidth_Unrecognized)
    return true;

  InstrType_e instrState = State_GetInstrType ();
  IT_ApplyCond (cc, false, instrState != eInstrType_ARM);

  if (instrState != eInstrType_ARM && instrWidth == eInstrWidth_NotSpecified)
    instrWidth = eInstrWidth_Enforce16bit;

  /* WFE executes as a NOP instruction in ARMv6T2.  */
  uint64_t cpuFeatures = Target_GetCPUFeatures ();
  if ((cpuFeatures & kArchExt_v7) != kArchExt_v7
      && (cpuFeatures & kCPUExt_v6T2) != 0)
    Error (ErrorWarning, "On ARMv6T2 %s will execute as NOP", "WFE");
  else
    Target_CheckCPUFeature (instrState == eInstrType_ARM ? kArchExt_v6K : kArchExt_v7, true);

  if (instrState == eInstrType_ARM)
    Put_Ins (4, cc | 0x0320F002);
  else if (instrWidth == eInstrWidth_Enforce16bit)
    Put_Ins (2, 0xBF20);
  else
    Put_Ins (4, 0xF3AF8002);

  return false;
}


/**
 * Implements WFI.
 *   WFI[<cc>][<q>]
 */
bool
m_wfi (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  InstrWidth_e instrWidth = Option_GetInstrWidth (doLowerCase);
  if (instrWidth == eInstrWidth_Unrecognized)
    return true;

  InstrType_e instrState = State_GetInstrType ();
  IT_ApplyCond (cc, false, instrState != eInstrType_ARM);

  if (instrState != eInstrType_ARM && instrWidth == eInstrWidth_NotSpecified)
    instrWidth = eInstrWidth_Enforce16bit;

  /* WFI executes as a NOP instruction in ARMv6T2.  */
  uint64_t cpuFeatures = Target_GetCPUFeatures ();
  if ((cpuFeatures & kArchExt_v7) != kArchExt_v7
      && (cpuFeatures & kCPUExt_v6T2) != 0)
    Error (ErrorWarning, "On ARMv6T2 %s will execute as NOP", "WFI");
  else
    Target_CheckCPUFeature (instrState == eInstrType_ARM ? kArchExt_v6K : kArchExt_v7, true);

  if (instrState == eInstrType_ARM)
    Put_Ins (4, cc | 0x0320F003);
  else if (instrWidth == eInstrWidth_Enforce16bit)
    Put_Ins (2, 0xBF30);
  else
    Put_Ins (4, 0xF3AF8003);

  return false;
}


/**
 * Implements YIELD.
 *   YIELD[<cc>][<q>]
 */
bool
m_yield (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  InstrWidth_e instrWidth = Option_GetInstrWidth (doLowerCase);
  if (instrWidth == eInstrWidth_Unrecognized)
    return true;

  InstrType_e instrState = State_GetInstrType ();
  IT_ApplyCond (cc, false, instrState != eInstrType_ARM);

  if (instrState != eInstrType_ARM && instrWidth == eInstrWidth_NotSpecified)
    instrWidth = eInstrWidth_Enforce16bit;

  /* YIELD executes as a NOP instruction in ARMv6T2.  */
  uint64_t cpuFeatures = Target_GetCPUFeatures ();
  if ((cpuFeatures & kArchExt_v7) != kArchExt_v7
      && (cpuFeatures & kCPUExt_v6T2) != 0)
    Error (ErrorWarning, "On ARMv6T2 %s will execute as NOP", "YIELD");
  else
    Target_CheckCPUFeature (instrState == eInstrType_ARM ? kArchExt_v6K : kArchExt_v7, true);

  if (instrState == eInstrType_ARM)
    Put_Ins (4, cc | 0x0320F001);
  else if (instrWidth == eInstrWidth_Enforce16bit)
    Put_Ins (2, 0xBF10);
  else
    Put_Ins (4, 0xF3AF8001);

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
      readMode = false;
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
  IT_ApplyCond (AL, false, instrState != eInstrType_ARM);

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
  IT_ApplyCond (cc, false, instrState != eInstrType_ARM);

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
  IT_ApplyCond (cc, true, instrState != eInstrType_ARM);

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
