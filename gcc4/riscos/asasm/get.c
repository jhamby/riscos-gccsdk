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
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
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
getCpuRegInternal (bool genError)
{
  Lex lexSym = genError ? lexGetId () : lexGetIdNoError ();
  if (lexSym.tag == LexNone)
    return genError ? 0 : INVALID_REG;

  const Symbol *sym = symbolGet (&lexSym);
  if ((sym->type & SYMBOL_DEFINED) && (sym->type & SYMBOL_ABSOLUTE))
    {
      if (SYMBOL_GETREG (sym->type) == SYMBOL_CPUREG)
	return sym->value.Data.Int.i;
      if (genError)
	error (ErrorError, "'%s' is not a %s register", sym->str, "cpu");
    }
  else if (genError)
    error (ErrorError, "Undefined register %s", sym->str);
  return genError ? 0 : INVALID_REG;
}

ARMWord
getCpuReg (void)
{
  return getCpuRegInternal (true);
}

ARMWord
getCpuRegNoError (void)
{
  return getCpuRegInternal (false);
}

ARMWord
getFpuReg (void)
{
  Lex lexSym = lexGetId ();
  if (lexSym.tag == LexNone)
    return 0;

  Symbol *sym = symbolGet (&lexSym);
  if ((sym->type & SYMBOL_DEFINED) && (sym->type & SYMBOL_ABSOLUTE))
    {
      if (SYMBOL_GETREG (sym->type) == SYMBOL_FPUREG)
	return sym->value.Data.Int.i;
      error (ErrorError, "'%s' is not a %s register", sym->str, "fpu");
    }
  else
    error (ErrorError, "Undefined float register %s", sym->str);
  return 0;
}

ARMWord
getCopReg (void)
{
  Lex lexSym = lexGetId ();
  if (lexSym.tag == LexNone)
    return 0;

  Symbol *sym = symbolGet (&lexSym);
  if ((sym->type & SYMBOL_DEFINED) && (sym->type & SYMBOL_ABSOLUTE))
    {
      if (SYMBOL_GETREG (sym->type) == SYMBOL_COPREG)
	return sym->value.Data.Int.i;
      error (ErrorError, "'%s' is not a %s register", sym->str, "cop");
    }
  else
    error (ErrorError, "Undefined coprocessor register %s", sym->str);
  return 0;
}

ARMWord
getCopNum (void)
{
  Lex lexSym = lexGetId ();
  if (lexSym.tag == LexNone)
    return 0;

  Symbol *sym = symbolGet (&lexSym);
  if ((sym->type & SYMBOL_DEFINED) && (sym->type & SYMBOL_ABSOLUTE))
    {
      if (SYMBOL_GETREG (sym->type) == SYMBOL_COPNUM)
	return sym->value.Data.Int.i;
      error (ErrorError, "'%s' is not a coprocessor number", sym->str);
    }
  else
    error (ErrorError, "Undefined coprocessor number %s", sym->str);
  return 0;
}

static ARMWord
getShiftOp (void)
{
  ARMWord r = 0;
  switch (inputLookLower ())
    {
      case 'a':
	if (inputLookNLower (1) == 's')
	{
	  switch (inputLookNLower (2))
	    {
	      case 'l':
		r = ASL;
		inputSkipN (3);
		break;
	      case 'r':
		r = ASR;
		inputSkipN (3);
		break;
	      default:
		goto illegal;
	    }
	}
	else
	  goto illegal;
	break;
      case 'l':
	if (inputLookNLower (1) == 's')
	{
	  switch (inputLookNLower (2))
	    {
	      case 'l':
		r = LSL;
		inputSkipN (3);
		break;
	      case 'r':
		r = LSR;
		inputSkipN (3);
		break;
	      default:
		goto illegal;
	    }
	}
	else
	  goto illegal;
	break;
      case 'r':
	switch (inputLookNLower (1))
	{
	  case 'o':
	    if (inputLookNLower (2) == 'r')
	      {
		r = ROR;
		inputSkipN (3);
		break;
	      }
	    else
	      goto illegal;
	  case 'r':
	    if (inputLookNLower (2) == 'x')
	      {
		r = RRX;
		inputSkipN (2);
		inputSkip ();
		break;
	      }
	    else
	      goto illegal;
	  default:
	    goto illegal;
	}
	break;
      default:
illegal:
	error (ErrorError, "Illegal shiftop %c%c%c", inputLook (), inputLookN (1), inputLookN (2));
	break;
    }
  return r;
}


static ARMWord
getShift (bool immonly)
{
  ARMWord op = 0;
  ARMWord shift = getShiftOp ();
  if (shift != RRX)
    {
      skipblanks ();
      if (inputLook () == '#')
	{
	  inputSkip ();
	  exprBuild ();
	  Value im = exprEval (ValueInt | ValueCode | ValueLateLabel);
	  switch (im.Tag)
	    {
	    case ValueInt:
	      op = fixShiftImm (0, shift, im.Data.Int.i); /* !! Fixed !! */
	      break;
	    case ValueCode:
	    case ValueLateLabel:
	      relocShiftImm (shift, &im);
	      op = SHIFT_OP (shift); /* !! Fixed !! */
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

ARMWord
getRhs (bool immonly, bool shift, ARMWord ir)
{
  if (inputLook () == '#')
    {
      ir |= IMM_RHS;
      inputSkip ();
      exprBuild ();
      Value im = exprEval (ValueInt | ValueCode | ValueLateLabel | ValueString | ValueAddr);
      switch (im.Tag)
	{
	case ValueInt:
	  if (inputLook () == ',')
	    {
	      inputSkip ();
	      Lex rotator = lexGetPrim ();

	      if (im.Data.Int.i < 0 || im.Data.Int.i >= 256)
	        error (ErrorError, "Immediate value out of range: 0x%x", im.Data.Int.i);

	      if (rotator.Data.Int.value < 0 || rotator.Data.Int.value > 30
		  || (rotator.Data.Int.value % 2) == 1)
	        error (ErrorError, "Bad rotator %d", rotator.Data.Int.value);

	      ir |= (rotator.Data.Int.value >> 1) << 8;
	    }
	case ValueAddr:
	  ir = fixImm8s4 (0, ir, im.Data.Int.i);
	  break;
	case ValueString:
	  if (im.Data.String.len != 1)
	    error (ErrorError, "String too long to be an immediate expression");
	  else
	    ir = fixImm8s4 (0, ir, im.Data.String.s[0]);
	  break;
	case ValueCode:
	case ValueLateLabel:
	  relocImm8s4 (ir, &im);
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
      if (inputLook () == ',')
	{
	  if (!shift)
	    return ir;		/* will cause a 'skip rest of line' error */
	  inputSkip ();
	  skipblanks ();
	  ir |= getShift (immonly);
	}
      else
	ir |= NO_SHIFT;
    }
  return ir;
}
