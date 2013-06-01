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
#include "reloc.h"
#include "state.h"
#include "targetcpu.h"

/**
 * Parses Address mode 2 and 3.
 *
 * Similar to HelpCop_Addr() @ help_cop.c.
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
  unsigned dst = Get_CPUReg ();
  ir |= DST_OP (dst);
  Input_SkipWS ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%sdst", InsertCommaAfter);

  const bool doubleReg = (ir & (L_FLAG | S_FLAG)) == S_FLAG;
  const bool translate = (ir & W_FLAG) != 0; /* We have "T" specified in our mnemonic.  */

  if (doubleReg)
    {
      if (dst & 1)
	Error (ErrorWarning, "Uneven first transfer register is UNPREDICTABLE");
      /* Try to parse "<reg>, " and check this register is dst + 1.
         If we can't parse this, assume pre-UAL syntax and move on.  */
      ARMWord dstPlusOne = Get_CPURegNoError ();
      if (dstPlusOne != INVALID_REG)
	{
	  if (dstPlusOne != dst + 1)
	    Error (ErrorError, "Second transfer register is not %d but %d", dst + 1, dstPlusOne);
	  Input_SkipWS ();
	  if (!Input_Match (',', true))
	    Error (ErrorError, "%sdst", InsertCommaAfter);
	}
    }
  
  const ARMWord offset = areaCurrentSymbol->area->curIdx;
  Value value;
  const Value *valP = NULL;
  bool movOptAllowed = false; /* true when MOV/MVN/MOVW optimisation is allowed.  */
  switch (Input_Look ())
    {
      case '[':	/* <reg>, [ */
	{
	  Input_Skip ();
	  const unsigned baseReg = Get_CPUReg (); /* Base register */
	  Input_SkipWS ();
	  bool pre = !Input_Match (']', true);
	  bool forcePreIndex;
	  if (Input_Match (',', true))
	    {
	      /* Either [base,XX] (pre = true) or [base],XX (pre = false).  */
	      if (Input_Match ('#', false))
		{
		  if (isAddrMode3)
		    ir |= B_FLAG;  /* Immediate mode for addr. mode 3.  */
		  
		  const Value *valueP = Expr_BuildAndEval (ValueInt);
		  if (gPhase != ePassOne)
		    {
		      switch (valueP->Tag)
			{
			  case ValueInt:
			    value = Value_Addr (baseReg, valueP->Data.Int.i);
			    valP = &value;
			    break;
			  default:
			    Error (ErrorError, "Illegal offset expression");
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
		      /* We're about to call Get_RHS() and that supports "#<immediate>"
			 which isn't possible for our case here.  Hence, check
			 on this.  */
		      Error (ErrorError, "Unknown register definition in offset field");
		    }
		  ir |= isAddrMode3 ? 0 : REG_FLAG;
		  ir = Get_RHS (false, !isAddrMode3, ir);
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
		Error (ErrorError, "Illegal character");
	      /* Pre-index nicer than post-index but don't this when 'T' is
	         specified as pre-index is not supported (FIXME: ARM only).  */
	      forcePreIndex = !translate;
	    }

	  if (pre)
	    {
	      if (!Input_Match (']', true))
		Error (ErrorError, "Inserting missing ] after address");
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
	        Error (ErrorError, "Writeback not allowed with post-index");
	    }
	  if (pre)
	    {
	      /* Pre-index.  */
	      ir |= P_FLAG;
	      if (translate)
	        Error (ErrorError, "Translate not allowed with pre-index");
	    }
	  else
	    {
	      /* Post-index.  */
	      if (dst == baseReg)
	        Error (ErrorError, "Post increment is not sane where base and destination register are the same");
	    }
	  break;
	}

      case '=': /* <reg>, =<expression> */
	{
	  Input_Skip ();
	  /* Is LDRD ? */
	  if ((ir & 0x0E1000D0) == 0xD0)
	    Error_Abort ("No support for LDRD and literal"); /* FIXME: */
	  /* This is only allowed for LDR.  */
	  else if ((ir & L_FLAG) == 0)
	    Error_Abort ("You can't store into a constant");

          if (translate)
	    Error (ErrorError, "Translate not allowed with literal");
	  
	  /* This is always pre-increment.  */
	  ir |= P_FLAG;

	  /* FIXME: ARM LDR{B}T can only use post-indexing, so =<expression>
	     can not be supported.  Thumb LDR{B}T + literal is in fact
	     LDR{B} + literal.
	     So LDR+T + literal -> error.  */
	  Lit_eSize litSize;
	  Lit_eAddrType addrType;
	  if ((ir & 0x0C500000) == 0x04100000) /* LDR */
	    {
	      litSize = eLitIntWord;
	      addrType = eLitAddr2;
	    }
	  else if ((ir & 0x0C500000) == 0x04500000)
	    {
	      litSize = eLitIntUByte; /* LDRB, LDRBT */
	      addrType = eLitAddr2;
	    }
	  else if ((ir & 0x0E1000F0) == 0x001000B0)
	    {
	      litSize = eLitIntUHalfWord; /* LDRH */
	      addrType = eLitAddr3;
	    }
	  else if ((ir & 0x0E1000F0) == 0x001000D0)
	    {
	      litSize = eLitIntSByte; /* LDRSB */
	      addrType = eLitAddr3;
	    }
	  else if ((ir & 0x0E1000F0) == 0x001000F0)
	    {
	      litSize = eLitIntSHalfWord; /* LDRSH */
	      addrType = eLitAddr3;
	    }
	  else
	    assert (0);

	  /* The ValueInt | ValueSymbol | ValueCode tags are what we support
	     as constants from user point of view.  */
	  const Value *literalP = Expr_BuildAndEval (ValueInt | ValueSymbol | ValueCode);
	  if (literalP->Tag == ValueIllegal)
	    Error (ErrorError, "Wrong literal type");
	  else
	    {
	      value = Lit_RegisterInt (literalP, litSize, addrType, eInstrType_ARM);
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
	    Error (ErrorError, "Translate not allowed with label");

	  /* Whatever happens, this must be a pre-increment.  */
	  ir |= P_FLAG;

	  valP = Expr_BuildAndEval (ValueInt | ValueAddr | ValueSymbol);
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
	      if ((im = HelpCPU_Imm8s4 (valP->Data.Int.i)) != UINT32_MAX)
		ir |= M_MOV | IMM_RHS | im; /* Optimize to MOV.  */
	      else if ((im = HelpCPU_Imm8s4 (~valP->Data.Int.i)) != UINT32_MAX)
		ir |= M_MVN | IMM_RHS | im; /* Optimize to MVN.  */
	      else if (CPUMem_ConstantInMOVW (valP->Data.Int.i))
		ir |= 0x03000000 | ((valP->Data.Int.i & 0xF000) << 4) | (valP->Data.Int.i & 0x0FFF); /* Optimize to MOVW.  */
	      else
		assert (0);
	      valP = NULL;
	    }
	  else if ((areaCurrentSymbol->area->type & AREA_ABS) != 0)
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
		      Reloc_CreateAOF (HOW2_INIT | HOW2_INSTR_UNLIM | HOW2_RELATIVE, offset, valP);
		      // FIXME: Reloc_CreateELF ();
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
		  ir = Fix_CPUOffset (ir, valP->Data.Addr.i);
		  break;
		}

	      default:
		Error (ErrorError, "Illegal %s expression", mnemonic);
		break;
	    }
	}
    }

  if ((ir & 0x0e000000) == 0 /* Address mode 3.  */
      && (ir & (1<<5)) != 0 /* Half-word.  */
      && (areaCurrentSymbol->area->type & AREA_HALFWORD) == 0
      && option_aof)
    Error (ErrorWarning, "Half-word memory access without area being marked with HALFWORD");

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
  else if (Input_Look () == (doLowerCase ? 'h' : 'H'))
    {
      /* Small hack needed : 'H' can also be the first condition character
	 of 'HS' or 'HI'.  */
      if (Input_LookN (1) != (doLowerCase ? 'i' : 'I')
          && Input_LookN (1) != (doLowerCase ? 's' : 'S'))
	{
	  Input_Skip ();
	  type = htype;
	}
      else
	type = wtype;
    }
  else
    type = wtype;

  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  if (type == wtype)
    Target_CheckCPUFeature (kCPUExt_v6, true);
  else
    Target_CheckCPUFeature (kCPUExt_v6K, true);
  
  /* The STREX* versions have an extra Rd register.  */
  unsigned regD;
  if (!isLoad)
    {
      regD = Get_CPUReg ();
      if (regD == INVALID_REG)
	return false;
      Input_SkipWS ();
      if (!Input_Match (',', true))
	{
	  Error (ErrorError, "Missing ,");
	  return false;
	}
    }

  unsigned regT = Get_CPUReg ();
  if (regT == INVALID_REG)
    return false;
  if (type == dtype && (regT & 1))
    Error (ErrorError, "Register needs to be even");
  Input_SkipWS ();
  if (!Input_Match (',', true))
    {
      Error (ErrorError, "Missing ,");
      return false;
    }

  if (type == dtype)
    {
      unsigned regT2 = Get_CPUReg ();
      if (regT2 == INVALID_REG)
	return false;
      if (regT2 != regT + 1)
	Error (ErrorError, "Registers are not consecutive");
      Input_SkipWS ();
      if (!Input_Match (',', true))
	{
	  Error (ErrorError, "Missing ,");
	  return false;
	}
    }
  
  if (!Input_Match ('[', true))
    {
      Error (ErrorError, "Missing [");
      return false;
    }
  unsigned regN = Get_CPUReg ();
  if (regN == INVALID_REG)
    return false;
  Input_SkipWS ();
  if (!Input_Match (']', false))
    {
      Error (ErrorError, "Missing ]");
      return false;
    }

  if (type == htype
      && (areaCurrentSymbol->area->type & AREA_HALFWORD) == 0
      && option_aof)
    Error (ErrorWarning, "Half-word memory access without area being marked with HALFWORD");

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

  Input_SkipWS ();

  if (!Input_Match ('[', true))
    Error (ErrorError, "Expected '[' after PLD instruction");

  ARMWord ir = 0xf450f000 | P_FLAG;
  unsigned op = Get_CPUReg (); /* Base register */
  ir |= LHS_OP (op);
  Input_SkipWS ();

  if (Input_Match (']', true))
    {			/* [base] */
      ir |= U_FLAG;	/* 0 nicer than -0 */
    }
  else
    {
      Input_SkipWS ();
      if (!Input_Match (',', true))
	Error (ErrorError, "Expected ',' or ']' in PLD instruction");

      if (Input_Match ('#', false))
	{
	  const Value *offset = Expr_BuildAndEval (ValueInt);
	  switch (offset->Tag)
	    {
	      case ValueInt:
		ir = Fix_CPUOffset (ir, offset->Data.Int.i);
		break;

	      default:
		Error (ErrorError, "Illegal offset expression");
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
	      Error (ErrorError, "Unknown register definition in offset field");
	    }
	  ir = Get_RHS (false, true, ir) | REG_FLAG;
	}

      if (!Input_Match (']', true))
	Error (ErrorError, "Expected closing ]");
    }
  Put_Ins (4, ir);
  return false;
}

