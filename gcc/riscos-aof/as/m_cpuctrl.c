/*
 * m_cpuctrl.c
 * Copyright © 1992 Niklas Röjemo
 */

#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include "mnemonics.h"
#include "error.h"
#include "option.h"
#include "put.h"
#include "input.h"
#include "global.h"
#include "expr.h"
#include "code.h"
#include "area.h"
#include "help_cpu.h"
#include "get.h"
#include "value.h"
#include "fix.h"
#include "os.h"
#include "targetcpu.h"

/** CONTROL **/

void 
m_branch (WORD cc)
{
  Value im;
  WORD ir = cc | 0x0A000000;
  switch (inputLook ())
    {
    case '#':
      inputSkip ();
    case '"':
      exprBuild ();
      break;
    default:
      exprBuild ();
      codePosition (areaCurrent);	/* It's relative */
      codeOperator (Op_sub);
      codeInt (8);
      codeOperator (Op_sub);
    }
  im = exprEval (ValueInt | ValueCode | ValueLateLabel);
  switch (im.Tag.t)
    {
    case ValueInt:
      ir |= fixBranch (inputLineNo, im.ValueInt.i);
      break;
    case ValueCode:
    case ValueLateLabel:
      relocBranch (im);
      break;
    default:
      error (ErrorError, TRUE, "Illegal branch destination");
    }
  putIns (ir);
}

void 
m_swi (WORD cc)
{
  Value im;
  WORD ir = cc | 0x0F000000;
  if (inputLook () == '#')
    {
      inputSkip ();
      error (ErrorInfo, TRUE, "SWI is always immediate");
    }
  exprBuild ();
  im = exprEval (ValueInt | ValueString | ValueCode | ValueLateLabel);
  switch (im.Tag.t)
    {
    case ValueInt:
      ir |= fixSwi (inputLineNo, im.ValueInt.i);
      break;
    case ValueCode:
    case ValueLateLabel:
      relocSwi (im);
      break;
    case ValueString:
#ifdef __riscos
      im.ValueString.s[im.ValueString.len] = 0;
      ir |= switonum (im.ValueString.s);
      if (ir == 0xFFFFFFFF)
	error (ErrorError, TRUE, "Unknown SWI name");
#else
      error (ErrorError, TRUE,
	     "Sorry, RISC OS is required to look up the SWI name");
#endif
      break;
    default:
      error (ErrorError, TRUE, "Illegal SWI expression");
    }
  putIns (ir);
}


/** EXTENSION **/

void 
m_adr (WORD cc)
{
  WORD ir = cc & ~1, ir2 = 0;	/* bit 0 set to indicate ADRL */
  Value im;
  ir |= DST_OP (getCpuReg ());
  ir |= LHS_OP (15) | IMM_RHS;	/* pc */
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, TRUE, "%sdst", InsertCommaAfter);
  exprBuild ();
  codePosition (areaCurrent);	/* It's relative */
  codeOperator (Op_sub);
  codeInt (8);
  codeOperator (Op_sub);
  im = exprEval (ValueInt | ValueCode | ValueLateLabel);
  switch (im.Tag.t)
    {
    case ValueInt:
      if (cc & 1)
	fixAdrl (inputLineNo, &ir, &ir2, im.ValueInt.i, 0);	/* don't warn ? */
      else
	ir = fixAdr (inputLineNo, ir, im.ValueInt.i);
      break;
    case ValueCode:
    case ValueLateLabel:
      if (cc & 1)
	relocAdrl (ir, im);
      else
	relocAdr (ir, im);
      break;
    default:
      error (ErrorError, TRUE, "Illegal ADR%s expression", cc & 1 ? "L" : "");
      break;
    }
  putIns (ir);
  if (cc & 1)
    putIns (ir2);
}


/** APCS prologue **/

static signed int regs[3] =
{-1, -1, -1};
/* [0] Stack args       0 = no, 1..4 = a1-an; -1 = RET or TAIL will cause error
 * [1] Stack reg vars 0 = no, 1..6 = v1-vn
 * [2] Stack fp vars    0 = no, 1..4 = f4-f(3+n)
 */

