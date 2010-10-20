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
 * m_cpuctrl.c
 */

#include "config.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "area.h"
#include "code.h"
#include "error.h"
#include "expr.h"
#include "fix.h"
#include "get.h"
#include "global.h"
#include "help_cpu.h"
#include "input.h"
#include "mnemonics.h"
#include "option.h"
#include "os.h"
#include "put.h"
#include "targetcpu.h"
#include "value.h"

/** CONTROL **/

void
m_branch (ARMWord cc)
{
  ARMWord ir = cc | 0x0A000000;
  switch (inputLook ())
    {
    case '#':
      inputSkip ();
    case '"':
      exprBuild ();
      break;
    default:
      exprBuild ();
      /* The branch instruction has its offset as relative, while the given
	 label is absolute, so calculate "<label> - . - 8".  */
      codePosition (areaCurrentSymbol);
      codeOperator (Op_sub);
      codeInt (8);
      codeOperator (Op_sub);
      break;
    }
  Value im = exprEval (ValueInt | ValueCode | ValueLateLabel);
  switch (im.Tag)
    {
    case ValueInt:
      ir |= fixBranch (0, im.ValueInt.i);
      break;
    case ValueCode:
    case ValueLateLabel:
      relocBranch (&im);
      break;
    default:
      error (ErrorError, "Illegal branch destination");
      break;
    }
  putIns (ir);
}

void
m_blx (ARMWord cc)
{
  cpuWarn (XSCALE);

  inputMark ();
  ARMWord reg = getCpuRegNoError ();
  ARMWord ir;
  if (reg == INVALID_REG)
    {			/* BLXcc <target_addr> */
      inputRollback ();

      if (cc != AL)
        error (ErrorError, "BLX <target_addr> must be unconditional");

      ir = NV | 0x0A000000;

      switch (inputLook ())
	{
	case '#':
	  inputSkip ();
	case '"':
	  exprBuild ();
	  break;
	default:
	  exprBuild ();
	  /* The branch instruction has its offset as relative, while the given
	     label is absolute, so calculate "<label> - . - 8".  */
	  codePosition (areaCurrentSymbol);
	  codeOperator (Op_sub);
	  codeInt (8);
	  codeOperator (Op_sub);
	  break;
	}
      Value im = exprEval (ValueInt | ValueCode | ValueLateLabel);
      switch (im.Tag)
	{
	case ValueInt:
	  ir |= fixBranchT (0, im.ValueInt.i);
	  break;
	case ValueCode:
	case ValueLateLabel:
	  relocBranchT (&im);
	  break;
	default:
	  error (ErrorError, "Illegal branch destination");
	  break;
	}
    }
  else
    ir = cc | 0x012FFF30 | RHS_OP (reg); /* BLX <Rm> */

  putIns (ir);
}

void
m_bx (ARMWord cc)
{
  cpuWarn (XSCALE);

  int dst = getCpuReg();
  if (dst == 15)
    error (ErrorWarning, "Use of PC with BX is discouraged");

  putIns (cc | 0x012fff10 | dst);
}

void
m_swi (ARMWord cc)
{
  if (inputLook () == '#')
    {
      inputSkip ();
      error (ErrorInfo, "SWI is always immediate");
    }
  exprBuild ();
  Value im = exprEval (ValueInt | ValueAddr | ValueString | ValueCode
		 | ValueLateLabel);
  ARMWord ir = cc | 0x0F000000;
  switch (im.Tag)
    {
      case ValueInt:
	ir |= fixSwi (0, im.ValueInt.i);
	break;
      case ValueAddr:
	ir |= fixSwi (0, im.ValueAddr.i);
	break;
      case ValueCode:
      case ValueLateLabel:
	relocSwi (&im);
	break;
      case ValueString:
#ifdef __riscos__
	{
	  /* ValueString are not NUL terminated.  */
	  char swiname[im.ValueString.len + 1];
	  memcpy (swiname, im.ValueString.s, im.ValueString.len);
	  swiname[im.ValueString.len] = '\0';
	  ir |= switonum (swiname);
	  if (ir == 0xFFFFFFFF)
	    error (ErrorError, "Unknown SWI name");
	}
#else
	error (ErrorError, "RISC OS is required to look up the SWI name");
#endif
	break;
      default:
	error (ErrorError, "Illegal SWI expression");
	break;
    }
  putIns (ir);
}