typedef struct
{
  unsigned baseReg;
  bool writeBack;
  bool hat; /* When ^ is specified.  Only valid for LDM/STM in ARM mode.  */
  unsigned regList;
} LSM_Arg_t;

typedef enum
{
  eIsLDM,
  eIsPop,
  eIsSTM,
  eIsPush
} LSM_Type_e;

/**
 * Get the arguments of a load/store multiple instruction (LDM, STM, PUSH, POP).
 */
static LSM_Arg_t
GetLoadStoreMultipleArg (LSM_Type_e lsmType)
{
  LSM_Arg_t rtrn;
  if (lsmType == eIsPop || lsmType == eIsPush)
    {
      /* Base register is fixed (r13) and writeback is always implicitely set.  */
      rtrn.baseReg = 13;
      rtrn.writeBack = true;
      Input_SkipWS ();
    }
  else
    {
      rtrn.baseReg = Get_CPUReg ();
      Input_SkipWS ();
      rtrn.writeBack = Input_Match ('!', true);
      if (!Input_Match (',', true))
	Error (ErrorError, "%sbase register", InsertCommaAfter);
    }

  /* Parse register list.  */
  if (Input_Look () == '{')
    rtrn.regList = Get_CPURList ();
  else
    {
      /* Symbol representing a register list.  */
      const Value *rlistValue = Expr_BuildAndEval (ValueInt);
      if (rlistValue->Tag != ValueInt || rlistValue->Data.Int.type != eIntType_CPURList)
	{
	  Error (ErrorError, "Not a register list");
	  rtrn.regList = 0;
	}
      else
	{
	  assert ((rlistValue->Data.Int.i & ~0xFFFFu) == 0);
	  rtrn.regList = rlistValue->Data.Int.i;
	}
    }

  Input_SkipWS ();
  rtrn.hat = false;
  if (Input_Match ('^', true))
    {
      /* Only LDM/STM can have an optional ^.  */
      if (lsmType == eIsLDM || lsmType == eIsSTM)
	rtrn.hat = true;
      else
	Error (ErrorError, "PUSH/POP can not have ^ specified");
    }

  return rtrn;
}

