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

static void expr (int pri);

static void
prim (void)
{
  Lex lex = Lex_GetPrim ();

  switch (lex.tag)
    {
      case LexId:
	Code_Symbol (Symbol_Get (&lex), 0);
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
      case LexString:
        Code_String (lex.Data.String.str, lex.Data.String.len);
        break;
      case LexFloat:
        Code_Float (lex.Data.Float.value);
        break;
      case LexStorage:
        Code_Storage ();
        break;
      case LexPosition:
        Code_Position (areaCurrentSymbol, areaCurrentSymbol->area->curIdx);
        break;
      case LexOperator:
        prim ();
        if (Lex_IsUnop (lex.Data.Operator.op))
	  Code_Operator (lex.Data.Operator.op);
        else
	  Error (ErrorAbort, "Illegal unary operator");
        break;
      case LexDelim:
        if (lex.Data.Delim.delim == '(')
	  {
	    expr (1);
	    lex = Lex_GetPrim ();
	    if (lex.tag != LexDelim || lex.Data.Delim.delim != ')')
	      Error (ErrorError, "Missing ')'");
	  }
        else if (lex.Data.Delim.delim == ')')
	  Error (ErrorError, "Missing '('");
        else
	  Error (ErrorError, "Illegal delimiter '%c'", lex.Data.Delim.delim);
        break;
      case LexBool:
        Code_Bool (lex.Data.Bool.value);
        break;
      default:
        assert (0);
        break;
    }
}


static void
expr (int pri)
{
  if (pri == kPrioOp_Max)
    prim ();
  else
    expr (pri + 1);

  while (Lex_NextPri () == pri)
    {
      Lex op = Lex_GetBinop ();
      if (pri == kPrioOp_Max)
	prim ();
      else
	expr (pri + 1);
      Code_Operator (op.Data.Operator.op);
    }
}


/**
 * Parses the expression found at the current input and builds up an
 * (code) expression stream.
 */
void
Expr_Build (void)
{
  Code_Init ();
  expr (kPrioOp_Min);
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
  Expr_Build ();
  return Expr_Eval (legal);
}
