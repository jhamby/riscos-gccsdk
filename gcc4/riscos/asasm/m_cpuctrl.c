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

/** CONTROL **/

static bool
Branch_RelocUpdater (const char *fileName, unsigned lineNum, ARMWord offset,
		     const Value *valueP, void *privData, bool final)
{
  bool isBLX = *(bool *)privData;
  ARMWord ir = GetWord (offset);
  assert (valueP->Tag == ValueCode && valueP->Data.Code.len != 0);

  if (final)
    return true;
  
  int relocs = 0;
  int relative = 0;
    {
      int factor = 1;
      for (size_t i = 0; i != valueP->Data.Code.len; ++i)
	{
	  if (i + 1 != valueP->Data.Code.len
	      && valueP->Data.Code.c[i + 1].Tag == CodeOperator
	      && valueP->Data.Code.c[i + 1].Data.op == eOp_Sub)
	    factor = -1;
	  else
	    factor = 1;

	  const Code *codeP = &valueP->Data.Code.c[i];
	  if (codeP->Tag == CodeOperator)
	    {
	      if (codeP->Data.op != eOp_Add && codeP->Data.op != eOp_Sub)
		return true;
	      continue;
	    }
	  assert (codeP->Tag == CodeValue);
	  const Value *valP = &codeP->Data.value;

	  switch (valP->Tag)
	    {
	      case ValueInt:
		break;

	      case ValueSymbol:
		{
		  Value value = *valP;
		  if (Value_ResolveSymbol (&value))
		    return true;
		  assert (value.Tag == ValueSymbol);
		  if (value.Data.Symbol.symbol == areaCurrentSymbol)
		    {
		      assert ((value.Data.Symbol.symbol->type & SYMBOL_ABSOLUTE) == 0);
		      relative += factor * value.Data.Symbol.factor;
		    }
		  else
		    {
		      if (factor * value.Data.Symbol.factor < 0)
			return true;
		      relocs += factor * value.Data.Symbol.factor;
		    }
		  break;
		}

	      default:
		return true;
	    }
	}
    }
  assert (relocs >= 0);
  
  /* Branch instruction with value 0 means a branch to {PC} + 8 which is
     current area base + branch instruction offset + 8.
     So start to compensate with this value, the result is a value what needs
     to be added to that branch instruction with value 0.  */
  int branchInstrValue = -(offset + 8);
  relative -= 1;
  if (areaCurrentSymbol->area.info->type & AREA_ABS)
    {
      branchInstrValue += relative * Area_GetBaseAddress (areaCurrentSymbol);
      relative = 0;
    }
    {
      int factor = 1;
      for (size_t i = 0; i != valueP->Data.Code.len; ++i)
	{
	  if (i + 1 != valueP->Data.Code.len
	      && valueP->Data.Code.c[i + 1].Tag == CodeOperator
	      && valueP->Data.Code.c[i + 1].Data.op == eOp_Sub)
	    factor = -1;
	  else
	    factor = 1;

	  const Code *codeP = &valueP->Data.Code.c[i];
	  if (codeP->Tag == CodeOperator)
	    {
	      if (codeP->Data.op != eOp_Add && codeP->Data.op != eOp_Sub)
		return true;
	      continue;
	    }
	  assert (codeP->Tag == CodeValue);
	  const Value *valP = &codeP->Data.value;

	  switch (valP->Tag)
	    {
	      case ValueInt:
		branchInstrValue += factor * valP->Data.Int.i;
		break;

	      case ValueSymbol:
		{
		  Value value = *valP;
		  if (Value_ResolveSymbol (&value))
		    return true;
		  assert (value.Tag == ValueSymbol);
		  branchInstrValue += factor * value.Data.Symbol.offset;

		  int numRelocs;
		  if (valP->Data.Symbol.symbol == areaCurrentSymbol)
		    {
		      if (relative > 0)
			{
			  numRelocs = relative;
			  relative = 0;
			}
		      else
			numRelocs = 0; /* Nothing to be done.  */
		    }
		  else
		    {
		      numRelocs = factor * value.Data.Symbol.factor;
		      relocs -= numRelocs;
		    }
		  while (numRelocs--)
		    {
		      int how2;
		      if (relative < 0)
			{
			  ++relative;
			  how2 = HOW2_INIT | HOW2_SIZE | HOW2_RELATIVE;
			}
		      else
			how2 = HOW2_INIT | HOW2_SIZE;
		      if (Reloc_Create (how2, offset, &value) == NULL)
			return true;
		      /* The R_ARM_PC24 ELF reloc needs to happen for a "B {PC}"
			 instruction, while in AOF this needs to happen for a
			 "B -<area origin>" instruction.  */
		      if (!option_aof)
			branchInstrValue += offset;
		    }
		  break;
		}

	      default:
		assert (0);
		break;
	    }
	}
      while (relative < 0)
	{
	  ++relative;
	  const Value value = Value_Symbol (areaCurrentSymbol, 1, 0);
	  if (Reloc_Create (HOW2_INIT | HOW2_SIZE, offset, &value) == NULL)
	    return true;
	}
      assert (!relocs && !relative);
    }

  int mask = isBLX ? 1 : 3;
  if (branchInstrValue & mask)
    errorLine (fileName, lineNum, ErrorError, "Branch value is not a multiple of %s", isBLX ? "two" : "four");
  ir |= ((branchInstrValue >> 2) & 0xffffff) | (isBLX ? (branchInstrValue & 2) << 23 : 0);
  Put_InsWithOffset (offset, 4, ir);

  return false;
}

