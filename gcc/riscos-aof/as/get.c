
/*
 * get.c
 * Copyright © 1992 Niklas Röjemo
 */

#include <string.h>
#include <stdint.h>
#include "get.h"
#include "error.h"
#include "input.h"
#include "lex.h"
#include "symbol.h"
#include "expr.h"
#include "reloc.h"
#include "help_cpu.h"
#include "fix.h"

extern int gcc_backend;

struct reg_id
  {
    char *name;
    int reg_no;
    int len;
  };

WORD 
getCpuReg (void)
{
  static struct reg_id cpu_regs[] =
  {
    "r0", 0, 2, "r1", 1, 2, "r2", 2, 2, "r3", 3, 2,
    "r4", 4, 2, "r5", 5, 2, "r6", 6, 2, "r7", 7, 2,
    "r8", 8, 2, "r9", 9, 2, "r10", 10, 3, "r11", 11, 3,
    "r12", 12, 3, "r13", 13, 3, "r14", 14, 3, "r15", 15, 3,
    "a1", 0, 2, "a2", 1, 2, "a3", 2, 2, "a4", 3, 2,
    "v1", 4, 2, "v2", 5, 2, "v3", 6, 2, "v4", 7, 2, "v5", 8, 2, "v6", 9, 2,
    "sl", 10, 2, "fp", 11, 2, "ip", 12, 2, "sp", 13, 2, "lr", 14, 2, "pc", 15, 2,
    "rfp", 9, 3, "sb", 9, 2
  };

  Lex lexSym;
  int loop;
  Symbol *sym;

  lexSym = lexGetId ();
  if (lexSym.tag == LexNone)
    return 0;

  if (gcc_backend && (lexSym.tag == LexId))
    for (loop = 0; loop < sizeof (cpu_regs) / sizeof (struct reg_id); loop++)
      {
	if ((cpu_regs[loop].len == lexSym.LexId.len) &&
	 !strncmp (cpu_regs[loop].name, lexSym.LexId.str, lexSym.LexId.len))
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
  static struct reg_id cpu_regs[8] =
  {"f0", 0, 2, "f1", 1, 2, "f2", 2, 2, "f3", 3, 2,
   "f4", 4, 2, "f5", 5, 2, "f6", 6, 2, "f7", 7, 2};

  Lex lexSym;
  Symbol *sym;
  int loop;

  lexSym = lexGetId ();
  if (lexSym.tag == LexNone)
    return 0;

  if (gcc_backend && (lexSym.tag == LexId) && (lexSym.LexId.len == 2))
    for (loop = 0; loop < sizeof (cpu_regs) / sizeof (struct reg_id); loop++)
      {
	if ((cpu_regs[loop].len == lexSym.LexId.len) &&
	 !strncmp (cpu_regs[loop].name, lexSym.LexId.str, lexSym.LexId.len))
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
  Symbol *sym;
  Lex lexSym = lexGetId ();

  if (lexSym.tag == LexNone)
    return 0;
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
  Symbol *sym;
  Lex lexSym = lexGetId ();

  if (lexSym.tag == LexNone)
    return 0;
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
  WORD r;
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
