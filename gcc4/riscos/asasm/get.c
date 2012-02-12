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
#include "lex.h"
#include "phase.h"
#include "reloc.h"
#include "symbol.h"

static ARMWord
GetRegisterValue (bool genError, RegType_e type, const char *typeStr)
{
  const Lex lexSym = genError ? lexGetId () : lexGetIdNoError ();
  if (lexSym.tag == LexId)
    {
      const Symbol *sym = symbolFind (&lexSym);
      if (sym && (sym->type & SYMBOL_DEFINED))
	{
	  if (sym->value.Tag == ValueRegister
	      && sym->value.Data.Register.type == type)
	    return sym->value.Data.Register.num;
	  if (genError)
	    error (ErrorError, "'%s' is not a %s", sym->str, typeStr);
	}
      else if (genError)
	error (ErrorError, "Undefined %s %.*s", typeStr,
	       (int)lexSym.Data.Id.len, lexSym.Data.Id.str);
    }

  return genError ? 0 : INVALID_REG;
}

ARMWord
getCpuReg (void)
{
  return GetRegisterValue (true, eRegType_CPU, "CPU register");
}

ARMWord
Get_CPURegNoError (void)
{
  const char * const inputMark = Input_GetMark ();
  ARMWord reg = GetRegisterValue (false, eRegType_CPU, "CPU register");
  if (reg == INVALID_REG)
    Input_RollBackToMark (inputMark);
  return reg;
}

ARMWord
getFpuReg (void)
{
  return GetRegisterValue (true, eRegType_FPU, "FPU register");
}

ARMWord
getCopReg (void)
{
  return GetRegisterValue (true, eRegType_CoProReg, "coprocessor register");
}

ARMWord
getCopNum (void)
{
  return GetRegisterValue (true, eRegType_CoProNum, "coprocessor number");
}

static ARMWord
getShiftOp (void)
{
  ARMWord r = 0;
  switch (inputLookLower ())
    {
      case 'a': /* ASL, ASR */
	{
	  if (inputLookNLower (1) != 's')
	    goto illegal;
	  switch (inputLookNLower (2))
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
	  inputSkipN (3);
	  break;
	}
      case 'l': /* LSL, LSR */
	{
	  if (inputLookNLower (1) != 's')
	    goto illegal;
	  switch (inputLookNLower (2))
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
	  inputSkipN (3);
	  break;
	}
      case 'r': /* ROR, RRX */
	{
	  switch (inputLookNLower (1))
	    {
	      case 'o':
		if (inputLookNLower (2) != 'r')
		  goto illegal;
		r = ROR;
		break;

	      case 'r':
		if (inputLookNLower (2) != 'x')
		  goto illegal;
		r = RRX;
		break;

	      default:
		goto illegal;
	    }
	  inputSkipN (3);
	  break;
	}

      default:
illegal:
	error (ErrorError, "Illegal shiftop %c%c%c", inputLook (), inputLookN (1), inputLookN (2));
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
      skipblanks ();
      if (Input_Match ('#', false))
	{
	  const Value *im = exprBuildAndEval (ValueInt);
	  switch (im->Tag)
	    {
	      case ValueInt:
		op = Fix_ShiftImm (NULL, 0, shift, im->Data.Int.i); /* !! Fixed !! */
		break;

	      default:
		error (ErrorError, "Illegal shift expression");
		break;
	    }
	}
      else
	{
	  if (regshift)
	    op = SHIFT_REG (getCpuReg ()) | SHIFT_OP (shift);
	  else
	    error (ErrorError, "Only shift immediate allowed here");
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
getRhs (bool regshift, bool shift, ARMWord ir)
{
  if (Input_Match ('#', false))
    {
      ir |= IMM_RHS;
      const Value *im = exprBuildAndEval (ValueInt | ValueAddr | ValueString); /* FIXME: *** NEED ValueSymbol & ValueCode */
      switch (im->Tag)
	{
	  case ValueInt:
	    {
	      const int imBase = im->Data.Int.i;
	      if (Input_Match (',', false))
		{
		  if (im->Data.Int.i < 0 || im->Data.Int.i >= 256)
		    error (ErrorError, "Immediate value out of range: 0x%x", im->Data.Int.i);

		  /* FIXME: why not call exprBuildAndEval instead ? vvv */
		  Lex rotator = lexGetPrim ();
		  int rotatorValue;
		  switch (rotator.tag)
		    {
		      case LexId:
			codeInit ();
			codeSymbol (symbolGet (&rotator), 0);
			const Value *rotatorResult = exprEval (ValueInt);
			if (rotatorResult->Tag != ValueInt)
			  {
			    error (ErrorError, "Illegal immediate expression");
			    rotatorValue = 0;
			  }
			else
			  rotatorValue = rotatorResult->Data.Int.i;
			break;
		      case LexInt:
			rotatorValue = rotator.Data.Int.value;
			break;
		      default:
			error (ErrorError, "Rotator is not an integer");
			rotatorValue = 0;
			break;
		    }
		  if (rotatorValue < 0 || rotatorValue > 30
		      || (rotatorValue % 2) == 1)
		    error (ErrorError, "Bad rotator %d", rotatorValue);

		  ir |= (rotatorValue >> 1) << 8;
	        }
	      ir = fixImm8s4 (0, ir, imBase);
	      break;
	    }

	  case ValueAddr: /* This is for "MOV Rx, #@" support.  */
	    ir = fixImm8s4 (0, ir, im->Data.Addr.i);
	    break;

	  case ValueString:
	    /* FIXME: Use Lex_Char2Int ? */
	    if (im->Data.String.len != 1)
	      error (ErrorError, "String too long to be an immediate expression");
	    else
	      ir = fixImm8s4 (0, ir, (uint8_t)im->Data.String.s[0]);
	    break;

	  default:
	    /* During pass one, we discard any errors of the evaluation as it
	       might contain unresolved symbols.  Wait until during pass two.  */
	    if (gPhase != ePassOne)
	      error (ErrorError, "Illegal immediate expression");
	    break;
	}
    }
  else
    {
      ir |= getCpuReg ();
      if (shift)
	{
	  skipblanks ();
	  if (Input_Match (',', true))
	    ir |= getShift (regshift);
	  else
	    ir |= NO_SHIFT;
	}
    }

  return ir;
}
