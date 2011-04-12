/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2011 GCCSDK Developers
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
#include "put.h"
#include "targetcpu.h"
#include "value.h"

/** CONTROL **/

static bool
Branch_RelocUpdater (const char *file, int lineno, ARMWord offset,
		     const Value *valueP, void *privData, bool final)
{
  bool isBLX = *(bool *)privData;
  ARMWord ir = GetWord (offset);
  assert (valueP->Tag == ValueCode && valueP->Data.Code.len != 0);

  int extraOffset = 0;
  if (final)
    {
      bool unresolvedSym = false, areaSym = false;
      for (size_t i = 0; i != valueP->Data.Code.len; ++i)
	{
	  const Code *codeP = &valueP->Data.Code.c[i];
	  if (codeP->Tag == CodeOperator)
	    {
	      if (codeP->Data.op != Op_add)
		return true;
	      continue;
	    }
	  assert (codeP->Tag == CodeValue);
	  const Value *valP = &codeP->Data.value;

	  if (valP->Tag == ValueSymbol)
	    {
	      if (valP->Data.Symbol.symbol == areaCurrentSymbol)
		{
		  if (valP->Data.Symbol.factor != -1)
		    return true; /* No way we can encode this.  */
		  areaSym =  true;
		}
	      else
		unresolvedSym = true;
	    }
	}
      if (unresolvedSym != areaSym)
	return true; /* No way we can encode this.  */
      /* The R_ARM_PC24 ELF reloc needs to happen for a "B + PC - 8"
         instruction, while in AOF this needs to happen for a "B 0".  */
      if (!option_aof && unresolvedSym)
	extraOffset = offset;
    }
  
  for (size_t i = 0; i != valueP->Data.Code.len; ++i)
    {
      const Code *codeP = &valueP->Data.Code.c[i];
      if (codeP->Tag == CodeOperator)
	{
	  if (codeP->Data.op != Op_add)
	    return true;
	  continue;
	}
      assert (codeP->Tag == CodeValue);
      const Value *valP = &codeP->Data.value;

      switch (valP->Tag)
	{
	  case ValueInt:
	    {
	      int intVal = valP->Data.Int.i + extraOffset;
	      int mask = isBLX ? 1 : 3;
	      if (intVal & mask)
		errorLine (file, lineno, ErrorError, "Branch value is not a multiple of %s", isBLX ? "two" : "four");
	      ir |= ((intVal >> 2) & 0xffffff) | (isBLX ? (intVal & 2) << 23 : 0);
	      Put_InsWithOffset (offset, ir);
	    }
	    break;

	  case ValueSymbol:
	    if (!final)
	      return true;
	    if (valP->Data.Symbol.symbol != areaCurrentSymbol
	        && Reloc_Create (HOW2_INIT | HOW2_SIZE | HOW2_RELATIVE, offset, valP) == NULL)
	      return true;
	    break;

	  default:
	    return true;
	}
    }

  return false;
}

static bool
branch_shared (ARMWord cc, bool isBLX)
{
  const ARMWord offset = areaCurrentSymbol->value.Data.Int.i;

  exprBuild ();
  /* The branch instruction has its offset as relative, while the given label
     is absolute, so calculate "<label> - . - 8".  */
  codePosition (areaCurrentSymbol, offset);
  codeOperator (Op_sub);
  codeInt (8);
  codeOperator (Op_sub);
  
  Put_Ins (cc | 0x0A000000);
  if (Reloc_QueueExprUpdate (Branch_RelocUpdater, offset, ValueInt | ValueCode | ValueSymbol, &isBLX, sizeof (isBLX)))
    error (ErrorError, "Illegal branch expression");
  return false;
}

/**
 * Implements B and BL.
 */
bool
m_branch (void)
{
  ARMWord cc = optionLinkCond ();
  if (cc == optionError)
    return true;
  return branch_shared (cc, false);
}

/**
 * Implements BLX.
 */
bool
m_blx (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
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
    Put_Ins (cc | 0x012FFF30 | RHS_OP (reg)); /* BLX <Rm> */

  return false;
}

/**
 * Implements BX.
 */
bool
m_bx (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;

  Target_NeedAtLeastArch (ARCH_ARMv5TE);

  int dst = getCpuReg ();
  if (dst == 15)
    error (ErrorWarning, "Use of PC with BX is discouraged");

  Put_Ins (cc | 0x012fff10 | dst);
  return false;
}