void 
m_stack (void)
{
  static const unsigned int lim[3] =
  {4, 6, 4};
  static const int
    arg_regs[] =
  {0x00000000, 0xE92D0001, 0xE92D0003, 0xE92D0007, 0xE92D000F}, push_inst[] =
  {0xE92DD800, 0xE92DD810, 0xE92DD830, 0xE92DD870,
   0xE92DD8F0, 0xE92DD9F0, 0xE92DDBF0}, pfp_inst[] =
  {0x00000000, 0xED2DC203, 0xED6D4206, 0xED6DC209, 0xED2D420C};

  regs[2] = regs[1] = regs[0] = -1;
  skipblanks ();
  if (inputLook ())
    {
      int reg = 0;
      Value im;
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
	      error (ErrorError, TRUE, "Illegal register class %c", c);
	    }
	  if (regs[reg] != -1)
	    error (ErrorError, TRUE, "Register class %c duplicated", c);
	  if (inputLook () == '=')
	    {
	      inputSkip ();
	      exprBuild ();
	      im = exprEval (ValueInt);
	      if (im.Tag.t != ValueInt)
		im.ValueInt.i = 0;
	      if ((unsigned) im.ValueInt.i > lim[reg])
		error (ErrorError, TRUE, "Too many registers to stack for class %c", c);
	      regs[reg] = (signed) im.ValueInt.i;
	    }
	  else
	    regs[reg] = (signed) lim[reg];
	  skipblanks ();
	  c = inputLook ();
	  if (c == ',')
	    inputSkip ();
	  else if (c)
	    error (ErrorError, TRUE, "%sregister class %c", InsertCommaAfter, c);
	}
      while (c);
      if (c)
	inputUnGet (c);
    }
  putIns (0xE1A0C00D);
  if (regs[0] < 0)
    regs[0] = 0;
  if (regs[0])
    putIns (arg_regs[regs[0]]);
  if (regs[1] == -1)
    regs[1] = 0;
  putIns (push_inst[regs[1]]);
  if (regs[2] > 0)
    putIns (pfp_inst[regs[2]]);
  putIns (0xE24CB004 + 4 * regs[0]);
}


/** APCS epilogue **/

static void 
apcsEpi (WORD cc, const int *pop_inst, const char *op)
{
  static const int pfp_inst[]
  =
  {0x00000000, 0x0CBDC203, 0x0CFD4206, 0x0CFDC209, 0x0CBD420C};

  if (regs[0] == -1)
    error (ErrorError, TRUE, "Cannot assemble %s without an earlier STACK", op);

  if (regs[2] > 0)
    putIns (pfp_inst[regs[2]] | cc);
  putIns (pop_inst[regs[1]] | cc);
}

/* APCS epilogue - return */

void 
m_ret (WORD cc)
{
  static const int pop_inst[]
  =
  {0x095BA800, 0x095BA810, 0x095BA830, 0x095BA870, 0x095BA8F0,
   0x095BA9F0, 0x095BABF0};
  apcsEpi (cc, pop_inst, "RET");
}

/* APCS epilogue - tail call */

void 
m_tail (WORD cc)
{
  static const int pop_inst[]
  =
  {0x091B6800, 0x091B6810, 0x091B6830, 0x091B6870, 0x091B68F0,
   0x091B69F0, 0x091B6BF0};
  apcsEpi (cc, pop_inst, "TAIL");
  skipblanks ();
  if (inputLook ())
    m_branch (cc);
}


/* PSR access */

static WORD 
getpsr (BOOL only_all)
{
  WORD saved;
  char w[4];

  skipblanks ();
  switch (inputGetLower ())
    {
    case 'c':
      saved = 0;
      break;
    case 's':
      saved = 1 << 22;
      break;
    default:
      error (ErrorError, TRUE, "Not a PSR name");
      return 0;
    }
  if (inputGetLower () == 'p' &&
      inputGetLower () == 's' &&
      inputGetLower () == 'r')
    {
      if (inputLook () != '_')
	return saved | (only_all ? 0xF0000 : 0x90000);
      inputMark ();
      inputSkip ();
      w[0] = inputGetLower ();
      w[1] = inputGetLower ();
      w[2] = inputGetLower ();
      w[3] = 0;
      if (!strcmp (w, "all"))
	return saved | (only_all ? 0xF0000 : 0x90000);
      if (only_all)
	{
	  error (ErrorError, TRUE, "Partial PSR access not allowed");
	  return 0;
	}
      if (!strcmp (w, "ctl"))
	return saved | 0x10000;
      if (!strcmp (w, "flg"))
	return saved | 0x80000;
      inputRollback ();
      while (strchr ("_cCxXsSfF", inputLook ()))
	{
	  int p = 0;
	  char c;
	  if (inputLook () == '_')
	    inputSkip ();
	  switch (c = inputGetLower ())
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
	      error (ErrorError, TRUE, "Unrecognised PSR subset");
	    }
	  if (p)
	    {
	      if (saved & 1 << p)
		error (ErrorError, TRUE, "PSR mask bit '%c' already specified", c);
	      saved |= 1 << p;
	    }
	}
      return saved;
    }
  else
    error (ErrorError, TRUE, "Not a PSR name");
  return 0;
}


void 
m_msr (WORD cc)
{
  cpuWarn (ARM6);
  cc |= getpsr (FALSE) | 0x0120F000;
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, TRUE, "%slhs", InsertCommaAfter);
  if (inputLook () == '#')
    {
      Value im;
      inputSkip ();
      exprBuild ();
      im = exprEval (ValueInt);
      if (im.Tag.t == ValueInt)
	{
	  cc |= 0x02000000;
	  cc |= fixImm8s4 (inputLineNo, cc, im.ValueInt.i);
	}
      else
	error (ErrorError, TRUE, "Illegal immediate expression");
    }
  else
    cc |= getCpuReg ();
  putIns (cc);
}


void 
m_mrs (WORD cc)
{
  cpuWarn (ARM6);
  cc |= getCpuReg () << 12 | 0x0100F000;
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, TRUE, "%slhs", InsertCommaAfter);
  cc |= getpsr (TRUE);
  putIns (cc);
}
