/*
 * AsAsm an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2014 GCCSDK Developers
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
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
#include "directive_storagemap.h"
#include "error.h"
#include "expr.h"
#include "global.h"
#include "input.h"
#include "lex.h"
#include "symbol.h"

static bool Expr_Get (void);

static bool
Expr_GetPrim (void)
{
  bool failed = false;
  const Lex lex = Lex_GetPrim ();
  switch (lex.tag)
    {
      case LexId:
	Code_Symbol (Symbol_Get (lex.Data.Id.str, lex.Data.Id.len), 0);
        break;

      case LexLocalLabel:
	assert (0);
	break;

      case LexString:
        Code_String (lex.Data.String.str, lex.Data.String.len, lex.Data.String.owns);
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
	/* All unary operators have same priority (kPrioOp_Unary), so we can
	   keep on calling Expr_GetPrim() recursively.  */
	if (!Lex_IsUnop (lex.Data.Operator.op))
	  {
	    Error (ErrorError, "Illegal operator");
	    failed = true;
	  }
	else if ((failed = Expr_GetPrim ()) == false)
	  Code_Operator (lex.Data.Operator.op);
        break;

      case LexPosition:
	Code_Symbol (areaCurrentSymbol, areaCurrentSymbol->attr.area->curIdx);
        break;

      case LexStorage:
	Code_Value (StorageMap_Value (), true);
        break;

      case LexDelim:
        switch (lex.Data.Delim.delim)
	  {
	    case '(':
	      {
		if (Expr_Get ())
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
	      Input_UnGetC (')');
	      Error (ErrorError, "')' unexpected");
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
	Error (ErrorError, "Incomplete expression");
	failed = true;
	break;
    }

  return failed;
}


/**
 * Builds expression with prim & binops with priority >= maxPri.
 */
static bool
Expr_GetInt (Lex *opP, int maxPri)
{
  if (maxPri != kPrioOp_Max ? Expr_GetInt (opP, maxPri + 1) : Expr_GetPrim ())
    return true;

  while (1)
    {
      if (opP->tag != LexOperator)
	{
	  *opP = Lex_GetBinop ();
	  if (opP->tag != LexOperator)
	    break; /* Done.  */
	}

      if (opP->Data.Operator.pri != maxPri)
	break;

      Lex curOp = *opP;
      opP->tag = LexNone;

      if (maxPri != kPrioOp_Max ? Expr_GetInt (opP, maxPri + 1) : Expr_GetPrim ())
	return true;
      
      Code_Operator (curOp.Data.Operator.op);
    }

  return false;
}


static bool
Expr_Get (void)
{
  Lex nop = { .tag = LexNone };
  return Expr_GetInt (&nop, kPrioOp_Min);
}


/**
 * Parses the expression found at the current input and builds up an
 * (code) expression stream.
 */
bool
Expr_Build (void)
{
  Code_Init ();
  return Expr_Get ();
}


/**
 * Evaluates the current (code) expression stream which was previous built up
 * using Expr_Build (or via Code_Init() + Code_*() calls).
 * \param legal Or'd ValueTag values which are allowed as result.
 * \return Result of evaluation.
 * Use Value_Assign()/Value_Copy() to keep a non-temporary copy of it.
 */
const Value *
Expr_Eval (ValueTag legal)
{
  return Code_Eval (legal);
}


/**
 * \return Result of evaluation.  Only to be used before next evaluation.
 * Use Value_Assign()/Value_Copy() to keep a non-temporary copy of it.
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
