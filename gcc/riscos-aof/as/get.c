/*
 * AS an assembler for ARM
 * Copyright © 1992 Niklas Röjemo
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

struct reg_id
  {
    const char *name;
    int reg_no;
    int len;
  };

WORD
getCpuReg (void)
{
  static const struct reg_id cpu_regs[] =
  {
    { "r0", 0, 2 },  { "r1", 1, 2 },   { "r2", 2, 2 },   { "r3", 3, 2 },
    { "r4", 4, 2 },  { "r5", 5, 2 },   { "r6", 6, 2 },   { "r7", 7, 2 },
    { "r8", 8, 2 },  { "r9", 9, 2 },   { "r10", 10, 3 }, { "r11", 11, 3 },
    { "r12", 12, 3 },{ "r13", 13, 3 }, { "r14", 14, 3 }, { "r15", 15, 3 },
    { "a1", 0, 2 },  { "a2", 1, 2  },  { "a3", 2, 2 },   { "a4", 3, 2 },
    { "v1", 4, 2 },  { "v2", 5, 2  },  { "v3", 6, 2 },   { "v4", 7, 2 },  { "v5", 8, 2  }, { "v6", 9, 2 },
    { "sl", 10, 2 }, { "fp", 11, 2 },  { "ip", 12, 2 },  { "sp", 13, 2 }, { "lr", 14, 2 }, {"pc", 15, 2 },
    { "rfp", 9, 3 }, { "sb", 9, 2 }
  };

  Lex lexSym;
  unsigned int loop;
  Symbol *sym;

  lexSym = lexGetId ();
  if (lexSym.tag == LexNone)
    return 0;

  if (lexSym.tag == LexId)
    for (loop = 0; loop < sizeof (cpu_regs) / sizeof (struct reg_id); loop++)
      {
	if (cpu_regs[loop].len == lexSym.LexId.len
	    && !strncasecmp (cpu_regs[loop].name, lexSym.LexId.str, lexSym.LexId.len))
	  return cpu_regs[loop].reg_no;
      }

  sym = symbolGet (lexSym);

  if (sym->type & SYMBOL_DEFINED && sym->type & SYMBOL_ABSOLUTE)
    {
      if (SYMBOL_GETREG (sym->type) == SYMBOL_CPUREG)
	return sym->value.ValueInt.i;
      else
	error (ErrorError, TRUE, "'%s' (=%d) is not a %s register", sym->str, sym->value, "cpu");
    }
  else
    error (ErrorError, TRUE, "Undefined register %s", sym->str);
  return 0;
}

WORD
getFpuReg (void)
{
  /* NB since len is fixed at 2, we don't actually use the length */
  static const struct reg_id cpu_regs[8] =
  {{ "f0", 0, 2 }, { "f1", 1, 2 }, { "f2", 2, 2 }, { "f3", 3, 2 },
   { "f4", 4, 2 }, { "f5", 5, 2 }, { "f6", 6, 2 }, { "f7", 7, 2 }};

  Lex lexSym;
  Symbol *sym;
  unsigned int loop;


  lexSym = lexGetId ();
  if (lexSym.tag == LexNone)
    return 0;

  if (lexSym.tag == LexId && lexSym.LexId.len == 2)
    for (loop = 0; loop < sizeof (cpu_regs) / sizeof (struct reg_id); loop++)
      {
	if ((cpu_regs[loop].len == lexSym.LexId.len) &&
	 !strncasecmp (cpu_regs[loop].name, lexSym.LexId.str, lexSym.LexId.len))
	  return cpu_regs[loop].reg_no;
      }

  sym = symbolGet (lexSym);


  if (sym->type & SYMBOL_DEFINED && sym->type & SYMBOL_ABSOLUTE)
    {
      if (SYMBOL_GETREG (sym->type) == SYMBOL_FPUREG)
	return sym->value.ValueInt.i;
      else
	error (ErrorError, TRUE, "'%s' (=%d) is not a %s register", sym->str, sym->value, "fpu");
    }
  else
    error (ErrorError, TRUE, "Undefined float register %s", sym->str);
  return 0;
}

WORD
getCopReg (void)
{
  static const struct reg_id cpu_regs[] =
  {
    { "c0", 0, 2 },  { "c1", 1, 2 },   { "c2", 2, 2 },   { "c3", 3, 2 },
    { "c4", 4, 2 },  { "c5", 5, 2 },   { "c6", 6, 2 },   { "c7", 7, 2 },
    { "c8", 8, 2 },  { "c9", 9, 2 },   { "c10", 10, 3 }, { "c11", 11, 3 },
    { "c12", 12, 3 },{ "c13", 13, 3 }, { "c14", 14, 3 }, { "c15", 15, 3 }
  };

  unsigned int loop;
  Symbol *sym;
  Lex lexSym = lexGetId ();

  if (lexSym.tag == LexNone)
    return 0;

  if (lexSym.tag == LexId)
    for (loop = 0; loop < sizeof (cpu_regs) / sizeof (struct reg_id); loop++)
      {
	if ((cpu_regs[loop].len == lexSym.LexId.len) &&
	 !strncasecmp (cpu_regs[loop].name, lexSym.LexId.str, lexSym.LexId.len))
	  return cpu_regs[loop].reg_no;
      }

  sym = symbolGet (lexSym);

  if (sym->type & SYMBOL_DEFINED && sym->type & SYMBOL_ABSOLUTE)
    {
      if (SYMBOL_GETREG (sym->type) == SYMBOL_COPREG)
	return sym->value.ValueInt.i;
      else
	error (ErrorError, TRUE, "'%s' (=%d) is not a %s register", sym->str, sym->value, "cop");
    }
  else
    error (ErrorError, TRUE, "Undefined coprocessor register %s", sym->str);
  return 0;
}