/**
 * Implements BXJ.
 */
bool
m_bxj (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;

  Target_NeedAtLeastArch (ARCH_ARMv5TEJ);

  int dst = getCpuReg ();
  if (dst == 15)
    error (ErrorWarning, "Use of PC with BXJ is discouraged");

  Put_Ins (cc | 0x012fff20 | dst);
  return false;
}

/**
 * Implements SVC / SWI.
 *   SVC/SWI #<24 bit int>
 *   SVC/SWI <24 bit int>
 *   SVC/SWI <string>
 */
bool
m_swi (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;

  skipblanks ();
  ValueTag valueOK = Input_Match ('#', false) ? ValueInt : ValueInt | ValueString;
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
	error (ErrorError, "Illegal SVC/SWI expression");
	break;
    }
  Put_Ins (ir);
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
  Put_Ins (ir);
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
ADR_RelocUpdaterCore (const char *file, int lineno, size_t offset, int constant,
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
      const int newConstant = constant - (areaCurrentSymbol->area.info->baseAddr + offset + 8);
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

  uint32_t offsetToReport = offset + areaCurrentSymbol->area.info->baseAddr;
  if (split[bestIndex].num == 1 && isADRL)
    {
      if (fixedNumInstr)
	{
	  errorLine (file, lineno, ErrorWarning, "ADRL at area offset 0x%08x is not required for addressing 0x%08x", offsetToReport, constant);
	  split[bestIndex].try[1] = 0;
	  split[bestIndex].num = 2;
	}
      else
	errorLine (file, lineno, ErrorInfo, "ADRL at area offset 0x%08x is not required for addressing 0x%08x, using ADR instead", offsetToReport, constant);
    }
  else if ((split[bestIndex].num == 2 && !isADRL)
	   || split[bestIndex].num == 3 || split[bestIndex].num == 4)
    {
      if (fixedNumInstr)
	{
	  errorLine (file, lineno, ErrorError, "%s at area offset 0x%08x can not address 0x%08x", (isADRL) ? "ADRL" : "ADR", offsetToReport, constant);
	  split[bestIndex].num = (isADRL) ? 2 : 1;
	}
      else
	errorLine (file, lineno, ErrorWarning, "%s at area offset 0x%08x can not address 0x%08x, using %d instruction sequence instead", (isADRL) ? "ADRL" : "ADR", offsetToReport, constant, split[bestIndex].num);
    }

  ARMWord ir = GetWord (offset);
  if (split[bestIndex].num == 2 && GET_DST_OP(ir) == 15)
    errorLine (file, lineno, ErrorError, "ADRL can not be used with register 15 as destination");

  for (int n = 0; n < split[bestIndex].num; ++n)
    {
      /* Fix up the base register.  */
      ARMWord irs = ir | (n == 0 ? irop1 : irop2);
      if (baseReg >= 0 || n != 0)
        irs = irs | LHS_OP (n == 0 ? baseReg : GET_DST_OP(ir));

      int i8s4 = help_cpuImm8s4 (split[bestIndex].try[n]);
      assert (i8s4 != -1);
      irs |= i8s4;

      Put_InsWithOffset (offset + n*4, irs);
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
ADR_RelocUpdater (const char *file, int lineno, ARMWord offset,
		  const Value *valueP, void *privData, bool final)
{
  const ADR_PrivData_t *privDataP = (const ADR_PrivData_t *)privData;

  Put_InsWithOffset (offset, privDataP->orgInstr);

  assert (valueP->Tag == ValueCode && valueP->Data.Code.len != 0);
  for (size_t i = 0; i != valueP->Data.Code.len; ++i)
    {
      const Code *codeP = &valueP->Data.Code.c[i];
      if (codeP->Tag == CodeOperator)
	{
	  if (codeP->Data.op != Op_add)
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
	    ADR_RelocUpdaterCore (file, lineno, offset, valP->Data.Int.i, -1, final, privDataP->userIntendedTwoInstr);
	    break;

	  case ValueAddr:
	    ADR_RelocUpdaterCore (file, lineno, offset, valP->Data.Addr.i, valP->Data.Addr.r, final, privDataP->userIntendedTwoInstr);
	    break;

	  case ValueSymbol:
	    {
	      /* Wait until the last (final) round as this avoid emitting unnecessary
		relocations.  */
	      if (!final)
		{
		  if (privDataP->userIntendedTwoInstr)
		    Put_InsWithOffset (offset + 4, 0);
		  return true;
		}
	      ADR_RelocUpdaterCore (file, lineno, offset, -(offset + 8), 15, final, privDataP->userIntendedTwoInstr);
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
m_adr (void)
{
  ARMWord ir = optionAdrL ();
  if (ir == optionError)
    return true;

  /* When bit 0 is set, we'll emit ADRL (2 instructions).  */
  ADR_PrivData_t privData =
    {
      .orgInstr = (ir | DST_OP (getCpuReg ()) | IMM_RHS) & ~1,
      .userIntendedTwoInstr = (ir & 1) != 0
    };

  skipblanks ();
  if (!Input_Match (',', false))
    error (ErrorError, "%sdst", InsertCommaAfter);

  /* The label will expand to either a field in a based map or a PC-relative 
     expression.  */
  exprBuild ();
  if (Reloc_QueueExprUpdate (ADR_RelocUpdater,
			     areaCurrentSymbol->value.Data.Int.i,
			     ValueAddr | ValueInt | ValueSymbol | ValueCode,
			     &privData, sizeof (privData)))
    error (ErrorError, "Illegal %s expression", (privData.userIntendedTwoInstr & 1) ? "ADRL" : "ADR");

  return false;
}


/* [0] Stack args      0 = no, 1..4 = a1-an; -1 = RET or TAIL will cause error
 * [1] Stack reg vars  0 = no, 1..6 = v1-vn
 * [2] Stack fp vars   0 = no, 1..4 = f4-f(3+n)
 */
static signed int regs[3] = {-1, -1, -1};

/**
 * Implements STACK : APCS prologue
 */
bool
m_stack (void)
{
  static const unsigned int lim[3] = {4, 6, 4};
  static const ARMWord
    arg_regs[]  = {0x00000000, 0xE92D0001, 0xE92D0003, 0xE92D0007, 0xE92D000F},
    push_inst[] = {0xE92DD800, 0xE92DD810, 0xE92DD830, 0xE92DD870,
		   0xE92DD8F0, 0xE92DD9F0, 0xE92DDBF0},
    pfp_inst[]  = {0x00000000, 0xED2DC203, 0xED6D4206, 0xED6DC209, 0xED2D420C};

  regs[2] = regs[1] = regs[0] = -1;
  skipblanks ();
  if (inputLook ())
    {
      int reg = 0;
      char c;
      do
	{
	  skipblanks ();
	  switch (c = toupper (inputGet ()))
	    {
	    case 'A':
	      reg = 0;
	      break;
	    case 'V':
	      reg = 1;
	      break;
	    case 'F':
	      reg = 2;
	      break;
	    default:
	      error (ErrorError, "Illegal register class %c", c);
	      break;
	    }
	  if (regs[reg] != -1)
	    error (ErrorError, "Register class %c duplicated", c);
	  if (Input_Match ('=', false))
	    {
	      const Value *im = exprBuildAndEval (ValueInt);
	      int i;
	      if (im->Tag != ValueInt)
		{
		  error (ErrorError, "No number of registers specified");
		  i = 0;
		}
	      else
		i = im->Data.Int.i;
	      if ((unsigned)i  > lim[reg])
		{
		  error (ErrorError, "Too many registers to stack for class %c", c);
		  i = 0;
		}
	      regs[reg] = i;
	    }
	  else
	    regs[reg] = (signed) lim[reg];
	  skipblanks ();
	  c = inputLook ();
	  if (c == ',')
	    inputSkip ();
	  else if (c)
	    error (ErrorError, "%sregister class %c", InsertCommaAfter, c);
	}
      while (c);
      if (c)
	inputUnGet (c);
    }
  Put_Ins (0xE1A0C00D);
  if (regs[0] < 0)
    regs[0] = 0;
  if (regs[0])
    Put_Ins (arg_regs[regs[0]]);
  if (regs[1] == -1)
    regs[1] = 0;
  Put_Ins (push_inst[regs[1]]);
  if (regs[2] > 0)
    Put_Ins (pfp_inst[regs[2]]);
  Put_Ins (0xE24CB004 + 4 * regs[0]);
  return false;
}


/** APCS epilogue **/

static void
apcsEpi (ARMWord cc, const int *pop_inst, const char *op)
{
  static const ARMWord pfp_inst[] =
    {
      0x00000000,
      0x0CBDC203,
      0x0CFD4206,
      0x0CFDC209,
      0x0CBD420C
    };

  if (regs[0] == -1)
    error (ErrorError, "Cannot assemble %s without an earlier STACK", op);

  if (regs[2] > 0)
    Put_Ins (pfp_inst[regs[2]] | cc);
  Put_Ins (pop_inst[regs[1]] | cc);
}

/**
 * Implements RET : APCS epilogue - return
 * ObjAsm extension.
 */
bool
m_ret (void)
{
  static const int pop_inst[] =
    {
      0x095BA800,
      0x095BA810,
      0x095BA830,
      0x095BA870,
      0x095BA8F0,
      0x095BA9F0,
      0x095BABF0
    };

  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;

  apcsEpi (cc, pop_inst, "RET");
  return false;
}

/**
 * Implements TAIL : APCS epilogue - tail call
 */
bool
m_tail (void)
{
  static const int pop_inst[] =
    {
      0x091B6800,
      0x091B6810,
      0x091B6830,
      0x091B6870,
      0x091B68F0,
      0x091B69F0,
      0x091B6BF0
    };

  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;

  apcsEpi (cc, pop_inst, "TAIL");
  skipblanks ();
  if (inputLook ())
    branch_shared (cc, false);
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
m_msr (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;

  Target_NeedAtLeastArch (ARCH_ARMv4);

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
	  cc |= fixImm8s4 (0, cc, im->Data.Int.i);
	}
      else
	error (ErrorError, "Illegal immediate expression");
      if (cc & ((1<<17) | (1<<18)))
	error (ErrorWarning, "Writing immediate value to status or extension field of CPSR/SPSR is inadvisable");
    }
  else
    cc |= getCpuReg ();
  Put_Ins (cc);
  return false;
}

/**
 * Implements MRS.
 */
bool
m_mrs (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;

  Target_NeedAtLeastArch (ARCH_ARMv4);

  cc |= getCpuReg () << 12 | 0x01000000;
  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%slhs", InsertCommaAfter);
  cc |= getpsr (true);
  Put_Ins (cc);
  return false;
}


/**
 * Implements SEV.
 *   SEV<cond>
 */
bool
m_sev (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;

  if (Target_GetArch() != ARCH_ARMv6K)
    Target_NeedAtLeastArch (ARCH_ARMv7);
  
  Put_Ins (cc | 0x0320F004);
  return false;
}


/**
 * Implements WFE.
 *   WFE<cond>
 */
bool
m_wfe (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;

  if (Target_GetArch() != ARCH_ARMv6K)
    Target_NeedAtLeastArch (ARCH_ARMv7);
  
  Put_Ins (cc | 0x0320F002);
  return false;
}


/**
 * Implements WFI.
 *   WFI<cond>
 */
bool
m_wfi (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;

  if (Target_GetArch() != ARCH_ARMv6K)
    Target_NeedAtLeastArch (ARCH_ARMv7);
  
  Put_Ins (cc | 0x0320F003);
  return false;
}


/**
 * Implements YIELD.
 *   YIELD<cond>
 */
bool
m_yield (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;

  Put_Ins (cc | 0x0320F001);
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
m_cps (void)
{
  int imod;
  if (isspace ((unsigned char)inputLookN (0)))
    imod = 0<<18;
  else if (Input_MatchKeyword ("ID"))
    imod = 3<<18;
  else if (Input_MatchKeyword ("IE"))
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
  Put_Ins ((0xF << 28) | (1<<24) | imod | (readMode ? (1<<17) : 0) | iflags | mode);
  return false;
}

/**
 * Implements DBG.
 *   DBG<cond> #<option>
 */
bool
m_dbg (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
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
		Put_Ins (cc | 0x0320F0F0 | optValue);
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
m_smc (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
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
		Put_Ins (cc | 0x01600070 | smcValue);
	      break;
	    }
	  default:
	    error (ErrorError, "Unknown SMC value type");
	    break;
	}
    }

  return false;
}
