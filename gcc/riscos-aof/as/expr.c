
/*
 * expr.c
 * Copyright © 1992 Niklas Röjemo
 */
#include "sdk-config.h"
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "expr.h"
#include "lex.h"
#include "error.h"
#include "global.h"
#include "symbol.h"
#include "code.h"
#include "area.h"

static void expr (int pri);

static void 
prim (void)
{
  Lex lex = lexGetPrim ();

  switch (lex.tag)
    {
    case LexId:
      codeSymbol (symbolGet (lex));
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
      codePosition (areaCurrent);
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
