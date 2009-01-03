/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2008 GCCSDK Developersrs
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
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "area.h"
#include "code.h"
#include "error.h"
#include "expr.h"
#include "expr.h"
#include "fix.h"
#include "get.h"
#include "global.h"
#include "input.h"
#include "lit.h"
#include "mnemonics.h"
#include "option.h"
#include "put.h"
#include "targetcpu.h"


static void
dstmem (WORD ir)
{
  int dst, op;
  BOOL trans = FALSE, half = FALSE;
  BOOL pre, offValue = FALSE;
  Value offset;
  if ((ir & 0x90) == 0x90)
    {
      if (cpuWarn (ARM7M) == FALSE && (ir & 0x20) && targetCPU < ARM10)
	error (ErrorWarning, TRUE,
	"Half-word ops only work correctly when accessed location is cached");
      half = TRUE;
    }
  dst = getCpuReg ();
  ir |= DST_OP (dst);
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, TRUE, "Inserting missing comma before address");
  switch (inputLook ())
    {
    case '[':			/* ldr reg,[ */
      {
	inputSkip ();
	skipblanks ();
	op = getCpuReg ();	/* Base register */
	ir |= LHS_OP (op);
	skipblanks ();
	if (inputLook () == ']')
	  {
	    pre = FALSE;
	    inputSkip ();
	    skipblanks ();
	  }
	else
	  pre = TRUE;
	if (inputLook () == ',')
	  {			/* either [base,XX] or [base],XX */
	    inputSkip ();
	    skipblanks ();
	    if (inputLook () == '#')
	      {
		inputSkip ();
		exprBuild ();
		offset = exprEval (ValueInt | ValueCode | ValueLateLabel | ValueAddr);
		offValue = TRUE;
		switch (offset.Tag.t)
		  {
		  case ValueInt:
		  case ValueAddr:
		    ir = fixCpuOffset (inputLineNo, ir, offset.ValueInt.i);
		    break;
		  case ValueCode:
		  case ValueLateLabel:
		    if ((ir & 0x90) == 0x90)
		      ir |= 0x00400000;
		    relocCpuOffset (ir, offset);
		    break;
		  default:
		    error (ErrorError, TRUE, "Illegal offset expression");
		    break;
		  }

		if (half)
		  ir |= (1 << 22);  /* for H/SH/SB */

		/* UP_FLAG is fixed in fixCpuOffset */
	      }
	    else
	      {
		ir |= UP_FLAG;
		if (inputLook () == '+')
		  {
		    inputSkip ();
		    skipblanks ();
		  }
		else if (inputLook () == '-')
		  {
		    inputSkip ();
		    skipblanks ();
		    ir &= ~UP_FLAG;
		  }
		if (inputLook () == '#')
		  {
		    /* Edge case - #XX */
		    error (ErrorError, TRUE, "Unknown register definition in offset field");
		  }
		ir |= ((ir & 0x90) == 0x90) ? 0 : REG_FLAG;
		ir = getRhs (TRUE, ((ir & 0x90) == 0x90) ? FALSE : TRUE, ir);
		/* Reg {,shiftop {#shift}} */
		offValue = TRUE;
	      }
	    skipblanks ();
	  }
	else
	  {			/* [base] if this way */
	    ir |= UP_FLAG;	/* 0 nicer than -0; pre nicer than post */
	    if (half)
	      ir |= (1 << 22);	/* For H/SH/SB - I bit is inverted */
	    if (pre)
	      error (ErrorError, TRUE, "Illegal character '%c' after base", inputLook ());
	  }
	if (pre)
	  {
	    if (inputLook () == ']')
	      {
		inputSkip ();
		skipblanks ();
	      }
	    else
	      error (ErrorError, TRUE, "Inserting missing ] after address");
	  }
	else
	  {
	    /* If offset value was never set, then make it a pre-index load */
	    if (!offValue)
	      pre = TRUE;
	    else if (dst == op)
	      error (ErrorError, TRUE, "Post increment is not sane where base and destination register are the same");
	  }
	if (inputLook () == '!')
	  {
	    if (pre)
	      ir |= WB_FLAG;
	    else
	      error (ErrorError, TRUE, "Writeback not allowed with post-index");
	    inputSkip ();
	    skipblanks ();
	  }
	if (pre)
	  {
	    ir |= PRE_FLAG;
	    if (trans)
	      error (ErrorError, TRUE, "Translate not allowed with pre-index");
	  }
      }
      break;
    case '=':
      if (ir & 0x00100000)
	{			/* only allowed for ldr */
	  ir |= PRE_FLAG | LHS_OP (15);
	  inputSkip ();
	  exprBuild ();
	  litInt (4, exprEval (ValueInt | ValueString | ValueCode | ValueLateLabel));
	}
      else
	error (ErrorError, FALSE, "You can't store into a constant");
      break;
    default:			/* ldr reg,label */
      /* This is not quite correct since we maybe referencing a label
         in a based area.  So the assumption that a label is PC relative
         will not hold up.  The problem is that we probably don't know
         that the label is within a based area.  We need to mark this
         instruction for later pre-processing.  */
      ir |= PRE_FLAG;
      exprBuild ();
      codePosition (areaCurrent);
      codeOperator (Op_sub);
      codeInt (8);
      codeOperator (Op_sub);
      offset = exprEval (ValueInt | ValueCode | ValueLateLabel | ValueAddr);
      switch (offset.Tag.t)
	{
	case ValueInt:
	  ir |= LHS_OP (15);
	  ir = fixCpuOffset (inputLineNo, ir, offset.ValueInt.i);
	  break;
	case ValueCode:
	case ValueLateLabel:
	  if ((ir & 0x90) == 0x90)
	    ir |= (1 << 22);
	  relocCpuOffset (ir |= LHS_OP (15), offset);
	  break;
	case ValueAddr:
	  ir |= LHS_OP (offset.ValueAddr.r);
	  ir = fixCpuOffset (inputLineNo, ir, offset.ValueAddr.i);
	  break;
	default:
	  error (ErrorError, TRUE, "Illegal address expression");
	  break;
	}
      break;
    }
  putIns (ir);
}


