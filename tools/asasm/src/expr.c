/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2013 GCCSDK Developers
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
 * expr.c
 */

#include "config.h"

#include <assert.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif

#include "area.h"
#include "code.h"
#include "error.h"
#include "expr.h"
#include "global.h"
#include "lex.h"
#include "symbol.h"

static bool expr (int pri);

static bool
prim (void)
{
  bool failed = false;
  const Lex lex = Lex_GetPrim ();
  switch (lex.tag)
    {
      case LexId:
	Code_Symbol (Symbol_Get (&lex), 0);
        break;

      case LexLocalLabel:
	assert (0);
	break;

      case LexString:
        Code_String (lex.Data.String.str, lex.Data.String.len);
        break;

      case LexInt:
        Code_Int (lex.Data.Int.value);
        break;

      case LexInt64:
	{
	  const Value valInt64 = Value_Int64 (lex.Data.Int64.value);
	  Code_Value (&valInt64, false);
	  break;
	}

      case LexFloat:
        Code_Float (lex.Data.Float.value);
        break;

      case LexOperator:
        failed = prim ();
	if (!failed)
	  {
	    if (Lex_IsUnop (lex.Data.Operator.op))
	      Code_Operator (lex.Data.Operator.op);
	    else
	      {
		Error (ErrorError, "Illegal unary operator");
		failed = true;
	      }
	  }
        break;

      case LexPosition:
        Code_Position (areaCurrentSymbol, areaCurrentSymbol->area->curIdx);
        break;

      case LexStorage:
        Code_Storage ();
        break;

      case LexDelim:
        switch (lex.Data.Delim.delim)
	  {
	    case '(':
	      {
		if (expr (kPrioOp_Min))
		  return true;
		/* Check closing ')'.  */
		const Lex lexNext = Lex_GetPrim ();
		if (lexNext.tag != LexDelim || lexNext.Data.Delim.delim != ')')
		  {
		    Error (ErrorError, "Missing ')'");
		    failed = true;
		  }
		break;
	      }

	    case ')':
	      Error (ErrorError, "Missing '('");
	      failed = true;
	      break;

	    default:
	      assert (0);
	      break;
	  }
        break;

      case LexBool:
        Code_Bool (lex.Data.Bool.value);
        break;

      default:
        assert (0);
      case LexNone:
	/* When Lex encountered an error.  */
	failed = true;
	break;
    }
  return failed;
}


static bool
expr (int pri)
{
  if (pri == kPrioOp_Max ? prim () : expr (pri + 1))
    return true;

  int nextPri;
  while ((nextPri = Lex_NextPri ()) == pri)
    {
      Lex op = Lex_GetBinop ();
      if (op.tag != LexOperator || pri == kPrioOp_Max ? prim () : expr (pri + 1))
	return true;
      Code_Operator (op.Data.Operator.op);
    }
  return nextPri == -1;
}


/**
 * Parses the expression found at the current input and builds up an
 * (code) expression stream.
 */
bool
Expr_Build (void)
{
  Code_Init ();
  return expr (kPrioOp_Min);
}


/**
 * Evaluates the current (code) expression stream.
 * \param legal Or'd ValueTag values which are allowed as result.
 * \return Result of evaluation.  Use Value_Assign() to keep a non-temporary
 * copy of it.
 */
const Value *
Expr_Eval (ValueTag legal)
{
  return Code_Eval (legal, NULL);
}


/**
 * \return Result of evaluation.  Only to be used before next evaluation.
 * Use Value_Assign() to keep a non-temporary copy of it.
 */
const Value *
Expr_BuildAndEval (ValueTag legal)
{
  if (Expr_Build ())
    {
      static const Value illValue = { .Tag = ValueIllegal };
      return &illValue;
    }
  return Expr_Eval (legal);
}
