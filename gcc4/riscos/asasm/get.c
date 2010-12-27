/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2010 GCCSDK Developers
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

#include "error.h"
#include "expr.h"
#include "fix.h"
#include "get.h"
#include "help_cpu.h"
#include "input.h"
#include "lex.h"
#include "os.h"
#include "reloc.h"
#include "symbol.h"

static ARMWord
getTypeInternal (bool genError, unsigned int type, const char *typeStr)
{
  const Lex lexSym = genError ? lexGetId () : lexGetIdNoError ();
  if (lexSym.tag == LexId)
    {
      const Symbol *sym = symbolFind (&lexSym);
      if (sym && (sym->type & SYMBOL_DEFINED))
	{
	  if (SYMBOL_GETREGTYPE (sym->type) == type)
	    return sym->value.Data.Int.i;
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
  return getTypeInternal (true, SYMBOL_CPUREG, "CPU register");
}

ARMWord
Get_CPURegNoError (void)
{
  const char * const inputMark = Input_GetMark ();
  ARMWord reg = getTypeInternal (false, SYMBOL_CPUREG, "CPU register");
  if (reg == INVALID_REG)
    Input_RollBackToMark (inputMark);
  return reg;
}

ARMWord
getFpuReg (void)
{
  return getTypeInternal (true, SYMBOL_FPUREG, "FPU register");
}

ARMWord
getCopReg (void)
{
  return getTypeInternal (true, SYMBOL_COPREG, "coprocessor register");
}

ARMWord
getCopNum (void)
{
  return getTypeInternal (true, SYMBOL_COPNUM, "coprocessor number");
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
 *   2. LSL <Rs>           (not when immonly = false)
 *   3. LSR #<shift_imm>
 *   4. LSR <Rs>           (not when immonly = false)
 *   5. ASR #<shift_imm>
 *   6. ASR <Rs>           (not when immonly = false)
 *   7. ROR #<shift_imm>
 *   8. ROR <Rs>           (not when immonly = false)
 *   9. RRX
 */
static ARMWord
getShift (bool immonly)
{
  ARMWord op = 0;
  ARMWord shift = getShiftOp ();
  if (shift != RRX)
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
	  if (immonly)
	    error (ErrorError, "Only shift immediate allowed here");
	  else
	    op = SHIFT_REG (getCpuReg ()) | SHIFT_OP (shift);
	}
    }
  else
    op = SHIFT_OP (shift);
  return op;
}

/**
 * Parses Addressing Mode 1 - Data-processing operands
 * Parses the <shifter_operand> in
 * <opcode>{<cond>}{S} <Rd>, <Rn>, <shifter_operand>
 * With <shifter_operand>:
 *   1. #<immediate>
 *   2. <Rm>
 *   3. <Rm>, LSL #<shift_imm>
 *   4. <Rm>, LSL <Rs>
 *   5. <Rm>, LSR #<shift_imm>
 *   6. <Rm>, LSR <Rs>
 *   7. <Rm>, ASR #<shift_imm>
 *   8. <Rm>, ASR <Rs>
 *   9. <Rm>, ROR #<shift_imm>
 *  10. <Rm>, ROR <Rs>
 *  11. <Rm>, RRX
 */
ARMWord
getRhs (bool immonly, bool shift, ARMWord ir)
{
  if (Input_Match ('#', false))
    {
      ir |= IMM_RHS;
      const Value *im = exprBuildAndEval (ValueInt | ValueAddr | ValueString); /* FIXME: *** NEED ValueSymbol & ValueCode */
      switch (im->Tag)
	{
	  case ValueInt:
	    if (Input_Match (',', false))
	      {
		if (im->Data.Int.i < 0 || im->Data.Int.i >= 256)
		  error (ErrorError, "Immediate value out of range: 0x%x", im->Data.Int.i);

	        Lex rotator = lexGetPrim ();
	        if (rotator.tag != LexInt
	            || rotator.Data.Int.value < 0
	            || rotator.Data.Int.value > 30
	            || (rotator.Data.Int.value % 2) == 1)
		  error (ErrorError, "Bad rotator %d", rotator.Data.Int.value);

		ir |= (rotator.Data.Int.value >> 1) << 8;
	      }
	    /* Fall through.  */

	  case ValueAddr: /* This is for "MOV Rx, #@" support.  */
	    ir = fixImm8s4 (0, ir, im->Data.Int.i);
	    break;

	  case ValueString:
	    /* FIXME: Use Lex_Char2Int ? */
	    if (im->Data.String.len != 1)
	      error (ErrorError, "String too long to be an immediate expression");
	    else
	      ir = fixImm8s4 (0, ir, im->Data.String.s[0]);
	    break;

	  default:
	    error (ErrorError, "Illegal immediate expression");
	    break;
	}
    }
  else
    {
      ir |= getCpuReg ();
      skipblanks ();
      if (Input_Match (',', true))
	{
	  if (!shift)
	    return ir;		/* will cause a 'skip rest of line' error */
	  ir |= getShift (immonly);
	}
      else
	ir |= NO_SHIFT;
    }
  return ir;
}
