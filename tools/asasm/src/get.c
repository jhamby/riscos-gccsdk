/*
 * AS an assembler for ARM
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * get.c
 */

#include "config.h"
#include <string.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif

#include "code.h"
#include "error.h"
#include "expr.h"
#include "fix.h"
#include "get.h"
#include "help_cpu.h"
#include "input.h"
#include "main.h"
#include "lex.h"
#include "phase.h"
#include "reloc.h"
#include "symbol.h"

static unsigned
GetRegisterValue (bool genError, IntType_e type, const char *typeStr)
{
  const Lex lexSym = genError ? Lex_GetID () : Lex_GetIDNoError ();
  if (lexSym.tag == LexId)
    {
      const Symbol *sym = Symbol_Find (&lexSym);
      if (sym && (sym->type & SYMBOL_DEFINED))
	{
	  if (sym->value.Tag == ValueInt
	      && sym->value.Data.Int.type == type)
	    return sym->value.Data.Int.i;
	  if (genError)
	    Error (ErrorError, "'%s' is not a %s", sym->str, typeStr);
	}
      else if (genError)
	Error (ErrorError, "Undefined %s %.*s", typeStr,
	       (int)lexSym.Data.Id.len, lexSym.Data.Id.str);
    }

  return genError ? 0 : INVALID_REG;
}

unsigned
Get_CPUReg (void)
{
  return GetRegisterValue (true, eIntType_CPU, "CPU register");
}

unsigned
Get_CPURegNoError (void)
{
  const char * const inputMark = Input_GetMark ();
  unsigned reg = GetRegisterValue (false, eIntType_CPU, "CPU register");
  if (reg == INVALID_REG)
    Input_RollBackToMark (inputMark);
  return reg;
}

unsigned
Get_FPUReg (void)
{
  return GetRegisterValue (true, eIntType_FPU, "FPU register");
}

unsigned
Get_CopReg (void)
{
  return GetRegisterValue (true, eIntType_CoProReg, "coprocessor register");
}

unsigned
Get_CopNum (void)
{
  return GetRegisterValue (true, eIntType_CoProNum, "coprocessor number");
}

/**
 * Parses a register list:
 *  "{" <regs> "}"
 *     <regs> := <regs> "," <regs>
 *               <reg>
 *               <reg> "-" <reg>
 *     <reg> := <CPU register>
 * \return Bitwise or of specified CPU register numbers.
 */
ARMWord
Get_CPURList (void)
{
  if (!Input_Match ('{', true))
    Error (ErrorError, "Missing '{' before reglist");
  ARMWord regList = 0;
  do
    {
      unsigned low = Get_CPUReg ();
      Input_SkipWS ();
      unsigned high;
      switch (Input_Look ())
	{
	  case '-':
	    Input_Skip ();
	    high = Get_CPUReg ();
	    Input_SkipWS ();
	    if (low > high)
	      {
		if (option_pedantic)
		  Error (ErrorInfo, "Register interval in wrong order r%d-r%d", low, high);
		ARMWord c = low;
		low = high;
		high = c;
	      }
	    break;

	  case ',':
	  case '}':
	    high = low;
	    break;

          default:
	    Error (ErrorError, "Illegal character '%c' in register list", Input_Look ());
	    high = 15;
	    break;
        }
      if (option_pedantic && (1U << low) < regList)
	Error (ErrorInfo, "Registers in wrong order");
      if (((1U << (high + 1)) - (1U << low)) & regList)
	Error (ErrorInfo, "Register occurs more than once in register list");
      regList |= (1U << (high + 1)) - (1U << low);
    }
  while (Input_Match (',', true));
  if (!Input_Match ('}', false))
    Error (ErrorError, "Inserting missing '}' after reglist");
  return regList;
}


static ARMWord
getShiftOp (void)
{
  ARMWord r = 0;
  switch (Input_LookLower ())
    {
      case 'a': /* ASL, ASR */
	{
	  if (Input_LookNLower (1) != 's')
	    goto illegal;
	  switch (Input_LookNLower (2))
	    {
	      case 'l':
		r = ASL;
		break;

	      case 'r':
		r = ASR;
		break;

	      default:
		goto illegal;
	    }
	  Input_SkipN (3);
	  break;
	}
      case 'l': /* LSL, LSR */
	{
	  if (Input_LookNLower (1) != 's')
	    goto illegal;
	  switch (Input_LookNLower (2))
	    {
	      case 'l':
		r = LSL;
		break;

	      case 'r':
		r = LSR;
		break;

	      default:
		goto illegal;
	    }
	  Input_SkipN (3);
	  break;
	}
      case 'r': /* ROR, RRX */
	{
	  switch (Input_LookNLower (1))
	    {
	      case 'o':
		if (Input_LookNLower (2) != 'r')
		  goto illegal;
		r = ROR;
		break;

	      case 'r':
		if (Input_LookNLower (2) != 'x')
		  goto illegal;
		r = RRX;
		break;

	      default:
		goto illegal;
	    }
	  Input_SkipN (3);
	  break;
	}

      default:
illegal:
	Error (ErrorError, "Illegal shiftop %c%c%c", Input_Look (), Input_LookN (1), Input_LookN (2));
	break;
    }

  return r;
}


