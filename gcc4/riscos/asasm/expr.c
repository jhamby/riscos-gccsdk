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
 * expr.c
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
#include "global.h"
#include "lex.h"
#include "symbol.h"

static void expr (int pri);

static void
prim (void)
{
  Lex lex = lexGetPrim ();

  switch (lex.tag)
    {
      case LexId:
	codeSymbol (symbolGet (&lex));
        break;
      case LexInt:
        codeInt (lex.Data.Int.value);
        break;
      case LexString:
        codeString (lex.Data.String.str, lex.Data.String.len);
        break;
      case LexFloat:
        codeFloat (lex.Data.Float.value);
        break;
      case LexStorage:
        codeStorage ();
        break;
      case LexPosition:
        codePosition (areaCurrentSymbol, areaCurrentSymbol->value.Data.Int.i);
        break;
      case LexOperator:
        prim ();
        if (lex.Data.Operator.op == Op_none)
  	  /* */;
        else if (isUnop (lex.Data.Operator.op))
	  codeOperator (lex.Data.Operator.op);
        else
	  error (ErrorError, "Illegal unop");
        break;
      case LexDelim:
        if (lex.Data.Delim.delim == '(')
	  {
	    expr (1);
	    lex = lexGetPrim ();
	    if (lex.tag != LexDelim || lex.Data.Delim.delim != ')')
	      error (ErrorError, "Missing ')'");
	  }
        else if (lex.Data.Delim.delim == ')')
	  error (ErrorError, "Missing '('");
        else
	  error (ErrorError, "Illegal delimiter '%c'", lex.Data.Delim.delim);
        break;
      case LexBool:
        codeBool (lex.Data.Bool.value);
        break;
      default:
        error (ErrorError, "Illegal expression");
        break;
    }
}


static void
expr (int pri)
{
  if (pri == 10)
    prim ();
  else
    expr (pri + 1);

  while (lexNextPri () == pri)
    {
      Lex op = lexGetBinop ();
      if (pri == 10)
	prim ();
      else
	expr (pri + 1);
      codeOperator (op.Data.Operator.op);
    }
}


/**
 * Parses the expression found at the current input and builds up an
 * (code) expression stream.
 */
void
exprBuild (void)
{
  codeInit ();
  expr (1);
}


/**
 * Evaluates the current (code) expression stream.
 * \param legal Or'd ValueTag values which are allowed as result.
 * \return Result of evaluation.  Use Value_Assign() to keep a non-temporary
 * copy of it.
 */
const Value *
exprEval (ValueTag legal)
{
  return codeEval (legal, NULL);
}


/**
 * \return Result of evaluation.  Only to be used before next evaluation.
 * Use Value_Assign() to keep a non-temporary copy of it.
 */
const Value *
exprBuildAndEval (ValueTag legal)
{
  exprBuild ();
  return exprEval (legal);
}