void
m_bkpt (void)
{
  cpuWarn (XSCALE);

  if (inputLook () == '#')
    {
      inputSkip ();
      error (ErrorInfo, "BKPT is always immediate");
    }
  exprBuild ();
  Value im = exprEval (ValueInt);
  if (im.Tag != ValueInt)
    error (ErrorError, "Illegal BKPT expression");
  ARMWord val = fixInt (0, 2, im.ValueInt.i);

  ARMWord ir = 0xE1200070;
  ir |= ((val & 0xFFF0) << 4) | (val & 0xF);

  putIns (ir);
}


/** EXTENSION **/

void
m_adr (ARMWord cc)
{
  ARMWord ir = cc & ~1, ir2 = 0;	/* bit 0 set to indicate ADRL */
  ir |= DST_OP (getCpuReg ());
  ir |= LHS_OP (15) | IMM_RHS;	/* pc */
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, "%sdst", InsertCommaAfter);
  /* The label will expand to either a field in a based map or a PC-relative 
     expression.  */
  exprBuild ();
  /* Try the field first */
  Value im = exprEval (ValueAddr);
  switch (im.Tag)
    {
    case ValueAddr:
      /* Fix up the base register */
      ir &= ~LHS_OP (15);
      ir |= LHS_OP (im.ValueAddr.r);
      /* Manufacture the offset as per normal */
      if (cc & 1)
	fixAdrl (0, &ir, &ir2, im.ValueInt.i, 0); /* don't warn ? */
      else
	ir = fixAdr (0, ir, im.ValueInt.i);
      break;
    default:
      /* Not a field, so append " - . - 8" to the expression, and treat it as
         PC-relative.  */
      /* TODO: It may also be a late field but there's no simple way of
         handling this when performing relocation.  */
      codePosition (areaCurrentSymbol);	/* It's relative */
      codeOperator (Op_sub);
      codeInt (8);
      codeOperator (Op_sub);
      im = exprEval (ValueInt | ValueCode | ValueLateLabel);
      switch (im.Tag)
	{
	case ValueInt:
	  if (cc & 1)
	    fixAdrl (0, &ir, &ir2, im.ValueInt.i, 0);	/* don't warn ? */
	  else
	    ir = fixAdr (0, ir, im.ValueInt.i);
	  break;
	case ValueCode:
	case ValueLateLabel:
	  if (cc & 1)
	    relocAdrl (ir, &im);
	  else
	    relocAdr (ir, &im);
	  break;
	default:
	  error (ErrorError, "Illegal ADR%s expression", (cc & 1) ? "L" : "");
	  break;
	}
      break;
    }
  putIns (ir);
  if (cc & 1)
    putIns (ir2);
}


/** APCS prologue **/

static signed int regs[3] = {-1, -1, -1};
/* [0] Stack args       0 = no, 1..4 = a1-an; -1 = RET or TAIL will cause error
 * [1] Stack reg vars 0 = no, 1..6 = v1-vn
 * [2] Stack fp vars    0 = no, 1..4 = f4-f(3+n)
 */