static bool
LoadStoreMultiple (ARMWord cc, bool doLowerCase, LSM_Type_e lsmType)
{
  InstrWidth_e instrWidth = Option_GetInstrWidth (doLowerCase);
  if (instrWidth == eInstrWidth_Unrecognized)
    return true;

  InstrType_e instrState = State_GetInstrType ();
  IT_ApplyCond (cc, instrState != eInstrType_ARM); 

  const LSM_Arg_t lsmArg = GetLoadStoreMultipleArg (lsmType);

  /* Count number of registers loaded or saved.  */
  unsigned numRegs = 0;
  for (unsigned i = 0; i != 16; ++i)
    {
      if (lsmArg.regList & (1u << i))
	++numRegs;
    }
  unsigned singleRegInList; /* Only valid when numRegs == 1.  */
  if (numRegs == 1)
    {
      singleRegInList = ffs (lsmArg.regList) - 1;
      assert (lsmArg.regList == (1U << singleRegInList));
    }

  /* For PRESERVE8, check if the number of stack elements is even.  */
  if (lsmArg.baseReg == 13 && lsmArg.writeBack)
    {
      if ((numRegs & 1) != 0)
	{
	  if (gArea_Preserve8 == ePreserve8_Yes)
	    Error (ErrorWarning, "Stack pointer update potentially breaks 8 byte stack alignment");
	  else if (gArea_Preserve8 == ePreserve8_Guess)
	    gArea_Preserve8Guessed = false;
	}
    }

  if (lsmArg.baseReg == 15)
    Error (ErrorWarning, "Use of PC as Rn is UNPREDICTABLE");
  if (numRegs == 0)
    Error (ErrorWarning, "Specifying no registers to %s is UNPREDICTABLE", lsmType == eIsLDM || lsmType == eIsPop ? "load" : "save");
  if (!lsmArg.hat && lsmArg.writeBack && numRegs == 1)
    {
      /* One register save/load via LDM/STM on stack (with writeback) can
	 better be done using POP/PUSH as those get optimised to LDR/STR.  */
      if (lsmArg.baseReg == 13 && lsmType == eIsLDM)
	Error (ErrorInfo, "Loading one register with writeback from stack can better be done using POP");
      else if (lsmArg.baseReg == 13 && lsmType == eIsSTM)
	Error (ErrorInfo, "Saving one register with writeback on stack can better be done using PUSH");
    }
  if (option_pedantic
      && (lsmType == eIsLDM || lsmType == eIsPop)
      && (lsmArg.regList &  (1 << 15))
      && Target_CheckCPUFeature (kCPUExt_v4T, false) && !Target_CheckCPUFeature (kCPUExt_v5T, false))
    Error (ErrorWarning, "ARMv4T does not switch ARM/Thumb state when LDM/POP specifies PC (use BX instead)");

// FIXME: LDM (?)/POP + Thumb with PC in reglist && IT Block && not last instr in IT block -> UNPREDICTABLE

  /* Simplifies ARM/Thumb2 implementation.  */
  switch (lsmType)
    {
      case eIsPop:
	cc |= STACKMODE_IA;
	break;
      case eIsPush:
	cc |= STACKMODE_DB;
	break;
      case eIsLDM:
      case eIsSTM:
	break;
    }
  if (instrState == eInstrType_ARM)
    {
      ARMWord ir;
      /* Encode one register load/store with POP/PUSH into LDR/STR.  */
      if (numRegs == 1 && (lsmType == eIsPop || lsmType == eIsPush))
	{
	  ir = (cc & NV) | (lsmType == eIsPop ? 0x049D0004 : 0x052D0004) | (singleRegInList << 12);
	  if (singleRegInList == 13)
	    Error (ErrorWarning, "%s r13 is UNPREDICTABLE", lsmType == eIsPop ? "Loading" : "Saving");
	}
      else
	{	    
	  switch (lsmType)
	    {
	      case eIsLDM:
		/* LDM (user register) with write-back is UNPREDICTABLE.  */
		if (lsmArg.hat && (lsmArg.regList & 0x8000) == 0 && lsmArg.writeBack)
		  Error (ErrorWarning, "Writeback of base register when loading user registers is UNPREDICTABLE");
		/* LDM (user register) before ARMv6 needs to be followed by a
		   NOP before a banked register can be accessed.  */
		if (lsmArg.hat && (lsmArg.regList & 0x8000) == 0 && !Target_CheckCPUFeature (kArchExt_v6, false))
		  Error (ErrorInfo, "Instruction after LDM (user register) can not access banked register");
		/* Fall through.  */
	      case eIsPop:
		/* From ARMv6T2 onwards, SP in LDM/POP register list is DEPRECATED.  */
		if ((lsmArg.regList & 0x2000u) != 0 && Target_CheckCPUFeature (kArchExt_v6T2, false))
		  Error (ErrorWarning, "LDM/POP with SP in register list is DEPRECATED from ARMv6T2 onwards");
		/* From ARMv6T2 onwards, both LR and PC in LDM/POP register list is DEPRECATED.  */
		if ((lsmArg.regList & 0xC000u) == 0xC000u && Target_CheckCPUFeature (kArchExt_v6T2, false))
		  Error (ErrorWarning, "LDM/POP with both LR and PC in register list is DEPRECATED from ARMv6T2 onwards");
		/* LDM/POP with writeback and base register in register list is DEPRECATED/UNPREDICTABLE.  */
		if (lsmArg.writeBack
		    && (lsmArg.regList & (1u << lsmArg.baseReg)) != 0)
		  {
		    if (Target_CheckCPUFeature (kArchExt_v7, false))
		      Error (ErrorWarning, "LDM/POP with writeback and base register in register list is UNPREDICTABLE from ARMv7 onwards");
		    else
		      Error (ErrorWarning, "LDM/POP with writeback and base register in register list is DEPRECATED");
		  }
		ir = cc | 0x08100000; /* cc already contains the stack type.  */
		break;

	      case eIsSTM:
		/* STM (user register) with write-back is UNPREDICTABLE.  */
		if (lsmArg.hat && lsmArg.writeBack)
		  Error (ErrorWarning, "Writeback of base register when loading user registers is UNPREDICTABLE");
		/* Fall through.  */
	      case eIsPush:
		/* From ARMv6T2 onwards, SP and/or PC in STM/PUSH register list is DEPRECATED.  */
		if ((lsmArg.regList & 0xA000u) != 0 && Target_CheckCPUFeature (kArchExt_v6T2, false))
		  Error (ErrorWarning, "STM/PUSH with SP or PC in register list is DEPRECATED from ARMv6T2 onwards");
		/* STM/PUSH with writeback and base register in register list is DEPRECATED/UNPREDICTABLE.  */
		if (lsmArg.writeBack
		    && (lsmArg.regList & (1u << lsmArg.baseReg)) != 0)
		  {
		    if ((lsmArg.regList & ((1u << lsmArg.baseReg)-1)) != 0)
		      Error (ErrorWarning, "Base register with writeback is not lowest-numbered in register list is UNPREDICTABLE");
		    else
		      Error (ErrorWarning, "Base register with writeback as lowest-numbered in register list is DEPRECATED");
		  }
		ir = cc | 0x08000000; /* cc already contains the stack type.  */
		break;
	    }
	  ir |= lsmArg.baseReg << 16;
	  if (lsmArg.writeBack)
	    ir |= W_FLAG;
	  if (lsmArg.hat)
	    ir |= FORCE_FLAG;
	  ir |= lsmArg.regList;
	}
      Put_Ins (4, ir);
    }
  else
    {
      /* Thumb or Thumb2.  */

      if (lsmArg.hat)
	{
	  assert (lsmType == eIsLDM || lsmType == eIsSTM);
	  if (lsmType == eIsLDM)
	    Error (ErrorError, "LDM user register or exception return is not possible in Thumb");
	  else
	    Error (ErrorError, "STM user register is not possible in Thumb");
	}

      /* Use 16-bit Thumb when possible.  If not and wide instruction width
         is specified, give an error.  */
      if (instrWidth == eInstrWidth_NotSpecified
          || instrWidth == eInstrWidth_Enforce16bit)
	{
	  const char *err = NULL;
	  switch (lsmType)
	    {
	      case eIsLDM:
	      case eIsSTM:
		if ((lsmArg.regList & ~0xFFu) != 0)
		  err = "Thumb LDM/STM only allows R0-R7 in register list";
		if ((lsmArg.baseReg & ~7) != 0)
		  err = "Thumb base register can only be one of R0 ... R7";
		/* Writeback is needed, *unless* 
		   1) There is only one register to load/save as then we're
		      using "LDR/STR rx, [ry, ..." alike encoding.
		   2) Or, base register is in reglist and it's LDM.  */
		if (!lsmArg.writeBack
		    && !(numRegs == 1
		         || (lsmType == eIsLDM && (lsmArg.regList & (1u << lsmArg.baseReg)) != 0)))
		  err = "Thumb LDM/STM needs base register writeback";
		if ((cc & STACKMODE_BITS) != STACKMODE_IA)
		  err = "Thumb doesn't support non-IA stackmode";
		break;

	      case eIsPop:
		/* Only R0-R7 and PC are allowed in register list.  */
		if ((lsmArg.regList & ~0x80FFu) != 0)
		  err = "Thumb POP only allows R0-R7 and PC in register list";
		break;

	      case eIsPush:
		/* Only R0-R7 and LR are allowed in register list.  */
		if ((lsmArg.regList & ~0x40FFu) != 0)
		  err = "Thumb PUSH only allows R0-R7 and LR in register list";
		break;
	    }
	  if (err)
	    {
	      if (instrWidth == eInstrWidth_Enforce16bit)
		Error (ErrorError, "%s", err);
	      else
	        instrWidth = eInstrWidth_Enforce32bit;
	    }
	}

      if (instrWidth == eInstrWidth_Enforce32bit)
	{
	  Target_CheckCPUFeature (kCPUExt_v6T2, true);

	  if ((cc & STACKMODE_BITS) != STACKMODE_IA
	      && (cc & STACKMODE_BITS) != STACKMODE_DB)
	    Error (ErrorError, "Stack mode is not supported in Thumb2 mode");

	  ARMWord ir;
	  switch (lsmType)
	    {
	      case eIsPop:
	      case eIsLDM:
		/* LDM/POP with writeback and base register in register list is UNPREDICTABLE.  */
		if (lsmArg.writeBack
		    && (lsmArg.regList & (1u << lsmArg.baseReg)) != 0)
		  Error (ErrorWarning, "LDM/POP with writeback and base register in register list is UNPREDICTABLE");
		/* One register LDM/POP gets encoded differently as 32-bit
		   Thumb LDM/POP instruction would be UNPREDICTABLE.  */
		if (numRegs == 1)
		  {
		    /* LDR T3 encoding:
		         LDR<c>.W <Rt>, [<Rn>{, #<imm12>}] with <imm12> = 0: LDMIA rx, {ry} (U_FLAG)
		       LDR T4 encoding:
			 LDR<c> <Rt>, [<Rn>, #-<imm8>] with -<imm8> = -4 : LDMDB rx, {ry} (P_FLAG)
			 LDR<c> <Rt>, [<Rn>], #+/-<imm8> with +/-<imm8> = 4 : LDMIA rx!, {ry} (U_FLAG)
			 LDR<c> <Rt>, [<Rn>, #+/-<imm8>]! with +/-<imm8> = -4 : LDMDB rx!, {ry} (P_FLAG) */
		    if ((cc & U_FLAG) != 0 && !lsmArg.writeBack)
		      ir = 0xF8D00000 | (lsmArg.baseReg << 16) | (singleRegInList << 12);
		    else
		      ir = 0xF8500000 | (lsmArg.baseReg << 16) | (singleRegInList << 12) | 0x0800 | ((cc & P_FLAG) ? 0x0400 : 0) | ((cc & U_FLAG) ? 0x0200 : 0) | (lsmArg.writeBack << 8) | 0x0004;
		  }
		else
		  {
		    /* Only R0-R12, LR and PC are allowed in register list.  */
		    if ((lsmArg.regList & ~0xDFFFu) != 0)
		      Error (ErrorError, "Thumb2 LDM/POP only allow R0-R12, LR and PC in register list");
		    /* LR and PC can not both be in register list.  */
		    if ((lsmArg.regList & 0xC000u) == 0xC000u)
		      Error (ErrorError, "Thumb2 LDM/POP can not have both LR and PC in register list");
		    ir = 0xE8100000 | ((cc & P_FLAG) ? 0x01000000 : 0) | ((cc & U_FLAG) ? 0x00800000 : 0) | (lsmArg.writeBack << 21) | (lsmArg.baseReg << 16) | lsmArg.regList;
		  }
		break;

	      case eIsPush:
	      case eIsSTM:
		/* One register STM/PUSH gets encoded differently as 32-bit
		   Thumb STM/PUSH instruction would be UNPREDICTABLE.  */
		if (numRegs == 1)
		  {
		    if (lsmType == eIsPush && singleRegInList == 13)
		      Error (ErrorWarning, "PUSH with r13 in register list is UNPREDICTABLE");
		    /* STR T3 encoding:
		         STR<c>.W <Rt>, [<Rn>{, #<imm12>}] with <imm12> = 0: STMIA rx, {ry} (U_FLAG)
		       STR T4 encoding:
			 STR<c> <Rt>, [<Rn>, #-<imm8>] with -<imm8> = -4 : STMDB rx, {ry} (P_FLAG)
			 STR<c> <Rt>, [<Rn>], #+/-<imm8> with +/-<imm8> = 4 : STMIA rx!, {ry} (U_FLAG)
			 STR<c> <Rt>, [<Rn>, #+/-<imm8>]! with +/-<imm8> = -4 : STMDB rx!, {ry} (P_FLAG) */
		    if ((cc & U_FLAG) != 0 && !lsmArg.writeBack)
		      ir = 0xF8C00000 | (lsmArg.baseReg << 16) | (singleRegInList << 12);
		    else
		      ir = 0xF8400000 | (lsmArg.baseReg << 16) | (singleRegInList << 12) | 0x0800 | ((cc & P_FLAG) ? 0x0400 : 0) | ((cc & U_FLAG) ? 0x0200 : 0) | (lsmArg.writeBack << 8) | 0x0004;
		  }
		else
		  {
		    /* Only R0-R12 and LR are allowed in register list.  */
		    if ((lsmArg.regList & ~0x5FFFu) != 0)
		      Error (ErrorError, "Thumb2 STM/PUSH only allow R0-R12 and LR in register list");
		    if (lsmArg.writeBack
		        && (lsmArg.regList & (1u << lsmArg.baseReg)) != 0)
		      Error (ErrorError, "Thumb2 mode does not support STM with write back and base register in register list");
		    ir = 0xE8000000 | ((cc & P_FLAG) ? 0x01000000 : 0) | ((cc & U_FLAG) ? 0x00800000 : 0) | (lsmArg.writeBack << 21) | (lsmArg.baseReg << 16) | lsmArg.regList;
		  }
		break;
	    }
	  Put_Ins (4, ir);
	}
      else
	{
	  /* 16-bit Thumb LDM/STM is not available in ThumbEE state.  */
	  if (State_GetInstrType () == eInstrType_ThumbEE)
	    Error (ErrorError, "Instruction not available in ThumbEE state");

	  ARMWord ir;
	  switch (lsmType)
	    {
	      case eIsLDM:
		if (!lsmArg.writeBack && numRegs == 1)
		  {
		    assert (numRegs == 1);
		    ir = 0x6800 | (lsmArg.baseReg << 3) | singleRegInList;
		  }
		else
		  {
		    if (lsmArg.writeBack
		        && (lsmArg.regList & (1u << lsmArg.baseReg)) != 0)
		      Error (ErrorError, "LDM with writeback and base register in register list is not possible");
		    ir = 0xC800 | (lsmArg.baseReg << 8) | (lsmArg.regList & 0xFFu);
		  }
		break;

	      case eIsPop:
		/* Oddly enough, one register in regList is ok here.  */
		ir = 0xBC00 | ((lsmArg.regList & 0x8000u) >> (15-8)) | (lsmArg.regList & 0xFFu);
		break;

	      case eIsSTM:
		if (!lsmArg.writeBack)
		  {
		    assert (numRegs == 1);
		    ir = 0x6000 | (lsmArg.baseReg << 3) | singleRegInList;
		  }
		else
		  {
		    /* 16-bit Thumb STM instructions with writeback that
		       specify base register as the lowest register in the
		       reglist are deprecated in ARMv6T2 and above.  */
		    if ((lsmArg.regList & ((1u << (lsmArg.baseReg + 1))-1)) == (1u << lsmArg.baseReg)
		        && Target_CheckCPUFeature (kArchExt_v6T2, false))
		      Error (ErrorWarning, "STM with writeback when base register is lowest register in register list is DEPRECATED from ARMv6T2 onwards");
		    ir = 0xC000 | (lsmArg.baseReg << 8) | (lsmArg.regList & 0xFFu);
		  }
		break;

	      case eIsPush:
		/* Oddly enough, one register in regList is ok here.  */
		ir = 0xB400 | ((lsmArg.regList & 0x4000u) >> (14-8)) | (lsmArg.regList & 0xFFu);
		break;
	    }
	  Put_Ins (2, ir);
	}
    }

  return false;
}


