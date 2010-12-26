/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2010 GCCSDK Developersrs
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

#include "area.h"
#include "code.h"
#include "error.h"
#include "expr.h"
#include "expr.h"
#include "fix.h"
#include "get.h"
#include "global.h"
#include "help_cpu.h"
#include "input.h"
#include "lit.h"
#include "m_cpu.h"
#include "m_cpumem.h"
#include "option.h"
#include "put.h"
#include "targetcpu.h"

/**
 * Reloc updater for dstmem() for pre-increment based on symbols.
 */
static bool
DestMem_RelocUpdater (const char *file, int lineno, ARMWord offset,
		      const Value *valueP,
		      void *privData __attribute__ ((unused)), bool final)
{
  ARMWord ir = GetWord (offset);
  bool isAddrMode3 = (ir & 0x04000090) == 0x90;

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
	    {
	      /* This can only happen when "LDR Rx, =<constant>" can be turned into
		 MOV/MVN Rx, #<constant>.  */
	      if (valueP->Data.Code.len != 1)
		return true;
	      ARMWord newIR = ir & NV;
	      newIR |= DST_OP (GET_DST_OP (ir));
	      ARMWord im;
	      if ((im = help_cpuImm8s4 (valP->Data.Int.i)) != -1)
		newIR |= M_MOV | IMM_RHS | im;
	      else if ((im = help_cpuImm8s4 (~valP->Data.Int.i)) != -1)
		newIR |= M_MVN | IMM_RHS | im;
	      else if (areaCurrentSymbol->area.info->type & AREA_ABS)
		{
		  ARMWord newOffset = valP->Data.Addr.i - (areaCurrentSymbol->area.info->baseAddr + offset + 8);
		  ir |= LHS_OP (15);
		  if (isAddrMode3)
		    ir |= B_FLAG;
		  newIR = Fix_CPUOffset (file, lineno, ir, newOffset);
		}
	      else
		return true;
	      Put_InsWithOffset (offset, newIR);
	    }
	    break;

	  case ValueAddr:
	    {
	      ir |= LHS_OP (valP->Data.Addr.r);
	      if (isAddrMode3)
		ir |= B_FLAG;
	      ir = Fix_CPUOffset (file, lineno, ir, valP->Data.Addr.i);
	      Put_InsWithOffset (offset, ir);
	    }
	    break;

	  case ValueSymbol:
	    if (!final)
	      return true;
	    if (Reloc_Create (HOW2_INIT | HOW2_WORD, offset, valP) == NULL)
	      return true;
	    break;

	  default:
	    return true;
	}
    }

  return false;
}

/**
 * Parses Address mode 2 and 3.
 */
