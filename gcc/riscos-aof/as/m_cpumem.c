/*
 * m_cpumem.c
 * Copyright © 1992 Niklas Röjemo
 */
#include "sdk-config.h"
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "mnemonics.h"
#include "error.h"
#include "option.h"
#include "put.h"
#include "input.h"
#include "global.h"
#include "expr.h"
#include "code.h"
#include "area.h"
#include "lit.h"
#include "get.h"
#include "fix.h"
#include "expr.h"
#include "targetcpu.h"


static void
dstmem (WORD ir)
{
  int dst, op;
  BOOL trans = FALSE;
  BOOL pre, offValue = FALSE;
  Value offset;
  if ((ir & 0x90) == 0x90)
    if (cpuWarn (ARM7M) == FALSE && (ir & 0x20))
      error (ErrorWarning, TRUE,
      "Half-word ops only work correctly when accessed location is cached");
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
	BOOL up = TRUE;
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
	    if (inputLook () == '+')
	      {
		inputSkip ();
		skipblanks ();
	      }
	    else if (inputLook () == '-')
	      {
		inputSkip ();
		skipblanks ();
		up = FALSE;
	      }
	    if (inputLook () == '#')
	      {
		inputSkip ();
		exprBuild ();
		if (!up)
		  codeOperator (Op_neg);
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
		/* UP_FLAG is fixed in fixCpuOffset */
	      }
	    else
	      {
		ir |= ((ir & 0x90) == 0x90) ? (1 << 22) /* for H/SH/SB */ : REG_FLAG;
		ir = getRhs (TRUE, ((ir & 0x90) == 0x90) ? FALSE : TRUE, ir);
		/* Reg {,shiftop {#shift}} */
		offValue = TRUE;
		if (up)
		  ir |= UP_FLAG;/* | PRE_FLAG;*/	/* 0 nicer than #-0 */
	      }
	    skipblanks ();
	  }
	else
	  {			/* [base] if this way */
	    ir |= UP_FLAG;	/* 0 nicer than -0; pre nicer than post */
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
    }
  putIns (ir);
}


void
m_ldr (WORD cc)
{
  dstmem (cc | (((cc & 0x90) == 0x90) ? (1 << 20) : ((1 << 20) | (1 << 26))));
}


void
m_str (WORD cc)
{
  dstmem (cc | (((cc & 0x90) == 0x90) ? 0 : (1 << 26)));
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
