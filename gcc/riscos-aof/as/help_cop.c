
/*
 * help_cop.c
 * Copyright © 1992 Niklas Röjemo
 */

#include <stdint.h>
#include "error.h"
#include "expr.h"
#include "help_cop.h"
#include "input.h"
#include "area.h"
#include "get.h"
#include "option.h"
#include "value.h"
#include "code.h"
#include "fix.h"
#include "reloc.h"

int 
help_copInt (int max, char *msg)
{
  Value i;
  exprBuild ();
  i = exprEval (ValueInt);
  if (i.Tag.t == ValueInt)
    {
      if (i.ValueInt.i < 0 || i.ValueInt.i > max)
	{
	  error (ErrorError, TRUE, "%d is not a legal %s", i.ValueInt.i, msg);
	  return 0;
	}
    }
  else
    {
      error (ErrorError, TRUE, "Illegal expression as %s", msg);
      return 0;
    }
  return i.ValueInt.i;
}


WORD 
help_copAddr (WORD ir, BOOL stack)
{
  BOOL pre, up = TRUE;
  Value offset;
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
    case '[':
      inputSkip ();
      skipblanks ();
      ir |= LHS_OP (getCpuReg ());	/* Base register */
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
	  if (stack)
	    error (ErrorError, TRUE, "Cannot specify both offset and stack type");
	  inputSkip ();
	  skipblanks ();
	  switch (inputLook ())
	    {
	    case '-':
	      up = FALSE;
	    case '+':
	      inputSkip ();
	      skipblanks ();
	    }
	  if (inputLook () == '#')
	    {
	      inputSkip ();
	      exprBuild ();
	      if (!up)
		codeOperator (Op_neg);
	      offset = exprEval (ValueInt | ValueCode | ValueLateLabel);
	      if (!stack)
		{
		  switch (offset.Tag.t)
		    {
		    case ValueInt:
		      ir = fixCopOffset (inputLineNo, ir, offset.ValueInt.i);
		      break;
		    case ValueCode:
		    case ValueLateLabel:
		      relocCopOffset (ir, offset);
		      break;
		    default:
		      error (ErrorError, TRUE, "Illegal offset expression");
		    }
		  if (!pre)
		    ir |= WB_FLAG;	/* If postfix, set writeback */
		}
	    }
	  else if (!stack)
	    error (ErrorError, TRUE, "Coprocessor memory instructions cannot use register offset");
	  skipblanks ();
	}
      else
	{			/* cop_reg,[base] if this way */
	  if (pre)
	    error (ErrorError, TRUE, "Illegal character '%c' after base", inputLook ());
	  if (!stack)
	    ir |= UP_FLAG;	/* changes #-0 to #+0 :-) */
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
	  ir |= PRE_FLAG;
	}
      else if (!stack)
	ir |= WB_FLAG;		/* Ensure r is updated for [r],#x */
      if (inputLook () == '!')
	{
	  if (pre || stack)
	    ir |= WB_FLAG;
	  else
	    error (ErrorError, TRUE, "Writeback is implied with post-index");
	  inputSkip ();
	  skipblanks ();
	}
      break;
    case '=':
      inputSkip ();
      if (stack)
	error (ErrorError, TRUE, "Literal cannot be used when stack type is specified");
      ir |= PRE_FLAG | LHS_OP (15);
      exprBuild ();
      offset = exprEval (ValueInt | ValueFloat | ValueLateLabel);
      if (stack)
	break;
      switch (offset.Tag.t)
	{
	case ValueInt:
	  offset.Tag.t = ValueFloat;
	  offset.ValueFloat.f = offset.ValueInt.i;
	case ValueFloat:
	  switch (ir & PRECISION_MEM_PACKED)
	    {
	    case PRECISION_MEM_SINGLE:
	      litInt (4, offset);
	      break;
	    case PRECISION_MEM_DOUBLE:
	      litInt (8, offset);
	      break;
	    default:
	      error (ErrorWarning, TRUE,
		     "Extended and packed not supported; using double");
	      ir = (ir & ~PRECISION_MEM_PACKED) | PRECISION_MEM_DOUBLE;
	      litInt (8, offset);
	      /* note that litFloat==litInt, so there's no litFloat */
	    }
	  break;
	default:
	  abort ();
	}
      break;
    default:
      if (stack)
	error (ErrorError, TRUE, "Address cannot be used when stack type is specified");
      /*  cop_reg,Address */
      ir |= PRE_FLAG | LHS_OP (15);
      exprBuild ();
      codePosition (areaCurrent);
      codeOperator (Op_sub);
      codeInt (8);
      codeOperator (Op_sub);
      offset = exprEval (ValueInt | ValueCode | ValueLateLabel | ValueAddr);
      if (stack)
	break;
      switch (offset.Tag.t)
	{
	case ValueInt:
	  ir = fixCopOffset (inputLineNo, ir | LHS_OP (15), offset.ValueInt.i);
	  break;
	case ValueCode:
	case ValueLateLabel:
	  relocCopOffset (ir | LHS_OP (15), offset);
	  break;
	case ValueAddr:
	  ir = fixCopOffset (inputLineNo, ir | LHS_OP (offset.ValueAddr.r),
			     offset.ValueAddr.i);
	  break;
	default:
	  error (ErrorError, TRUE, "Illegal address expression");
	}
    }
  return ir;
}