static bool
dstmem (ARMWord ir, const char *mnemonic)
{
  bool isAddrMode3;
  if ((ir & 0x04000090) == 0x90)
    {
      if (!cpuWarn (ARM7M) && (ir & H_FLAG) && targetCPU < ARM10)
	error (ErrorWarning, "Half-word ops only work correctly when accessed location is cached");
      isAddrMode3 = true;
    }
  else
    isAddrMode3 = false;
  int dst = getCpuReg ();
  ir |= DST_OP (dst);
  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%sdst", InsertCommaAfter);
  switch (inputLook ())
    {
      case '[':	/* LDR <reg>, [ */
	{
	  inputSkip ();
	  bool trans = false; /* FIXME: wtf ? */
	  skipblanks ();
	  int op = getCpuReg ();	/* Base register */
	  ir |= LHS_OP (op);
	  skipblanks ();
	  bool pre = !Input_Match (']', true);
	  bool offValue = false;
	  Value symValue = { .Tag = ValueIllegal };
	  if (Input_Match (',', true))
	    { /* either [base,XX] or [base],XX */
	      if (Input_Match ('#', false))
		{
		  if (isAddrMode3)
		    ir |= B_FLAG;  /* Immediate mode for addr. mode 3.  */

		  const Value *offset = exprBuildAndEval (ValueInt | ValueSymbol | ValueCode);
		  switch (offset->Tag)
		    {
		      case ValueInt:
			ir = Fix_CPUOffset (NULL, 0, ir, offset->Data.Int.i);
			break;

		      case ValueSymbol:
		      case ValueCode:
			symValue = *offset;
			break;
			
		      default:
			error (ErrorError, "Illegal offset expression");
			break;
		    }

		  /* U_FLAG is fixed in Fix_CPUOffset() */
		  offValue = true;
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
		  ir |= isAddrMode3 ? 0 : REG_FLAG;
		  ir = getRhs (true, !isAddrMode3, ir);
		  offValue = true;
		}
	      skipblanks ();
	    }
	  else
	    {			/* [base] if this way */
	      ir |= U_FLAG;	/* 0 nicer than -0; pre nicer than post */
	      if (isAddrMode3)
		ir |= B_FLAG;	/* Immediate mode for addr. mode 3. */
	      if (pre)
		error (ErrorError, "Illegal character '%c' after base", inputLook ());
	    }
	  if (pre)
	    {
	      if (!Input_Match (']', true))
		error (ErrorError, "Inserting missing ] after address");
	    }
	  else
	    {
	      /* If offset value was never set, then make it a pre-index load */
	      if (!offValue)
	        pre = true;
	      else if (dst == op)
	        error (ErrorError, "Post increment is not sane where base and destination register are the same");
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
	      ir |= P_FLAG;
	      if (trans)
	        error (ErrorError, "Translate not allowed with pre-index");
	    }

	  codeInit ();
	  codeValue (&symValue, true);
	  const ARMWord offset = areaCurrentSymbol->value.Data.Int.i;
	  Put_Ins (ir);
	  if (symValue.Tag != ValueIllegal
	      && Reloc_QueueExprUpdate (DestMem_RelocUpdater, offset,
					ValueInt | ValueAddr | ValueSymbol | ValueCode, NULL, 0))
	    error (ErrorError, "Illegal %s expression", mnemonic);
	}
        break;

      case '=': /* LDR* <reg>, =<expression> */
	{
	  inputSkip ();
	  /* Is LDRD ? */
	  if ((ir & 0x0E1000D0) == 0xD0)
	    errorAbort ("No support for LDRD and literal");
	  /* This is only allowed for LDR.  */
	  else if ((ir & L_FLAG) == 0)
	    errorAbort ("You can't store into a constant");
	  /* The ValueInt | ValueSymbol | ValueCode tags are what we support
	     as constants from user point of view.  */
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
	  Value value = Lit_RegisterInt (exprBuildAndEval (ValueInt | ValueSymbol | ValueCode), litSize);
	  codeInit ();
	  codeValue (&value, true);
	  valueFree (&value);
	  const ARMWord offset = areaCurrentSymbol->value.Data.Int.i;
	  Put_Ins (ir | P_FLAG);
	  /* The ValueInt | ValueAddr | ValueSymbol | ValueCode tags are what we
	     support in the LDR instruction.  When we have ValueInt it is
	     garanteed to be a valid immediate.  */
	  if (Reloc_QueueExprUpdate (DestMem_RelocUpdater, offset,
				     ValueInt | ValueAddr | ValueSymbol | ValueCode, NULL, 0))
	    error (ErrorError, "Illegal %s expression", mnemonic);
	}
	break;

      default: /* LDR* <reg>, <label>  */
	{
	  /* We're dealing with one of the following:
	   *
	   * 1) a PC-relative label
	   * 2) a field in a register-based map
	   * 3) a label in a register-based area
	   */
	  exprBuild ();
	  const ARMWord offset = areaCurrentSymbol->value.Data.Int.i;
	  /* Whatever happens, this must be a pre-increment.  */
	  Put_Ins (ir | P_FLAG);
	  if (Reloc_QueueExprUpdate (DestMem_RelocUpdater, offset,
				     ValueInt | ValueAddr | ValueSymbol | ValueCode, NULL, 0))
	    error (ErrorError, "Illegal %s expression", mnemonic);
	}
	break;
    }
  return false;
}
	     

/**
 * Implements LDR:
 *   LDR[<cond>] <Rd>, <address mode 2> | <pc relative label>
 *   LDR[<cond>]T <Rd>, <address mode 2> | <pc relative label>
 *   LDR[<cond>]B <Rd>, <address mode 2> | <pc relative label>
 *   LDR[<cond>]BT <Rd>, <address mode 2> | <pc relative label>
 *   LDR[<cond>]D <Rd>, <address mode 3> | <pc relative label>
 *   LDR[<cond>]H <Rd>, <address mode 3> | <pc relative label>
 *   LDR[<cond>]SB <Rd>, <address mode 3> | <pc relative label>
 *   LDR[<cond>]SH <Rd>, <address mode 3> | <pc relative label>
 */
bool
m_ldr (void)
{
  ARMWord cc = optionCondBT (false);
  if (cc == optionError)
    return true;
  return dstmem (cc, "LDR");
}

#if 0
// FIXME:
/**
 * Implements LDREX.
 *   LDREX[<cond>] <Rd>, [<Rn>]
 */
bool
m_ldrex (void)
{
}
#endif

/**
 * Implements STR<cond>[B].
 *   STR[<cond>] <Rd>, <address mode 2> | <pc relative label>
 *   STR[<cond>]T <Rd>, <address mode 2> | <pc relative label>
 *   STR[<cond>]B <Rd>, <address mode 2> | <pc relative label>
 *   STR[<cond>]BT <Rd>, <address mode 2> | <pc relative label>
 *   STR[<cond>]D <Rd>, <address mode 3> | <pc relative label>
 *   STR[<cond>]H <Rd>, <address mode 3> | <pc relative label>
 *   STR[<cond>]SB <Rd>, <address mode 3> | <pc relative label>
 *   STR[<cond>]SH <Rd>, <address mode 3> | <pc relative label>
 */
