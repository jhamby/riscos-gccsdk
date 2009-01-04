/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2008 GCCSDK Developers
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
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
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
      codeInt (lex.LexInt.value);
      break;
    case LexString:
      codeString (lex.LexString.len, lex.LexString.str);
      break;
    case LexFloat:
      codeFloat (lex.LexFloat.value);
      break;
    case LexStorage:
      codeStorage ();
      break;
    case LexPosition:
      codePosition (areaCurrentSymbol);
      break;
    case LexOperator:
      prim ();
      if (lex.LexOperator.op == Op_none);
      else if (isUnop (lex.LexOperator.op))
	codeOperator (lex.LexOperator.op);
      else
	error (ErrorError, TRUE, "Illegal unop");
      break;
    case LexDelim:
      if (lex.LexDelim.delim == '(')
	{
	  expr (1);
	  lex = lexGetPrim ();
	  if (lex.tag != LexDelim || lex.LexDelim.delim != ')')
	    error (ErrorError, TRUE, "Missing ')'");
	}
      else if (lex.LexDelim.delim == ')')
	{
	  error (ErrorError, TRUE, "Missing '('");
	}
      else
	error (ErrorError, TRUE, "Illegal delimiter '%c'", lex.LexDelim.delim);
      break;
    case LexBool:
      codeBool (lex.LexBool.value);
      break;
    default:
      error (ErrorError, TRUE, "Illegal expression");
      break;
    }
}


static void
expr (int pri)
{
  Lex op;

  if (pri == 10)
    prim ();
  else
    expr (pri + 1);

  while (lexNextPri () == pri)
    {
      op = lexGetBinop ();
      if (pri == 10)
	prim ();
      else
	expr (pri + 1);
      codeOperator (op.LexOperator.op);
    }
}


void
exprBuild (void)
{
  codeInit ();
  expr (1);
}


Value
exprEval (ValueTag legal)
{
  return codeEval (legal);
}
