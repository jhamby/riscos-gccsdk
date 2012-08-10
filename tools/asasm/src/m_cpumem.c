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
 * m_cpumem.c
 */

#include "config.h"

#include <assert.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif
#include <strings.h>

#include "area.h"
#include "code.h"
#include "common.h"
#include "error.h"
#include "expr.h"
#include "expr.h"
#include "fix.h"
#include "get.h"
#include "global.h"
#include "help_cpu.h"
#include "input.h"
#include "lit.h"
#include "main.h"
#include "m_cpu.h"
#include "m_cpumem.h"
#include "option.h"
#include "phase.h"
#include "put.h"
#include "state.h"
#include "targetcpu.h"

/**
 * Parses Address mode 2 and 3.
 *
 * Similar to help_copAddr() @ help_cop.c.
 */
static bool
dstmem (ARMWord ir, const char *mnemonic)
{
  bool isAddrMode3;
  if ((ir & 0x04000090) == 0x90)
    {
      Target_CheckCPUFeature (kCPUExt_v4, true);
      isAddrMode3 = true;
    }
  else
    isAddrMode3 = false;
  ARMWord dst = getCpuReg ();
  ir |= DST_OP (dst);
  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%sdst", InsertCommaAfter);

  const bool doubleReg = (ir & (L_FLAG | S_FLAG)) == S_FLAG;
  const bool translate = (ir & W_FLAG) != 0; /* We have "T" specified in our mnemonic.  */

  if (doubleReg)
    {
      if (dst & 1)
	error (ErrorWarning, "Uneven first transfer register is UNPREDICTABLE");
      /* Try to parse "<reg>, " and check this register is dst + 1.
         If we can't parse this, assume pre-UAL syntax and move on.  */
      ARMWord dstPlusOne = Get_CPURegNoError ();
      if (dstPlusOne != INVALID_REG)
	{
	  if (dstPlusOne != dst + 1)
	    error (ErrorError, "Second transfer register is not %d but %d", dst + 1, dstPlusOne);
	  skipblanks ();
	  if (!Input_Match (',', true))
	    error (ErrorError, "%sdst", InsertCommaAfter);
	}
    }
  
  const ARMWord offset = areaCurrentSymbol->area.info->curIdx;
  Value value;
  const Value *valP = NULL;
  bool movOptAllowed = false; /* true when MOV/MVN/MOVW optimisation is allowed.  */
  switch (inputLook ())
    {
      case '[':	/* <reg>, [ */
	{
	  inputSkip ();
	  const ARMWord baseReg = getCpuReg (); /* Base register */
	  skipblanks ();
	  bool pre = !Input_Match (']', true);
	  bool forcePreIndex;
	  if (Input_Match (',', true))
	    {
	      /* Either [base,XX] (pre = true) or [base],XX (pre = false).  */
	      if (Input_Match ('#', false))
		{
		  if (isAddrMode3)
		    ir |= B_FLAG;  /* Immediate mode for addr. mode 3.  */
		  
		  const Value *valueP = exprBuildAndEval (ValueInt);
		  if (gPhase != ePassOne)
		    {
		      switch (valueP->Tag)
			{
			  case ValueInt:
			    value = Value_Addr (baseReg, valueP->Data.Int.i);
			    valP = &value;
			    break;
			  default:
			    error (ErrorError, "Illegal offset expression");
			    break;
			}
		    }
		  else
		    {
		      value = Value_Addr (baseReg, 0);
		      valP = &value;
		    }
		}
	      else
		{
		  ir |= LHS_OP (baseReg);
		  if (!Input_Match ('-', true))
		    {
		      Input_Match ('+', true);
		      ir |= U_FLAG;
		    }
		  if (Input_Match ('#', false))
		    {
		      /* We're about to call getRhs() and that supports "#<immediate>"
			 which isn't possible for our case here.  Hence, check
			 on this.  */
		      error (ErrorError, "Unknown register definition in offset field");
		    }
		  ir |= isAddrMode3 ? 0 : REG_FLAG;
		  ir = getRhs (false, !isAddrMode3, ir);
		}
	      forcePreIndex = false;
	    }
	  else
	    {
	      /* [base] */
	      ir |= LHS_OP (baseReg);
	      ir |= U_FLAG; /* 0 nicer than -0.  */
	      if (isAddrMode3)
		ir |= B_FLAG; /* Immediate mode for addr. mode 3.  */
	      if (pre)
		error (ErrorError, "Illegal character");
	      /* Pre-index nicer than post-index but don't this when 'T' is
	         specified as pre-index is not supported (FIXME: ARM only).  */
	      forcePreIndex = !translate;
	    }

	  if (pre)
	    {
	      if (!Input_Match (']', true))
		error (ErrorError, "Inserting missing ] after address");
	    }
	  else
	    {
	      if (forcePreIndex)
	        pre = true;
	    }
	  if (Input_Match ('!', true))
	    {
	      if (pre)
	        ir |= W_FLAG;
	      else
	        error (ErrorError, "Writeback not allowed with post-index");
	    }
	  if (pre)
	    {
	      /* Pre-index.  */
	      ir |= P_FLAG;
	      if (translate)
	        error (ErrorError, "Translate not allowed with pre-index");
	    }
	  else
	    {
	      /* Post-index.  */
	      if (dst == baseReg)
	        error (ErrorError, "Post increment is not sane where base and destination register are the same");
	    }
	  break;
	}

      case '=': /* <reg>, =<expression> */
	{
	  inputSkip ();
	  /* Is LDRD ? */
	  if ((ir & 0x0E1000D0) == 0xD0)
	    errorAbort ("No support for LDRD and literal");
	  /* This is only allowed for LDR.  */
	  else if ((ir & L_FLAG) == 0)
	    errorAbort ("You can't store into a constant");

          if (translate)
	    error (ErrorError, "Translate not allowed with literal");
	  
	  /* This is always pre-increment.  */
	  ir |= P_FLAG;

	  Lit_eSize litSize;
	  if ((ir & 0x0C500000) == 0x04100000)
	    litSize = eLitIntWord;
	  else if ((ir & 0x0C500000) == 0x04500000)
	    litSize = eLitIntUByte; /* LDRB, LDRBT */
	  else if ((ir & 0x0E1000F0) == 0x001000B0)
	    litSize = eLitIntUHalfWord; /* LDRH */
	  else if ((ir & 0x0E1000F0) == 0x001000D0)
	    litSize = eLitIntSByte; /* LDRSB */
	  else if ((ir & 0x0E1000F0) == 0x001000F0)
	    litSize = eLitIntSHalfWord; /* LDRSH */
	  else
	    assert (0);

	  /* The ValueInt | ValueSymbol | ValueCode tags are what we support
	     as constants from user point of view.  */
	  const Value *literalP = exprBuildAndEval (ValueInt | ValueSymbol | ValueCode);
	  if (literalP->Tag == ValueIllegal)
	    error (ErrorError, "Wrong literal type");
	  else
	    {
	      value = Lit_RegisterInt (literalP, litSize);
	      valP = &value;
	      movOptAllowed = true;
	    }
	  break;
	}

      default: /* <reg>, <label>  */
	{
	  /* We're dealing with one of the following:
	   *
	   * 1) a PC-relative label
	   * 2) a field in a register-based map
	   * 3) a label in a register-based area
	   */

	  if (translate)
	    error (ErrorError, "Translate not allowed with label");

	  /* Whatever happens, this must be a pre-increment.  */
	  ir |= P_FLAG;

	  valP = exprBuildAndEval (ValueInt | ValueAddr | ValueSymbol);
	  break;
	}
    }

  if (gPhase != ePassOne && valP != NULL)
    {
      /* ValueInt is special.  When movOptAllowed is true, we have a
         literal which guaranteed can fit in MOV/MVN/MOVW.
         If movOptAllowed is false, then we have someone using a direct
         int value which can only make sense in an absolute AREA so we
         translate this back into [pc, #...] construction.  */
      if (valP->Tag == ValueInt)
	{
	  if (movOptAllowed)
	    {
	      ir = (ir & NV) | DST_OP (GET_DST_OP (ir));
	      uint32_t im;
	      if ((im = Help_CPUImm8s4 (valP->Data.Int.i)) != UINT32_MAX)
		ir |= M_MOV | IMM_RHS | im; /* Optimize to MOV.  */
	      else if ((im = Help_CPUImm8s4 (~valP->Data.Int.i)) != UINT32_MAX)
		ir |= M_MVN | IMM_RHS | im; /* Optimize to MVN.  */
	      else if (CPUMem_ConstantInMOVW (valP->Data.Int.i))
		ir |= 0x03000000 | ((valP->Data.Int.i & 0xF000) << 4) | (valP->Data.Int.i & 0x0FFF); /* Optimize to MOVW.  */
	      else
		assert (0);
	      valP = NULL;
	    }
	  else if ((areaCurrentSymbol->area.info->type & AREA_ABS) != 0)
	    {
	      /* FIXME: MOV/MVN/MOVW would be an option too.  */
	      value = Value_Addr (15, valP->Data.Int.i - (Area_GetBaseAddress (areaCurrentSymbol) + offset + 8));
	      valP = &value;
	    }
	  else
	    {
	      /* Don't do anything here, we will automatically give an error
	         further on.  */
	    }
	}

      if (valP != NULL)
	{
	  switch (valP->Tag)
	    {
	      case ValueSymbol:
		{
		  if (valP->Data.Symbol.symbol != areaCurrentSymbol)
		    {
		      assert ((ir & P_FLAG) && "Calling reloc for non pre-increment instructions ?");
		      if (Reloc_Create (HOW2_INIT | HOW2_SIZE | HOW2_RELATIVE, offset, valP) == NULL)
			error (ErrorError, "Relocation failed");
		      break;
		    }
		  value = Value_Addr (15, valP->Data.Symbol.offset - (offset + 8));
		  valP = &value;
		  /* Fall through.  */
		}

	      case ValueAddr:
		{
		  ir |= LHS_OP (valP->Data.Addr.r);
		  if (isAddrMode3)
		    ir |= B_FLAG;
		  ir = Fix_CPUOffset (NULL, 0, ir, valP->Data.Addr.i);
		  break;
		}

	      default:
		error (ErrorError, "Illegal %s expression", mnemonic);
		break;
	    }
	}
    }
    
  Put_Ins (4, ir);

  return false;
}
	     