/**
 * Parses:
 *   1. LSL #<shift_imm>
 *   2. LSL <Rs>           (only when regshift = true)
 *   3. LSR #<shift_imm>
 *   4. LSR <Rs>           (only when regshift = true)
 *   5. ASR #<shift_imm>
 *   6. ASR <Rs>           (only when regshift = true)
 *   7. ROR #<shift_imm>
 *   8. ROR <Rs>           (only when regshift = true)
 *   9. RRX
 */
static ARMWord
getShift (bool regshift)
{
  ARMWord op = 0;
  ARMWord shift = getShiftOp ();
  if (shift == RRX)
    op = SHIFT_OP (shift);
  else
    {
      Input_SkipWS ();
      if (Input_Match ('#', false))
	{
	  const Value *im = Expr_BuildAndEval (ValueInt);
	  switch (im->Tag)
	    {
	      case ValueInt:
		op = Fix_ShiftImm (shift, im->Data.Int.i); /* !! Fixed !! */
		break;

	      default:
		Error (ErrorError, "Illegal shift expression");
		break;
	    }
	}
      else
	{
	  if (regshift)
	    op = SHIFT_REG (Get_CPUReg ()) | SHIFT_OP (shift);
	  else
	    Error (ErrorError, "Only shift immediate allowed here");
	}
    }

  return op;
}

/**
 * Parses Addressing Mode 1 - Data-processing operands
 * Parses the <shifter_operand> in
 * <opcode>{<cond>}{S} <Rd>, <Rn>, <shifter_operand>
 * With <shifter_operand>:
 *   1. #<immediate>
 *   2. <Rm>
 *   3. <Rm>, LSL #<shift_imm>   (only when shift = true)
 *   4. <Rm>, LSL <Rs>           (only when shift = true and regshift = true)
 *   5. <Rm>, LSR #<shift_imm>   (only when shift = true)
 *   6. <Rm>, LSR <Rs>           (only when shift = true and regshift = true)
 *   7. <Rm>, ASR #<shift_imm>   (only when shift = true)
 *   8. <Rm>, ASR <Rs>           (only when shift = true and regshift = true)
 *   9. <Rm>, ROR #<shift_imm>   (only when shift = true)
 *  10. <Rm>, ROR <Rs>           (only when shift = true and regshift = true)
 *  11. <Rm>, RRX                (only when shift = true)
 */
ARMWord
Get_RHS (bool regshift, bool shift, ARMWord ir)
{
  if (Input_Match ('#', false))
    {
      ir |= IMM_RHS;
      const Value *im = Expr_BuildAndEval (ValueInt | ValueAddr | ValueString); /* FIXME: *** NEED ValueSymbol & ValueCode */
      switch (im->Tag)
	{
	  case ValueAddr: /* This is for "MOV Rx, #@" support.  */
	    ir = Fix_Imm8s4 (ir, im->Data.Addr.i);
	    break;

	  case ValueString: /* FIXME: Remove ValueString case, cfr. Code_EvalLow() handling this.  */
	    if (im->Data.String.len != 1)
	      Error (ErrorError, "String too long to be an immediate expression");
	    else
	      ir = Fix_Imm8s4 (ir, (uint8_t)im->Data.String.s[0]);
	    break;

	  case ValueInt:
	    if (im->Data.Int.type == eIntType_PureInt)
	      {
	        const int imBase = im->Data.Int.i;
		if (Input_Match (',', false))
		  {
		    if (im->Data.Int.i < 0 || im->Data.Int.i >= 256)
		      Error (ErrorError, "Immediate value out of range: 0x%x", im->Data.Int.i);

		    uint32_t rotatorResult = 0;
		    const Value *rotatorValue = Expr_BuildAndEval (ValueInt);
		    switch (rotatorValue->Tag)
		      {
			case ValueInt:
			  if (rotatorValue->Data.Int.type == eIntType_PureInt)
			    {
			      rotatorResult = rotatorValue->Data.Int.i;
			      break;
			    }
			  /* Fall through.  */
			default:
			  Error (ErrorError, "Illegal immediate expression");
			  break;
		      }
		    if (rotatorResult > 30 || (rotatorResult % 2) == 1)
		      Error (ErrorError, "Bad rotator %d", rotatorResult);

		    ir |= (rotatorResult >> 1) << 8;
		  }
	        ir = Fix_Imm8s4 (ir, imBase);
	        break;
	      }
	    /* Fall through.  */

	  default:
	    /* During pass one, we discard any errors of the evaluation as it
	       might contain unresolved symbols.  Wait until during pass two.  */
	    if (gPhase != ePassOne)
	      Error (ErrorError, "Illegal immediate expression");
	    break;
	}
    }
  else
    {
      ir |= Get_CPUReg ();
      if (shift)
	{
	  Input_SkipWS ();
	  if (Input_Match (',', true))
	    ir |= getShift (regshift);
	  else
	    ir |= NO_SHIFT;
	}
    }

  return ir;
}