void
m_ldr (WORD cc)
{
  /* Bit 27 set => LDRD */
  dstmem ((cc & ~(1 << 27)) |
    (((cc & 0x90) == 0x90) ? (cc & (1 << 27)) ? 0 : (1 << 20)
			   : ((1 << 20) | (1 << 26))));
}


void
m_str (WORD cc)
{
  /* Bit 27 set => STRD */
  dstmem ((cc & ~(1 << 27)) |
    (((cc & 0x90) == 0x90) ? (cc & (1 << 27)) ? 0x20 : 0 : (1 << 26)));
}


void
m_pld (void)
{
  int op;
  WORD ir = 0xf450f000 | PRE_FLAG;

  cpuWarn (XSCALE);

  skipblanks();

  if (inputGet () != '[')
    error (ErrorError, TRUE, "Expected '[' after PLD instruction");

  skipblanks();
  op = getCpuReg (); /* Base register */
  ir |= LHS_OP (op);
  skipblanks();

  if (inputLook () == ']')
    {			/* [base] */
      inputSkip();
      skipblanks();
      ir |= UP_FLAG;	/* 0 nicer than -0 */
    }
  else
    {
      skipblanks();
      if (inputGet () != ',')
	error (ErrorError, TRUE, "Expected ',' or ']' in PLD instruction");

      skipblanks();

      if (inputLook () == '#')
	{
	  Value offset;

	  inputSkip ();
	  exprBuild ();
	  offset = exprEval (ValueInt | ValueCode);

	  switch (offset.Tag.t)
	    {
	    case ValueInt:
	    case ValueAddr:
	      ir = fixCpuOffset (inputLineNo, ir, offset.ValueInt.i);
	      break;
	    default:
	      error (ErrorError, TRUE, "Illegal offset expression");
	      break;
	    }

	    /* UP_FLAG is fixed in fixCpuOffset */
	}
      else
	{
	  ir |= UP_FLAG;
	  if (inputLook () == '+')
	    {
	      inputSkip ();
	      skipblanks ();
	    }
	  else if (inputLook () == '-')
	    {
	      inputSkip ();
	      skipblanks ();
	      ir &= ~UP_FLAG;
	    }
	  if (inputLook () == '#')
	    {
	      /* Edge case - #XX */
	      error (ErrorError, TRUE, "Unknown register definition in offset field");
	    }
	  ir = getRhs(TRUE, TRUE, ir) | REG_FLAG;
	}

      if (inputLook () == ']')
	{
	  inputSkip ();
	  skipblanks ();
	}
      else
	error (ErrorError, TRUE, "Expected closing ]");
    }
  putIns(ir);
}