/**
 * Implements LDR:
 * Pre-UAL:
 *   LDR[<cond>] <Rd>, <address mode 2> | <pc relative label>
 *   LDR[<cond>]T <Rd>, <address mode 2> | <pc relative label>
 *   LDR[<cond>]B <Rd>, <address mode 2> | <pc relative label>
 *   LDR[<cond>]BT <Rd>, <address mode 2> | <pc relative label>
 *   LDR[<cond>]D <Rd>, <address mode 3> | <pc relative label>
 *   LDR[<cond>]H <Rd>, <address mode 3> | <pc relative label>
 *   LDR[<cond>]SB <Rd>, <address mode 3> | <pc relative label>
 *   LDR[<cond>]SH <Rd>, <address mode 3> | <pc relative label>
 * UAL:
 *   LDR[<cond>] <Rd>, <address mode 2> | <pc relative label>
 *   LDRT[<cond>] <Rd>, <address mode 2> | <pc relative label>
 *   LDRB[<cond>] <Rd>, <address mode 2> | <pc relative label>
 *   LDRBT[<cond>] <Rd>, <address mode 2> | <pc relative label>
 *   LDRD[<cond>] <Rd>, <address mode 3> | <pc relative label>
 *   LDRH[<cond>] <Rd>, <address mode 3> | <pc relative label>
 *   LDRSB[<cond>] <Rd>, <address mode 3> | <pc relative label>
 *   LDRSH[<cond>] <Rd>, <address mode 3> | <pc relative label>
 */