bool
m_str (void)
{
  ARMWord cc = optionCondBT (true);
  if (cc == optionError)
    return true;
  return dstmem (cc, "LDR");
}

#if 0
// FIXME:
/**
 * Implements STREX.
 *   STREX[<cond>] <Rd>, [<Rn>]
 */
bool
m_strex (void)
{
}
#endif

/**
 * Implements PLD.
 */
bool
m_pld (void)
{
  ARMWord ir = 0xf450f000 | P_FLAG;

  cpuWarn (XSCALE);

  skipblanks();

  if (!Input_Match ('[', true))
    error (ErrorError, "Expected '[' after PLD instruction");

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
	  ir = getRhs (true, true, ir) | REG_FLAG;
	}

      if (!Input_Match (']', true))
	error (ErrorError, "Expected closing ]");
    }
  Put_Ins(ir);
  return false;
}


static void
dstreglist (ARMWord ir, bool isPushPop)
{
  int op;
  if (isPushPop)
    {
      ir |= BASE_MULTI (13);
      ir |= W_FLAG;
    }
  else
    {
      op = getCpuReg ();
      ir |= BASE_MULTI (op);
      skipblanks ();
      if (Input_Match ('!', true))
	ir |= W_FLAG;
      if (!Input_Match (',', true))
	error (ErrorError, "Inserting missing comma before reglist");
    }
  if (!Input_Match ('{', true))
    error (ErrorError, "Inserting missing '{' before reglist");
  op = 0;
  do
    {
      int low = getCpuReg ();
      skipblanks ();
      int high;
      switch (inputLook ())
	{
	  case '-':
	    inputSkip ();
	    skipblanks ();
	    high = getCpuReg ();
	    skipblanks ();
	    if (low > high)
	      {
		error (ErrorInfo, "Register interval in wrong order r%d-r%d", low, high);
		int c = low;
		low = high;
		high = c;
	      }
	    break;
	  case ',':
	  case '}':
	    high = low;
	    break;
	  default:
	    error (ErrorError, "Illegal character '%c' in register list", inputLook ());
	    high = 15;
	    break;
        }
      if ((1 << low) < op)
	error (ErrorInfo, "Registers in wrong order");
      if (((1 << (high + 1)) - (1 << low)) & op)
	error (ErrorInfo, "Register occurs more than once in register list");
      op |= (1 << (high + 1)) - (1 << low);
    } while (Input_Match (',', true));
  if (!Input_Match ('}', false))
    error (ErrorError, "Inserting missing '}' after reglist");
  ir |= op;
  skipblanks ();
  if (Input_Match ('^', true))
    {
      if ((ir & W_FLAG) && !(ir & (1 << 15)))
	error (ErrorInfo, "Writeback together with force user");
      ir |= FORCE_FLAG;
    }
  Put_Ins (ir);
}


/**
 * Implements LDM.
 */
bool
m_ldm (void)
{
  ARMWord cc = optionCondLdmStm (true);
  if (cc == optionError)
    return true;
  dstreglist (cc | 0x08100000, false);
  return false;
}


/**
 * Implements POP, i.e. LDM<cond>FD sp!, {...}
 * (= LDM<cond>IA sp!, {...})
 */
bool
m_pop (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;
  dstreglist (cc | IA | 0x08100000, true);
  return false;
}


/**
 * Implements STM.
 */
bool
m_stm (void)
{
  ARMWord cc = optionCondLdmStm (false);
  if (cc == optionError)
    return true;
  dstreglist (cc | 0x08000000, false);
  return false;
}


/**
 * Implements PUSH, i.e. STM<cond>FD sp!, {...}
 * (= STM<cond>DB sp!, {...})
 */
bool
m_push (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;
  dstreglist (cc | DB | 0x08000000, true);
  return false;
}


/**
 * Implements SWP.
 */
bool
m_swp (void)
{
  ARMWord cc = optionCondB ();
  if (cc == optionError)
    return true;
  int ir = cc | 0x01000090;
  cpuWarn (ARM250);
  ir |= DST_OP (getCpuReg ());
  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%sdst", InsertCommaAfter);
  ir |= RHS_OP (getCpuReg ());	/* Note wrong order swp dst,rhs,[lsh] */
  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%slhs", InsertCommaAfter);
  if (!Input_Match ('[', true))
    error (ErrorError, "Inserting missing '['");
  ir |= DST_MUL (getCpuReg ());
  skipblanks ();
  if (!Input_Match (']', true))
    error (ErrorError, "Inserting missing ']'");
  Put_Ins (ir);
  return false;
}