void
m_stack (void)
{
  static const unsigned int lim[3] = {4, 6, 4};
  static const ARMWord
    arg_regs[]  = {0x00000000, 0xE92D0001, 0xE92D0003, 0xE92D0007, 0xE92D000F},
    push_inst[] = {0xE92DD800, 0xE92DD810, 0xE92DD830, 0xE92DD870,
		   0xE92DD8F0, 0xE92DD9F0, 0xE92DDBF0},
    pfp_inst[]  = {0x00000000, 0xED2DC203, 0xED6D4206, 0xED6DC209, 0xED2D420C};

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
	      error (ErrorError, "Illegal register class %c", c);
	      break;
	    }
	  if (regs[reg] != -1)
	    error (ErrorError, "Register class %c duplicated", c);
	  if (inputLook () == '=')
	    {
	      inputSkip ();
	      exprBuild ();
	      im = exprEval (ValueInt);
	      if (im.Tag != ValueInt)
		im.ValueInt.i = 0;
	      if ((unsigned) im.ValueInt.i > lim[reg])
		error (ErrorError, "Too many registers to stack for class %c", c);
	      regs[reg] = (signed) im.ValueInt.i;
	    }
	  else
	    regs[reg] = (signed) lim[reg];
	  skipblanks ();
	  c = inputLook ();
	  if (c == ',')
	    inputSkip ();
	  else if (c)
	    error (ErrorError, "%sregister class %c", InsertCommaAfter, c);
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
apcsEpi (ARMWord cc, const int *pop_inst, const char *op)
{
  static const ARMWord pfp_inst[] =
    {
      0x00000000,
      0x0CBDC203,
      0x0CFD4206,
      0x0CFDC209,
      0x0CBD420C
    };

  if (regs[0] == -1)
    error (ErrorError, "Cannot assemble %s without an earlier STACK", op);

  if (regs[2] > 0)
    putIns (pfp_inst[regs[2]] | cc);
  putIns (pop_inst[regs[1]] | cc);
}

/* APCS epilogue - return */

void
m_ret (ARMWord cc)
{
  static const int pop_inst[] =
    {
      0x095BA800,
      0x095BA810,
      0x095BA830,
      0x095BA870,
      0x095BA8F0,
      0x095BA9F0,
      0x095BABF0
    };

  apcsEpi (cc, pop_inst, "RET");
}

/* APCS epilogue - tail call */

void
m_tail (ARMWord cc)
{
  static const int pop_inst[] =
    {
      0x091B6800,
      0x091B6810,
      0x091B6830,
      0x091B6870,
      0x091B68F0,
      0x091B69F0,
      0x091B6BF0
    };

  apcsEpi (cc, pop_inst, "TAIL");
  skipblanks ();
  if (inputLook ())
    m_branch (cc);
}


/* PSR access */

static ARMWord
getpsr (bool only_all)
{
  ARMWord saved;
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
      error (ErrorError, "Not a PSR name");
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
	  error (ErrorError, "Partial PSR access not allowed");
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
	      error (ErrorError, "Unrecognised PSR subset");
	      break;
	    }
	  if (p)
	    {
	      if (saved & (1 << p))
		error (ErrorError, "PSR mask bit '%c' already specified", c);
	      saved |= 1 << p;
	    }
	}
      return saved;
    }
  else
    error (ErrorError, "Not a PSR name");
  return 0;
}


void
m_msr (ARMWord cc)
{
  cpuWarn (ARM6);
  cc |= getpsr (false) | 0x0120F000;
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, "%slhs", InsertCommaAfter);
  if (inputLook () == '#')
    {
      inputSkip ();
      exprBuild ();
      Value im = exprEval (ValueInt);
      if (im.Tag == ValueInt)
	{
	  cc |= 0x02000000;
	  cc |= fixImm8s4 (0, cc, im.ValueInt.i);
	}
      else
	error (ErrorError, "Illegal immediate expression");
    }
  else
    cc |= getCpuReg ();
  putIns (cc);
}


void
m_mrs (ARMWord cc)
{
  cpuWarn (ARM6);
  cc |= getCpuReg () << 12 | 0x01000000;
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, "%slhs", InsertCommaAfter);
  cc |= getpsr (true);
  putIns (cc);
}