bool
m_ldr (bool doLowerCase)
{
  ARMWord cc = Option_LdrStrCondAndType (false, doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  return dstmem (cc, "LDR");
}

static bool
LdrStrEx (bool isLoad, bool doLowerCase)
{
  enum { wtype = 0x18<<20, dtype = 0x1A<<20, btype = 0x1C<<20, htype = 0x1E<<20 } type;
  if (Input_Match (doLowerCase ? 'b' : 'B', false))
    type = btype;
  else if (Input_Match (doLowerCase ? 'd' : 'D', false))
    type = dtype;
  else if (inputLook () == (doLowerCase ? 'h' : 'H'))
    {
      /* Small hack needed : 'H' can also be the first condition character
	 of 'HS' or 'HI'.  */
      if (inputLookN (1) != (doLowerCase ? 'i' : 'I')
          && inputLookN (1) != (doLowerCase ? 's' : 'S'))
	{
	  inputSkip ();
	  type = htype;
	}
      else
	type = wtype;
    }
  else
    type = wtype;

  ARMWord cc = optionCond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  if (type == wtype)
    Target_CheckCPUFeature (kCPUExt_v6, true);
  else
    Target_CheckCPUFeature (kCPUExt_v6K, true);
  
  /* The STREX* versions have an extra Rd register.  */
  ARMWord regD;
  if (!isLoad)
    {
      regD = getCpuReg ();
      if (regD == INVALID_REG)
	return false;
      skipblanks ();
      if (!Input_Match (',', true))
	{
	  error (ErrorError, "Missing ,");
	  return false;
	}
    }

  ARMWord regT = getCpuReg ();
  if (regT == INVALID_REG)
    return false;
  if (type == dtype && (regT & 1))
    error (ErrorError, "Register needs to be even");
  skipblanks ();
  if (!Input_Match (',', true))
    {
      error (ErrorError, "Missing ,");
      return false;
    }

  if (type == dtype)
    {
      ARMWord regT2 = getCpuReg ();
      if (regT2 == INVALID_REG)
	return false;
      if (regT2 != regT + 1)
	error (ErrorError, "Registers are not consecutive");
      skipblanks ();
      if (!Input_Match (',', true))
	{
	  error (ErrorError, "Missing ,");
	  return false;
	}
    }
  
  if (!Input_Match ('[', true))
    {
      error (ErrorError, "Missing [");
      return false;
    }
  ARMWord regN = getCpuReg ();
  if (regN == INVALID_REG)
    return false;
  skipblanks ();
  if (!Input_Match (']', false))
    {
      error (ErrorError, "Missing ]");
      return false;
    }
  if (isLoad)
    Put_Ins (4, cc | 0x00100F9F | type | (regN<<16) | (regT<<12));
  else
    Put_Ins (4, cc | 0x00000F90 | type | (regN<<16) | (regD<<12) | regT);
  return false;
}

/**
 * Implements LDREX/LDREXB/LDREXH.
 *   LDREX[<cond>] <Rd>, [<Rn>]
 *   LDREXB[<cond>] <Rd>, [<Rn>]
 *   LDREXH[<cond>] <Rd>, [<Rn>]
 *   LDREXD[<cond>] <Rd>, <Rd2>, [<Rn>]
 */
bool
m_ldrex (bool doLowerCase)
{
  return LdrStrEx (true, doLowerCase);
}

/**
 * Implements STR<cond>[B].
 * Pre-UAL:
 *   STR[<cond>] <Rd>, <address mode 2> | <pc relative label>
 *   STR[<cond>]T <Rd>, <address mode 2> | <pc relative label>
 *   STR[<cond>]B <Rd>, <address mode 2> | <pc relative label>
 *   STR[<cond>]BT <Rd>, <address mode 2> | <pc relative label>
 *   STR[<cond>]D <Rd>, <address mode 3> | <pc relative label>
 *   STR[<cond>]H <Rd>, <address mode 3> | <pc relative label>
 *   STR[<cond>]SB <Rd>, <address mode 3> | <pc relative label>
 *   STR[<cond>]SH <Rd>, <address mode 3> | <pc relative label>
 * UAL:
 *   STR[<cond>] <Rd>, <address mode 2> | <pc relative label>
 *   STRT[<cond>] <Rd>, <address mode 2> | <pc relative label>
 *   STRB[<cond>] <Rd>, <address mode 2> | <pc relative label>
 *   STRBT[<cond>] <Rd>, <address mode 2> | <pc relative label>
 *   STRD[<cond>] <Rd>, <address mode 3> | <pc relative label>
 *   STRH[<cond>] <Rd>, <address mode 3> | <pc relative label>
 *   STRSB[<cond>] <Rd>, <address mode 3> | <pc relative label>
 *   STRSH[<cond>] <Rd>, <address mode 3> | <pc relative label>
 */
bool
m_str (bool doLowerCase)
{
  ARMWord cc = Option_LdrStrCondAndType (true, doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  return dstmem (cc, "STR");
}

/**
 * Implements STREX/LDREXB/LDREXH.
 *   STREX[<cond>] <Rd>, [<Rn>]
 *   STREXB[<cond>] <Rd>, [<Rn>]
 *   STREXH[<cond>] <Rd>, [<Rn>]
 *   STREXD[<cond>] <Rd>, <Rd2>, [<Rn>]
 */
bool
m_strex (bool doLowerCase)
{
  return LdrStrEx (false, doLowerCase);
}


/**
 * Implements CLREX.
 */
bool
m_clrex (void)
{
  Target_CheckCPUFeature (kCPUExt_v6K, true);
  Put_Ins (4, 0xF57FF01F);
  return false;
}


/**
 * Implements PLD, PLDW and PLI.
 *   PLD{W} [<Rn>, #+/-<imm12>]
 *   PLD{W} <label>                        <= FIXME: not supported
 *   PLD{W} [<Rn>,+/-<Rm>{, <shift>}]
 *   PLI [<Rn>, #+/-<imm12>]
 *   PLI <label>                        <= FIXME: not supported
 *   PLI [<Rn>,+/-<Rm>{, <shift>}]
 */
/* FIXME: support PLDW & PLI  */
bool
m_pl (bool doLowerCase)
{
  enum { isPLD, isPLDW, isPLI } type;
  if (Input_Match (doLowerCase ? 'd' : 'D', false))
    {
      if (Input_Match (doLowerCase ? 'w' : 'W', false))
	type = isPLDW;
      else
	type = isPLD;
    }
  else if (Input_Match (doLowerCase ? 'i' : 'I', false))
    type = isPLI;
  else
    return true;

  if (!Input_IsEndOfKeyword ())
    return true;

  /* FIXME: we don't check in case of isPLDW that ARMv7 has MP extensions
     enabled.  */
  Target_CheckCPUFeature (type == isPLD ? kCPUExt_v5E : kArchExt_v7, true);

  skipblanks();

  if (!Input_Match ('[', true))
    error (ErrorError, "Expected '[' after PLD instruction");

  ARMWord ir = 0xf450f000 | P_FLAG;
  int op = getCpuReg (); /* Base register */
  ir |= LHS_OP (op);
  skipblanks();

  if (Input_Match (']', true))
    {			/* [base] */
      ir |= U_FLAG;	/* 0 nicer than -0 */
    }
  else
    {
      skipblanks();
      if (!Input_Match (',', true))
	error (ErrorError, "Expected ',' or ']' in PLD instruction");

      if (Input_Match ('#', false))
	{
	  const Value *offset = exprBuildAndEval (ValueInt);
	  switch (offset->Tag)
	    {
	      case ValueInt:
		ir = Fix_CPUOffset (NULL, 0, ir, offset->Data.Int.i);
		break;

	      default:
		error (ErrorError, "Illegal offset expression");
		break;
	    }

	  /* U_FLAG is fixed in Fix_CPUOffset() */
	}
      else
	{
	  ir |= U_FLAG;
	  if (Input_Match ('-', true))
	    ir &= ~U_FLAG;
	  else
	    Input_Match ('+', true);
	  if (Input_Match ('#', false))
	    {
	      /* Edge case - #XX */
	      error (ErrorError, "Unknown register definition in offset field");
	    }
	  ir = getRhs (false, true, ir) | REG_FLAG;
	}

      if (!Input_Match (']', true))
	error (ErrorError, "Expected closing ]");
    }
  Put_Ins (4, ir);
  return false;
}


static void
dstreglist (ARMWord ir, bool isPushPop)
{
  if (isPushPop)
    {
      ir |= BASE_MULTI (13);
      ir |= W_FLAG;
      skipblanks ();
    }
  else
    {
      ir |= BASE_MULTI (getCpuReg ());
      skipblanks ();
      if (Input_Match ('!', true))
	ir |= W_FLAG;
      if (!Input_Match (',', true))
	error (ErrorError, "Inserting missing comma before reglist");
    }

  /* Parse register list.  */
  ARMWord regList;
  if (inputLook () == '{')
    regList = Get_CPURList ();
  else
    {
      const Value *rlistValue = exprBuildAndEval (ValueInt);
      if (rlistValue->Tag != ValueInt || rlistValue->Data.Int.type != eIntType_CPURList)
	{
	  error (ErrorError, "Not a register list");
	  regList = 0;
	}
      else
	{
	  assert ((unsigned)rlistValue->Data.Int.i <= 0xFFFF);
	  regList = rlistValue->Data.Int.i;
	}
    }

  skipblanks ();
  if (Input_Match ('^', true))
    {
      if ((ir & W_FLAG) && !(regList & (1 << 15)))
	error (ErrorInfo, "Writeback together with force user");
      ir |= FORCE_FLAG;
    }

  /* Count number of registers loaded or saved.  */
  int numRegs = 0;
  for (int i = 0; i < 16; ++i)
    {
      if (regList & (1<<i))
	++numRegs;
    }
  if (GET_BASE_MULTI (ir) == 13 && (ir & W_FLAG))
    {
      if (numRegs & 1)
	{
	  if (gArea_Preserve8 == ePreserve8_Yes)
	    error (ErrorWarning, "Stack pointer update potentially breaks 8 byte stack alignment");
	  else if (gArea_Preserve8 == ePreserve8_Guess)
	    gArea_Preserve8Guessed = false;
	}
    }
  if (GET_BASE_MULTI (ir) == 15)
    error (ErrorWarning, "Use of PC as Rn is UNPREDICTABLE");
  if (numRegs == 0)
    error (ErrorWarning, "Specifying no registers to %s is UNPREDICTABLE", ir & L_FLAG ? "load" : "save");
  if ((ir & W_FLAG) && numRegs == 1 && Target_CheckCPUFeature (kArchExt_v7, false))
    error (ErrorWarning, "%s one register with writeback is UNPREDICTABLE for ARMv7 onwards, use %s instead",
           (ir & L_FLAG) ? "Loading" : "Saving",
           (ir & L_FLAG) ? "POP" : "PUSH");
  if (isPushPop && numRegs == 1)
    {
      /* Switch to LDR/STR.  */
      bool isLoad = ir & L_FLAG;
      int rt = ffs (regList) - 1;
      assert (regList == (1U << rt));
      ir = (ir & NV) | (isLoad ? 0x049D0004 : 0x052D0004) | (rt << 12);
      if (rt == 13)
	error (ErrorWarning, "%s r13 is UNPREDICTABLE", isLoad ? "Loading" : "Saving");
    }
  else
    {
      if ((ir & W_FLAG) /* Write-back is specified.  */
	  && (ir & L_FLAG) /* Is LDM/POP.  */
	  && (regList & (1 << GET_BASE_MULTI (ir))) /* Base reg. in reg. list.  */
	  && (!isPushPop || (regList ^ (1 << GET_BASE_MULTI (ir))) != 0) /* Is either LDM/STM, either multi-reg. POP/PUSH.  */)
	{
	  /* LDM instructions and multi-register POP instructions that specify base
	     register writeback and load their base register are permitted but
	     deprecated before ARMv7.
	     Use of such instructions is obsolete in ARMv7.  */
	  const char *what = isPushPop ? "multi-register POP" : "LDM";
	  if (!Target_CheckCPUFeature (kArchExt_v7, false))
	    error (ErrorWarning,
	           "Deprecated before ARMv7 : %s with writeback and base register in register list",
	           what);
	  else
	    error (ErrorWarning,
	           "Obsoleted from ARMv7 onwards : %s with writeback and base register in register list",
	           what);
	}
      ir |= regList;
    }
  if (option_pedantic && (ir & L_FLAG) && (1 << 15)
      && Target_CheckCPUFeature (kCPUExt_v4T, false) && !Target_CheckCPUFeature (kCPUExt_v5T, false))
    error (ErrorWarning, "ARMv4T does not switch ARM/Thumb state when LDM/POP specifies PC (use BX instead)");
  Put_Ins (4, ir);
}


/**
 * Implements LDM.
 */
bool
m_ldm (bool doLowerCase)
{
  ARMWord cc = optionCondLdmStm (true, doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  dstreglist (cc | 0x08100000, false);
  return false;
}


/**
 * Implements POP, i.e. LDM<cond>FD sp!, {...} (= LDM<cond>IA sp!, {...})
 * when more than one register is to be popped from the stack, or
 * LDR Rx, [sp, #4]! when one register is to be popped from the stack.
 * UAL syntax.
 */
bool
m_pop (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  dstreglist (cc | STACKMODE_IA | 0x08100000, true);
  return false;
}


/**
 * Implements STM.
 */
bool
m_stm (bool doLowerCase)
{
  ARMWord cc = optionCondLdmStm (false, doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  dstreglist (cc | 0x08000000, false);
  return false;
}


/**
 * Implements PUSH, i.e. STM<cond>FD sp!, {...} (= STM<cond>DB sp!, {...})
 * when more than one register is to be pushed on the stack, or
 * STR Rx, [sp], #-4 when one register is to be pushed on the stack.
 * UAL syntax.
 */
bool
m_push (bool doLowerCase)
{
  ARMWord cc = optionCond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  dstreglist (cc | STACKMODE_DB | 0x08000000, true);
  return false;
}


/**
 * Implements SWP / SWPB.
 */
bool
m_swp (bool doLowerCase)
{
  /* There is no Thumb equivalent of SWP/SWPB.  */
  if (State_GetInstrType () != eInstrType_ARM)
    return true;

  ARMWord cc = Option_CondB (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  Target_CheckCPUFeature (kCPUExt_v2a, true);
  if (option_pedantic && Target_CheckCPUFeature (kCPUExt_v6, false))
    error (ErrorWarning, "The use of SWP/SWPB is deprecated from ARMv6 onwards");

  int ir = cc | 0x01000090;
  ARMWord rt = getCpuReg ();
  ir |= DST_OP (rt);
  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%sdst", InsertCommaAfter);
  ARMWord rt2 = getCpuReg ();
  ir |= RHS_OP (rt2);	/* Note wrong order swp dst,rhs,[lsh] */
  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%slhs", InsertCommaAfter);
  if (!Input_Match ('[', true))
    error (ErrorError, "Inserting missing '['");
  ARMWord rn = getCpuReg ();
  ir |= DST_MUL (rn);
  skipblanks ();
  if (!Input_Match (']', true))
    error (ErrorError, "Inserting missing ']'");

  if (rt == 15 || rt2 == 15 || rn == 15)
    error (ErrorError, "SWP(B) registers can not be r15");
  if (rn == rt || rn == rt2)
    error (ErrorError, "SWP(B) address register can not be the same as one of the swap registers");

  Put_Ins (4, ir);
  return false;
}

typedef enum
{
  BL_eSY = 0xF,
  BL_eST = 0xE,
  BL_eISH = 0xB,
  BL_eSH = BL_eISH,
  BL_eISHST = 0xA,
  BL_eSHST = BL_eISHST,
  BL_eNSH = 0x7,
  BL_eUN = BL_eNSH,
  BL_eNSHST = 0x6,
  BL_eUNST = BL_eNSHST,
  BL_eOSH = 0x3,
  BL_eOSHST = 0x2
} Barrier_eType;

static Barrier_eType
GetBarrierType (void)
{
  skipblanks ();

  Barrier_eType result;
  if (Input_IsEolOrCommentStart ())
    result = BL_eSY;
  else if (Input_MatchKeywordLower ("ish"))
    result = BL_eISH;
  else if (Input_MatchKeywordLower ("ishst"))
    result = BL_eISHST;
  else if (Input_MatchKeywordLower ("nsh"))
    result = BL_eNSH;
  else if (Input_MatchKeywordLower ("nshst"))
    result = BL_eNSHST;
  else if (Input_MatchKeywordLower ("osh"))
    result = BL_eOSH;
  else if (Input_MatchKeywordLower ("oshst"))
    result = BL_eOSHST;
  else if (Input_MatchKeywordLower ("sh"))
    {
      if (option_pedantic)
	error (ErrorWarning, "Use barrier type %s instead of %s", "ISH", "SH");
      result = BL_eSH;
    }
  else if (Input_MatchKeywordLower ("shst"))
    {
      if (option_pedantic)
	error (ErrorWarning, "Use barrier type %s instead of %s", "ISHST", "SHST");
      result = BL_eSHST;
    }
  else if (Input_MatchKeywordLower ("sy"))
    result = BL_eSY;
  else if (Input_MatchKeywordLower ("st"))
    result = BL_eST;
  else if (Input_MatchKeywordLower ("un"))
    {
      if (option_pedantic)
	error (ErrorWarning, "Use barrier type %s instead of %s", "NSH", "UN");
      result = BL_eUN;
    }
  else if (Input_MatchKeywordLower ("unst"))
    {
      if (option_pedantic)
	error (ErrorWarning, "Use barrier type %s instead of %s", "NSHST", "UNST");
      result = BL_eUNST;
    }
  else
    {
      error (ErrorError, "Unknown barrier type");
      result = BL_eSY;
    }

  return result;
}

/**
 * Implements DMB.
 * ARM DMB is unconditional.
 */
bool
m_dmb (void)
{
  Target_CheckCPUFeature (kCPUExt_BARRIER, true);
  Barrier_eType bl = GetBarrierType ();
  Put_Ins (4, 0xF57FF050 | bl);
  return false;
}


/**
 * Implements DSB.
 * ARM DSB is unconditional.
 */
bool
m_dsb (void)
{
  Target_CheckCPUFeature (kCPUExt_BARRIER, true);
  Barrier_eType bl = GetBarrierType ();
  Put_Ins (4, 0xF57FF040 | bl);
  return false;
}


/**
 * Implements ISB.
 * ARM ISB is unconditional.
 */
bool
m_isb (void)
{
  Target_CheckCPUFeature (kCPUExt_BARRIER, true);
  Barrier_eType bl = GetBarrierType ();
  if (option_pedantic && bl != BL_eSY)
    error (ErrorWarning, "Using reserved barrier type");
  Put_Ins (4, 0xF57FF060 | bl);
  return false;
}


/**
 * Implements RFE.
 *   RFE{<amode>} <Rn>{!}
 */
bool
m_rfe (bool doLowerCase)
{
  ARMWord option = Option_CondRfeSrs (true, doLowerCase);
  if (option == kOption_NotRecognized)
    return true;

  Target_CheckCPUFeature (kCPUExt_v6, true);

  ARMWord regN = getCpuReg ();
  if (regN == INVALID_REG)
    return false;
  if (regN == 15)
    error (ErrorError, "Using PC as base register is unpredictable");

  skipblanks ();
  bool updateStack = Input_Match ('!', false);

  if (updateStack)
    option |= W_FLAG;
  Put_Ins (4, 0xF8100A00 | option | (regN<<16));
  return false;
}


/**
 * Implements SRS.
 *   SRS{<amode>} SP{!},#<mode>  : UAL syntax
 *   SRS{<amode>} #<mode>{!}     : pre-UAL syntax
 */
bool
m_srs (bool doLowerCase)
{
  ARMWord option = Option_CondRfeSrs (false, doLowerCase);
  if (option == kOption_NotRecognized)
    return true;

  Target_CheckCPUFeature (kCPUExt_v6, true);

  skipblanks ();
  bool isUALSyntax, updateStack;
  if (!Input_Match ('#', false))
    {
      isUALSyntax = true;
      ARMWord sp = getCpuReg ();
      if (sp == INVALID_REG)
	return false;
      if (sp != 13)
	{
	  error (ErrorError, "SRS can only be used with stack register 13 (sp)");
	  return false;
	}
      skipblanks ();
      updateStack = Input_Match ('!', true);
      if (!Input_Match (',', true))
	{
	  error (ErrorError, "Missing ,");
	  return false;
	}
      if (!Input_Match ('#', false))
	{
	  error (ErrorError, "%s needs a mode specified", "SRS");
	  return false;
	}
    }
  else
    isUALSyntax = false;

  const Value *im = exprBuildAndEval (ValueInt);
  if (im->Tag != ValueInt)
    {
      error (ErrorError, "Illegal immediate expression");
      return false;
    }
  int mode = im->Data.Int.i;
  if (!Option_IsValidARMMode (mode))
    {
      error (ErrorWarning, "Mode 0x%x is not a valid ARM mode", mode);
      mode &= 0x1F;
    }

  if (!isUALSyntax)
    {
      skipblanks ();
      updateStack = Input_Match ('!', false);
    }

  if (updateStack)
    option |= W_FLAG;
  Put_Ins (4, 0xF84D0500 | option | mode);

  return false;
}


/**
 * \return true when ARM MOVW instruction can be used to load given constant.
 */
bool
CPUMem_ConstantInMOVW (uint32_t constant)
{
  return Target_CheckCPUFeature (kCPUExt_v6T2, false) && (constant & 0xFFFF0000U) == 0;
}