/**
 * Implements LDM.
 */
bool
m_ldm (bool doLowerCase)
{
  ARMWord cc = Option_CondLdmStm (true, doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;

  return LoadStoreMultiple (cc, doLowerCase, eIsLDM);
}


/**
 * Implements POP, i.e. LDM<cond>FD sp!, {...} when more than one register is
 * to be popped from the stack, or is LDR Rx, [sp, #4]! when one register is
 * to be popped from the stack.
 * UAL syntax.
 */
bool
m_pop (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  return LoadStoreMultiple (cc, doLowerCase, eIsPop);
}


/**
 * Implements STM.
 */
bool
m_stm (bool doLowerCase)
{
  ARMWord cc = Option_CondLdmStm (false, doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  return LoadStoreMultiple (cc, doLowerCase, eIsSTM);
}


/**
 * Implements PUSH, i.e. STM<cond>FD sp!, {...} when more than one register is
 * to be pushed on the stack, or STR Rx, [sp], #-4 when one register is to be
 * pushed on the stack.
 * UAL syntax.
 */
bool
m_push (bool doLowerCase)
{
  ARMWord cc = Option_Cond (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  return LoadStoreMultiple (cc, doLowerCase, eIsPush);
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

  ARMWord cc_b = Option_CondB (doLowerCase);
  if (cc_b == kOption_NotRecognized)
    return true;

  Target_CheckCPUFeature (kCPUExt_v2a, true);
  if (Target_CheckCPUFeature (kCPUExt_v8, false))
    Error (ErrorWarning, "The use of SWP/SWPB is obsolete for ARMv8");
  else if (Target_CheckCPUFeature (kCPUExt_v6, false))
    Error (ErrorWarning, "The use of SWP/SWPB is deprecated for ARMv6 and ARMv7");

  IT_ApplyCond (cc_b, false);

  int ir = cc_b | 0x01000090;
  unsigned rt = Get_CPUReg ();
  ir |= DST_OP (rt);
  Input_SkipWS ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%sdst", InsertCommaAfter);
  unsigned rt2 = Get_CPUReg ();
  ir |= RHS_OP (rt2);	/* Note wrong order swp dst,rhs,[lsh] */
  Input_SkipWS ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%slhs", InsertCommaAfter);
  if (!Input_Match ('[', true))
    Error (ErrorError, "Inserting missing '['");
  unsigned rn = Get_CPUReg ();
  ir |= DST_MUL (rn);
  Input_SkipWS ();
  if (!Input_Match (']', true))
    Error (ErrorError, "Inserting missing ']'");

  if (rt == 15 || rt2 == 15 || rn == 15)
    Error (ErrorError, "SWP(B) registers can not be r15");
  if (rn == rt || rn == rt2)
    Error (ErrorError, "SWP(B) address register can not be the same as one of the swap registers");

  Put_Ins (4, ir);
  return false;
}

typedef enum
{
  BL_eSY = 0xF,
  BL_eST = 0xE,
  BL_eLD = 0xD,
  BL_eISH = 0xB,
  BL_eISHST = 0xA,
  BL_eISHLD = 0x9,
  BL_eNSH = 0x7,
  BL_eNSHST = 0x6,
  BL_eNSHLD = 0x5,
  BL_eOSH = 0x3,
  BL_eOSHST = 0x2,
  BL_eOSHLD = 0x1
} Barrier_eType;

static Barrier_eType
GetBarrierType (void)
{
  static const struct
    {
      const char *nameP;
      const char *prefNameP;
      Barrier_eType barrierType;
      unsigned cpuFeatures; /* Minimum CPU features needed.  */
    } oBarrierTypes[] =
    {
      { "sy", NULL, BL_eSY, kCPUExt_BARRIER },
      { "st", NULL, BL_eST, kCPUExt_BARRIER },
      { "ld", NULL, BL_eLD, kCPUExt_v8 },
      { "ish", NULL, BL_eISH, kCPUExt_BARRIER },
      { "sh", "ish", BL_eISH, kCPUExt_BARRIER },
      { "ishst", NULL, BL_eISHST, kCPUExt_BARRIER },
      { "shst", "ishst", BL_eISHST, kCPUExt_BARRIER },
      { "ishld", NULL, BL_eISHLD, kCPUExt_v8 },
      { "nsh", NULL, BL_eNSH, kCPUExt_BARRIER },
      { "un", "nsh", BL_eNSH, kCPUExt_BARRIER },
      { "nshst", NULL, BL_eNSHST, kCPUExt_BARRIER },
      { "unst", "nshst", BL_eNSHST, kCPUExt_BARRIER },
      { "nshld", NULL, BL_eNSHLD, kCPUExt_v8 },
      { "osh", NULL, BL_eOSH, kCPUExt_BARRIER },
      { "oshst", NULL, BL_eOSHST, kCPUExt_BARRIER },
      { "oshld", NULL, BL_eOSHLD, kCPUExt_v8 }
    };

  Input_SkipWS ();

  Barrier_eType barrierType = BL_eSY;
  unsigned cpuFeatures = kCPUExt_BARRIER;
  if (!Input_IsEolOrCommentStart ())
    {
      size_t i;
      for (i = 0; i != sizeof (oBarrierTypes)/sizeof (oBarrierTypes[0]); ++i)
	{
	  if (Input_MatchKeywordLower (oBarrierTypes[i].nameP))
	    {
	      if (option_pedantic && oBarrierTypes[i].prefNameP != NULL)
		Error (ErrorWarning, "Use barrier type %s instead of %s", oBarrierTypes[i].prefNameP, oBarrierTypes[i].nameP);
	      barrierType = oBarrierTypes[i].barrierType;
	      cpuFeatures = oBarrierTypes[i].cpuFeatures;
	      break;
	    }
	}
      if (i == sizeof (oBarrierTypes)/sizeof (oBarrierTypes[0]))
	Error (ErrorError, "Unknown barrier type");
    }
  Target_CheckCPUFeature (cpuFeatures, true);

  return barrierType;
}

/**
 * Implements DMB.
 * ARM DMB is unconditional.
 */
bool
m_dmb (void)
{
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
  Barrier_eType bl = GetBarrierType ();
  if (option_pedantic && bl != BL_eSY)
    Error (ErrorWarning, "Using reserved barrier type");
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

  unsigned regN = Get_CPUReg ();
  if (regN == INVALID_REG)
    return false;
  if (regN == 15)
    Error (ErrorError, "Using PC as base register is unpredictable");

  Input_SkipWS ();
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

  Input_SkipWS ();
  bool isUALSyntax, updateStack;
  if (!Input_Match ('#', false))
    {
      isUALSyntax = true;
      unsigned sp = Get_CPUReg ();
      if (sp == INVALID_REG)
	return false;
      if (sp != 13)
	{
	  Error (ErrorError, "SRS can only be used with stack register 13 (sp)");
	  return false;
	}
      Input_SkipWS ();
      updateStack = Input_Match ('!', true);
      if (!Input_Match (',', true))
	{
	  Error (ErrorError, "Missing ,");
	  return false;
	}
      if (!Input_Match ('#', false))
	{
	  Error (ErrorError, "%s needs a mode specified", "SRS");
	  return false;
	}
    }
  else
    isUALSyntax = false;

  const Value *im = Expr_BuildAndEval (ValueInt);
  if (im->Tag != ValueInt)
    {
      Error (ErrorError, "Illegal immediate expression");
      return false;
    }
  int mode = im->Data.Int.i;
  if (!Option_IsValidARMMode (mode))
    {
      Error (ErrorWarning, "Mode 0x%x is not a valid ARM mode", mode);
      mode &= 0x1F;
    }

  if (!isUALSyntax)
    {
      Input_SkipWS ();
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