static bool
branch_shared (ARMWord cc, bool isBLX)
{
  /* At this point the current area index can be unaligned for ARM/Thumb
     instructions, upfront correct this index.  */
  const ARMWord instrAlign = State_GetInstrType () == eInstrType_ARM ? 4 : 2;
  const ARMWord offset = (areaCurrentSymbol->area.info->curIdx + instrAlign-1) & -instrAlign;

  exprBuild ();

  Put_Ins (4, cc | 0x0A000000);
  if (gPhase != ePassOne
      && Reloc_QueueExprUpdate (Branch_RelocUpdater, offset, ValueInt | ValueCode | ValueSymbol, &isBLX, sizeof (isBLX)))
    error (ErrorError, "Illegal branch expression");
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
 * \param baseReg When >= 0, emit "ADD/SUB Rx, <baseReg>, #xyz" +
 * "ADD/SUB Rx, Rx, #xyz", otherwise emit "MOV/MVN Rx, #xyz" +
 * "ORR/BIC Rx, Rx, #xyz"
 * \param fixedNumInstr When true, we can't change the number of instructions.
 * When ir & 1 is set and fixedNumInstr is false, we can ignore to emit the
 * 2nd instruction.  When it is true, we have to.
 */
static void
ADR_RelocUpdaterCore (const char *fileName, unsigned lineNum, size_t offset, int constant,
		      int baseReg, bool fixedNumInstr, bool isADRL)
{
  /* FIXME: Can clever use of ADD/SUB mixture cover more constants ? */
  struct
    {
      unsigned int try[4];
      int num;
    } split[2];
  split[0].num = Help_SplitByImm8s4 (constant, split[0].try);
  split[1].num = Help_SplitByImm8s4 (baseReg >= 0 ? -constant : ~constant, split[1].try);
  bool bestIndex, getOnes;
  if (split[0].num < split[1].num)
    {
      bestIndex = false;
      getOnes = true;
    }
  else
    {
      bestIndex = true;
      getOnes = false;
    }

  /* When baseReg is not specified and when we're in an absolute area, we can
     use PC-relative addressing as well in order to increase our options.  */
  if (baseReg < 0 && (areaCurrentSymbol->area.info->type & AREA_ABS))
    {
      const int newConstant = constant - (Area_GetBaseAddress (areaCurrentSymbol) + offset + 8);
      split[!bestIndex].num = Help_SplitByImm8s4 (-newConstant, split[!bestIndex].try);
      if (split[bestIndex].num >= split[!bestIndex].num)
	{
	  bestIndex = !bestIndex;
	  getOnes = false;
	  baseReg = 15;
	}
      split[!bestIndex].num = Help_SplitByImm8s4 (newConstant, split[!bestIndex].try);
      if (split[bestIndex].num >= split[!bestIndex].num)
	{
	  bestIndex = !bestIndex;
	  getOnes = true;
	  baseReg = 15;
	}
    }

  ARMWord irop1, irop2;
  if (getOnes)
    {
      irop1 = baseReg >= 0 ? M_ADD : M_MOV;
      irop2 = M_ADD;
    }
  else
    {
      irop1 = baseReg >= 0 ? M_SUB : M_MVN;
      irop2 = M_SUB;
    }

  if (split[bestIndex].num == 1 && isADRL)
    {
      if (fixedNumInstr)
	{
	  errorLine (fileName, lineNum, ErrorWarning, "ADRL at area offset 0x%08zx is not required for encoding 0x%08x", offset, constant);
	  split[bestIndex].try[1] = 0;
	  split[bestIndex].num = 2;
	}
      else
	errorLine (fileName, lineNum, ErrorInfo, "ADRL at area offset 0x%08zx is not required for encoding 0x%08x, using ADR instead", offset, constant);
    }
  else if ((split[bestIndex].num == 2 && !isADRL)
	   || split[bestIndex].num == 3 || split[bestIndex].num == 4)
    {
      if (fixedNumInstr)
	{
	  errorLine (fileName, lineNum, ErrorError, "%s at area offset 0x%08zx can not be used to encode 0x%08x", (isADRL) ? "ADRL" : "ADR", offset, constant);
	  split[bestIndex].num = (isADRL) ? 2 : 1;
	}
      else
	errorLine (fileName, lineNum, ErrorWarning, "%s at area offset 0x%08zx can not be used to encode 0x%08x, using %d instruction sequence instead", (isADRL) ? "ADRL" : "ADR", offset, constant, split[bestIndex].num);
    }

  ARMWord ir = GetWord (offset);
  if (split[bestIndex].num == 2 && GET_DST_OP(ir) == 15)
    errorLine (fileName, lineNum, ErrorError, "ADRL can not be used with register 15 as destination");

  for (int n = 0; n < split[bestIndex].num; ++n)
    {
      /* Fix up the base register.  */
      ARMWord irs = ir | (n == 0 ? irop1 : irop2);
      if (baseReg >= 0 || n != 0)
        irs = irs | LHS_OP (n == 0 ? (ARMWord)baseReg : GET_DST_OP(ir));

      int i8s4 = help_cpuImm8s4 (split[bestIndex].try[n]);
      assert (i8s4 != -1);
      irs |= i8s4;

      Put_InsWithOffset (offset + n*4, 4, irs);
    }
}

typedef struct
{
  ARMWord orgInstr;
  bool userIntendedTwoInstr;
} ADR_PrivData_t;

/**
 * Shared reloc updater for ADR and ADRL.
 */
static bool
ADR_RelocUpdater (const char *fileName, unsigned lineNum, ARMWord offset,
		  const Value *valueP, void *privData, bool final)
{
  const ADR_PrivData_t *privDataP = (const ADR_PrivData_t *)privData;

  Put_InsWithOffset (offset, 4, privDataP->orgInstr);

  assert (valueP->Tag == ValueCode && valueP->Data.Code.len != 0);
  for (size_t i = 0; i != valueP->Data.Code.len; ++i)
    {
      const Code *codeP = &valueP->Data.Code.c[i];
      if (codeP->Tag == CodeOperator)
	{
	  if (codeP->Data.op != eOp_Add)
	    return true;
	  continue;
	}
      assert (codeP->Tag == CodeValue);
      const Value *valP = &codeP->Data.value;

      switch (valP->Tag)
	{
	  case ValueInt:
	    /* Absolute value : results in MOV/MVN (followed by ADD/SUB in case of
	       ADRL).  */
	    ADR_RelocUpdaterCore (fileName, lineNum, offset, valP->Data.Int.i, -1, true /* final */, privDataP->userIntendedTwoInstr);
	    break;

	  case ValueAddr:
	    ADR_RelocUpdaterCore (fileName, lineNum, offset, valP->Data.Addr.i, valP->Data.Addr.r, true /* final */, privDataP->userIntendedTwoInstr);
	    break;

	  case ValueSymbol:
	    {
	      /* Wait until the last (final) round as this avoid emitting unnecessary
		relocations.  */
	      if (!final)
		{
		  if (privDataP->userIntendedTwoInstr)
		    Put_InsWithOffset (offset + 4, 4, 0);
		  return true;
		}
	      ADR_RelocUpdaterCore (fileName, lineNum, offset, -(offset + 8), 15, true /* final */, privDataP->userIntendedTwoInstr);
	      if (Reloc_Create (HOW2_INIT | HOW2_SIZE | HOW2_RELATIVE, offset, valP) == NULL)
		return true;
	    }
	    break;

	  default:
	    return true;
	}
    }

  return false;
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

  exprBuild ();

  if (gPhase == ePassOne)
    {
      Put_Ins (4, 0);
      /* When bit 0 is set, we'll emit ADRL (2 instructions).  */
      if (ir & 1)
	Put_Ins (4, 0);
      return false;
    }
  
  /* When bit 0 is set, we'll emit ADRL (2 instructions).  */
  ADR_PrivData_t privData =
    {
      .orgInstr = (ir | DST_OP (regD) | IMM_RHS) & ~1,
      .userIntendedTwoInstr = (ir & 1) != 0
    };

  /* The label will expand to either a field in a based map or a PC-relative 
     expression.  */
  if (Reloc_QueueExprUpdate (ADR_RelocUpdater,
			     areaCurrentSymbol->area.info->curIdx,
			     ValueAddr | ValueInt | ValueSymbol | ValueCode,
			     &privData, sizeof (privData)))
    error (ErrorError, "Illegal %s expression", (privData.userIntendedTwoInstr & 1) ? "ADRL" : "ADR");

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
