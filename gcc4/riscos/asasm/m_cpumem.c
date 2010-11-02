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
#include "input.h"
#include "lit.h"
#include "m_cpumem.h"
#include "option.h"
#include "put.h"
#include "targetcpu.h"


static void
dstmem (ARMWord ir)
{
  int dst, op;
  bool trans = false, half = false;
  bool pre, offValue = false;
  Value offset;
  if ((ir & 0x90) == 0x90)
    {
      if (cpuWarn (ARM7M) == false && (ir & 0x20) && targetCPU < ARM10)
	error (ErrorWarning, "Half-word ops only work correctly when accessed location is cached");
      half = true;
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
    error (ErrorError, "Inserting missing comma before address");
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
	    pre = false;
	    inputSkip ();
	    skipblanks ();
	  }
	else
	  pre = true;
	if (inputLook () == ',')
	  {			/* either [base,XX] or [base],XX */
	    inputSkip ();
	    skipblanks ();
	    if (inputLook () == '#')
	      {
		inputSkip ();
		offset = exprBuildAndEval (ValueInt | ValueAddr | ValueCode | ValueLateLabel);
		offValue = true;
		switch (offset.Tag)
		  {
		  case ValueInt:
		  case ValueAddr:
		    ir = fixCpuOffset (0, ir, offset.Data.Int.i);
		    break;
		  case ValueCode:
		  case ValueLateLabel:
		    if ((ir & 0x90) == 0x90)
		      ir |= 0x00400000;
		    relocCpuOffset (ir, &offset);
		    break;
		  default:
		    error (ErrorError, "Illegal offset expression");
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
		    error (ErrorError, "Unknown register definition in offset field");
		  }
		ir |= ((ir & 0x90) == 0x90) ? 0 : REG_FLAG;
		ir = getRhs (true, ((ir & 0x90) == 0x90) ? false : true, ir);
		/* Reg {,shiftop {#shift}} */
		offValue = true;
	      }
	    skipblanks ();
	  }
	else
	  {			/* [base] if this way */
	    ir |= UP_FLAG;	/* 0 nicer than -0; pre nicer than post */
	    if (half)
	      ir |= (1 << 22);	/* For H/SH/SB - I bit is inverted */
	    if (pre)
	      error (ErrorError, "Illegal character '%c' after base", inputLook ());
	  }
	if (pre)
	  {
	    if (inputLook () == ']')
	      {
		inputSkip ();
		skipblanks ();
	      }
	    else
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
	if (inputLook () == '!')
	  {
	    if (pre)
	      ir |= WB_FLAG;
	    else
	      error (ErrorError, "Writeback not allowed with post-index");
	    inputSkip ();
	    skipblanks ();
	  }
	if (pre)
	  {
	    ir |= PRE_FLAG;
	    if (trans)
	      error (ErrorError, "Translate not allowed with pre-index");
	  }
      }
      break;
    case '=':
      if (ir & 0x00100000)
	{			/* only allowed for ldr */
	  Value value;
	  ir |= PRE_FLAG | LHS_OP (15);
	  inputSkip ();
	  value = exprBuildAndEval (ValueInt | ValueString | ValueCode | ValueLateLabel);
	  litInt (4, &value);
	}
      else
	errorAbort ("You can't store into a constant");
      break;
    default:			/* ldr reg,label */
      /* We're dealing with one of the following:
       *
       * 1) a PC-relative label
       * 2) a field in a register-based map
       * 3) a label in a register-based area
       */
      /* Whatever happens, this must be a pre-increment */
      ir |= PRE_FLAG;

      /* Firstly, see if it's a field in a register-based map */
      offset = exprBuildAndEval (ValueAddr);
      switch (offset.Tag)
	{
	case ValueAddr:
	  ir |= LHS_OP (offset.Data.Addr.r);
	  ir = fixCpuOffset (0, ir, offset.Data.Addr.i);
	  break;
	default:
	  /* No, so it's either a PC-relative or based area.
	   * The relocation code will fix based areas up for us.
	   *
	   * TODO: It could also be a late field. Unfortunately, there's no
	   * simple way of detecting this when fixing up relocations. For now
	   * we'll just treat the input as invalid and give up. */

	  /* The previous evaluation will have left its result in the current
	   * program. Append " - . - 8" to the program so we can calculate the
	   * offset of the label from the current position. */
	  codePosition (areaCurrentSymbol);
	  codeOperator (Op_sub);
	  codeInt (8);
	  codeOperator (Op_sub);
	  offset = exprEval (ValueInt | ValueCode | ValueLateLabel | ValueAddr);
	  switch (offset.Tag)
	    {
	    case ValueInt:
	      ir |= LHS_OP (15);
	      ir = fixCpuOffset (0, ir, offset.Data.Int.i);
	      break;
	    case ValueCode:
	    case ValueLateLabel:
	      if ((ir & 0x90) == 0x90)
	        ir |= (1 << 22);
	      relocCpuOffset (ir |= LHS_OP (15), &offset);
	      break;
	    case ValueAddr:
	      ir |= LHS_OP (offset.Data.Addr.r);
	      ir = fixCpuOffset (0, ir, offset.Data.Addr.i);
	      break;
	    default:
	      error (ErrorError, "Illegal address expression");
	      break;
	    }
	  break;
	}
      break;
    }
  putIns (ir);
}