WORD
getCopNum (void)
{
  static const struct reg_id cop_nums[] =
  {
    { "p0", 0, 2 },  { "p1", 1, 2 },   { "p2", 2, 2 },   { "p3", 3, 2 },
    { "p4", 4, 2 },  { "p5", 5, 2 },   { "p6", 6, 2 },   { "p7", 7, 2 },
    { "p8", 8, 2 },  { "p9", 9, 2 },   { "p10", 10, 3 }, { "p11", 11, 3 },
    { "p12", 12, 3 },{ "p13", 13, 3 }, { "p14", 14, 3 }, { "p15", 15, 3 }
  };

  unsigned int loop;
  Symbol *sym;
  Lex lexSym = lexGetId ();

  if (lexSym.tag == LexNone)
    return 0;

  if (lexSym.tag == LexId)
    for (loop = 0; loop < sizeof (cop_nums) / sizeof (struct reg_id); loop++)
      {
	if ((cop_nums[loop].len == lexSym.LexId.len) &&
	 !strncasecmp (cop_nums[loop].name, lexSym.LexId.str, lexSym.LexId.len))
	  return cop_nums[loop].reg_no;
      }

  sym = symbolGet (lexSym);

  if (sym->type & SYMBOL_DEFINED && sym->type & SYMBOL_ABSOLUTE)
    {
      if (SYMBOL_GETREG (sym->type) == SYMBOL_COPNUM)
	return sym->value.ValueInt.i;
      else
	error (ErrorError, TRUE, "'%s' (=%d) is not a coprocessor number", sym->str, sym->value);
    }
  else
    error (ErrorError, TRUE, "Undefined coprocessor number %s", sym->str);
  return 0;
}

static WORD
getShiftOp (void)
{
  WORD r = 0;
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
      error (ErrorError, TRUE, "Illegal shiftop %c%c%c", inputLook (), inputLookN (1), inputLookN (2));
      break;
    }
  return r;
}


static WORD
getShift (BOOL immonly)
{
  WORD shift;
  WORD op = 0;
  Value im;
  shift = getShiftOp ();

  if (shift != RRX)
    {
      skipblanks ();
      if (inputLook () == '#')
	{
	  inputSkip ();
	  exprBuild ();
	  im = exprEval (ValueInt | ValueCode | ValueLateLabel);
	  switch (im.Tag.t)
	    {
	    case ValueInt:
	      op = fixShiftImm (inputLineNo, shift, im.ValueInt.i);	/* !! Fixed !! */
	      break;
	    case ValueCode:
	    case ValueLateLabel:
	      relocShiftImm (shift, im);
	      op = SHIFT_OP (shift);	/* !! Fixed !! */
	      break;
	    default:
	      error (ErrorError, TRUE, "Illegal shift expression");
	      break;
	    }
	}
      else
	{
	  if (immonly)
	    {
	      error (ErrorError, TRUE, "Only shift immediate allowed here");
	    }
	  else
	    {
	      op = SHIFT_REG (getCpuReg ()) | SHIFT_OP (shift);
	    }
	}
    }
  else
    op = SHIFT_OP (shift);
  return op;
}

WORD
getRhs (BOOL immonly, BOOL shift, WORD ir)
{
  Value im;
  if (inputLook () == '#')
    {
      ir |= IMM_RHS;
      inputSkip ();
      exprBuild ();
      im = exprEval (ValueInt | ValueCode | ValueLateLabel | ValueString | ValueAddr);
      switch (im.Tag.t)
	{
	case ValueInt:
	  if (inputLook () == ',')
	    {
	      Lex shift;
	  
	      inputSkip ();
	      shift = lexGetPrim ();
	  
	      if (im.ValueInt.i < 0 || im.ValueInt.i >= 256)
	        error (ErrorError, TRUE, "Immediate value out of range: 0x%x", im.ValueInt.i);
	  
	      if (shift.LexInt.value < 0 || shift.LexInt.value > 30 || (shift.LexInt.value % 2) == 1)
	        error (ErrorError, TRUE, "Bad rotator %d", shift.LexInt.value);
	  
	      ir |= (shift.LexInt.value >> 1) << 8;
	    }
	case ValueAddr:
	  ir = fixImm8s4 (inputLineNo, ir, im.ValueInt.i);
	  break;
	case ValueString:
	  if (im.ValueString.len != 1)
	    error (ErrorError, TRUE, "String too long to be an immediate expression");
	  else
	    ir = fixImm8s4 (inputLineNo, ir, im.ValueString.s[0]);
	  break;
	case ValueCode:
	case ValueLateLabel:
	  relocImm8s4 (ir, im);
	  break;
	default:
	  error (ErrorError, TRUE, "Illegal immediate expression");
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
	{
	  ir |= NO_SHIFT;
	}
    }
  return ir;
}