static void
dstreglist (WORD ir)
{
  int op, low, high, c;
  Value mask;
  op = getCpuReg ();
  ir |= BASE_MULTI (op);
  skipblanks ();
  if (inputLook () == '!')
    {
      inputSkip ();
      ir |= WB_FLAG;
      skipblanks ();
    }
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, TRUE, "Inserting missing comma before reglist");
  if (inputLook () == '#')
    {				/* constant */
      exprBuild ();
      mask = exprEval (ValueInt | ValueCode | ValueLateLabel);
      switch (mask.Tag.t)
	{
	case ValueInt:
	  ir |= fixMask (inputLineNo, mask.ValueInt.i);
	  break;
	case ValueCode:
	case ValueLateLabel:
	  relocMask (mask);
	  break;
	default:
	  error (ErrorError, TRUE, "Illegal mask expression");
	  break;
	}
    }
  else
    {
      if (inputLook () == '{')
	inputSkip ();
      else
	error (ErrorError, TRUE, "Inserting missing '{' before reglist");
      op = 0;
      do
	{
	  skipblanks ();
	  low = getCpuReg ();
	  skipblanks ();
	  switch (c = inputLook ())
	    {
	    case '-':
	      inputSkip ();
	      skipblanks ();
	      high = getCpuReg ();
	      skipblanks ();
	      if (low > high)
		{
		  error (ErrorInfo, TRUE, "Register interval in wrong order r%d-r%d", low, high);
		  c = low;
		  low = high;
		  high = c;
		}
	      break;
	    case ',':
	    case '}':
	      high = low;
	      break;
	    default:
	      error (ErrorError, TRUE, "Illegal character '%c' in register list", c);
	      high = 15;
	      break;
	    }
	  if (1 << low < op)
	    error (ErrorInfo, TRUE, "Registers in wrong order");
	  if (((1 << (high + 1)) - (1 << low)) & op)
	    error (ErrorInfo, TRUE, "Register occurs more than once in register list");
	  op |= (1 << (high + 1)) - (1 << low);
	}
      while ((c = inputGet ()) == ',');
      if (c != '}')
	{
	  inputUnGet (c);
	  error (ErrorError, TRUE, "Inserting missing '}' after reglist");
	}
      ir |= op;
    }
  skipblanks ();
  if (inputLook () == '^')
    {
      inputSkip ();
      if (ir & WB_FLAG && !(ir & (1 << 15)))
	error (ErrorInfo, TRUE, "Writeback together with force user");
      ir |= FORCE_FLAG;
      skipblanks ();
    }
  putIns (ir);
}


void
m_ldm (WORD cc)
{
  dstreglist (cc | 0x08100000);
}


void
m_stm (WORD cc)
{
  dstreglist (cc | 0x08000000);
}


void
m_swp (WORD cc)
{
  int ir = cc | 0x01000090;
  cpuWarn (ARM250);
  ir |= DST_OP (getCpuReg ());
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, TRUE, "%sdst", InsertCommaAfter);
  ir |= RHS_OP (getCpuReg ());	/* Note wrong order swp dst,rhs,[lsh] */
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, TRUE, "%slhs", InsertCommaAfter);
  if (inputLook () == '[')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, TRUE, "Inserting missing '['");
  ir |= DST_MUL (getCpuReg ());
  skipblanks ();
  if (inputLook () == ']')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, TRUE, "Inserting missing ']'");
  putIns (ir);
}