/**
 * Implements LDR<cond>[B].
 */
bool
m_ldr (void)
{
  ARMWord cc = optionCondBT ();
  if (cc == optionError)
    return true;
  /* Bit 27 set => LDRD */
  dstmem ((cc & ~(1 << 27))
          | (((cc & 0x90) == 0x90) ? (cc & (1 << 27)) ? 0 : (1 << 20)
	                           : ((1 << 20) | (1 << 26))));
  return false;
}

/**
 * Implements STR<cond>[B].
 */
bool
m_str (void)
{
  ARMWord cc = optionCondBT ();
  if (cc == optionError)
    return true;
  /* Bit 27 set => STRD */
  dstmem ((cc & ~(1 << 27))
          | (((cc & 0x90) == 0x90) ? (cc & (1 << 27)) ? 0x20 : 0
	                           : (1 << 26)));
  return false;
}

/**
 * Implements PLD.
 */
bool
m_pld (void)
{
  ARMWord ir = 0xf450f000 | PRE_FLAG;

  cpuWarn (XSCALE);

  skipblanks();

  if (inputGet () != '[')
    error (ErrorError, "Expected '[' after PLD instruction");

  skipblanks();
  int op = getCpuReg (); /* Base register */
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
	error (ErrorError, "Expected ',' or ']' in PLD instruction");

      skipblanks();

      if (inputLook () == '#')
	{
	  inputSkip ();
	  Value offset = exprBuildAndEval (ValueInt | ValueCode);
	  switch (offset.Tag)
	    {
	    case ValueInt:
	    case ValueAddr:
	      ir = fixCpuOffset (0, ir, offset.Data.Int.i);
	      break;
	    default:
	      error (ErrorError, "Illegal offset expression");
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
	      error (ErrorError, "Unknown register definition in offset field");
	    }
	  ir = getRhs(true, true, ir) | REG_FLAG;
	}

      if (inputLook () == ']')
	{
	  inputSkip ();
	  skipblanks ();
	}
      else
	error (ErrorError, "Expected closing ]");
    }
  putIns(ir);
  return false;
}


static void
dstreglist (ARMWord ir)
{
  int op, low, high, c;
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
    error (ErrorError, "Inserting missing comma before reglist");
  if (inputLook () == '#')
    {				/* constant */
      Value mask = exprBuildAndEval (ValueInt | ValueCode | ValueLateLabel);
      switch (mask.Tag)
	{
	case ValueInt:
	  ir |= fixMask (0, mask.Data.Int.i);
	  break;
	case ValueCode:
	case ValueLateLabel:
	  relocMask (&mask);
	  break;
	default:
	  error (ErrorError, "Illegal mask expression");
	  break;
	}
    }
  else
    {
      if (inputLook () == '{')
	inputSkip ();
      else
	error (ErrorError, "Inserting missing '{' before reglist");
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
		  error (ErrorInfo, "Register interval in wrong order r%d-r%d", low, high);
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
	      error (ErrorError, "Illegal character '%c' in register list", c);
	      high = 15;
	      break;
	    }
	  if (1 << low < op)
	    error (ErrorInfo, "Registers in wrong order");
	  if (((1 << (high + 1)) - (1 << low)) & op)
	    error (ErrorInfo, "Register occurs more than once in register list");
	  op |= (1 << (high + 1)) - (1 << low);
	}
      while ((c = inputGet ()) == ',');
      if (c != '}')
	{
	  inputUnGet (c);
	  error (ErrorError, "Inserting missing '}' after reglist");
	}
      ir |= op;
    }
  skipblanks ();
  if (inputLook () == '^')
    {
      inputSkip ();
      if ((ir & WB_FLAG) && !(ir & (1 << 15)))
	error (ErrorInfo, "Writeback together with force user");
      ir |= FORCE_FLAG;
      skipblanks ();
    }
  putIns (ir);
}


/**
 * Implements LDM.
 */
bool
m_ldm (void)
{
  ARMWord cc = optionCondDirLdm ();
  if (cc == optionError)
    return true;
  dstreglist (cc | 0x08100000);
  return false;
}

/**
 * Implements STM.
 */
bool
m_stm (void)
{
  ARMWord cc = optionCondDirStm ();
  if (cc == optionError)
    return true;
  dstreglist (cc | 0x08000000);
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
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, "%sdst", InsertCommaAfter);
  ir |= RHS_OP (getCpuReg ());	/* Note wrong order swp dst,rhs,[lsh] */
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, "%slhs", InsertCommaAfter);
  if (inputLook () == '[')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, "Inserting missing '['");
  ir |= DST_MUL (getCpuReg ());
  skipblanks ();
  if (inputLook () == ']')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, "Inserting missing ']'");
  putIns (ir);
  return false;
}
