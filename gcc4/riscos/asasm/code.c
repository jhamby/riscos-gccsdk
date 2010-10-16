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
 * code.c
 */

#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "aoffile.h"
#include "area.h"
#include "code.h"
#include "error.h"
#include "eval.h"
#include "input.h"
#include "main.h"
#include "option.h"
#include "storage.h"

static Code Program[CODE_SIZECODE];
static int FirstFreeIns;
static Value Stack[CODE_SIZESTACK];

static LateInfo LateHeap[CODE_SIZELATE];
static int LateHeapPtr;

static int Sp;			/* Used by codeEvalLow and codeEvalLowest */

bool exprNotConst;


LateInfo *
codeNewLateInfo (Symbol *symbol)
{
  if (LateHeapPtr >= CODE_SIZELATE)
    errorOutOfMem ();

  LateHeap[LateHeapPtr].next = 0;
  LateHeap[LateHeapPtr].factor = 1;
  LateHeap[LateHeapPtr].symbol = symbol;
  return &LateHeap[LateHeapPtr++];
}

void
codeInit (void)
{
  FirstFreeIns = LateHeapPtr = 0;
  exprNotConst = false;
}

void
codeOperator (Operator op)
{
  if (FirstFreeIns < CODE_SIZECODE)
    {
      Program[FirstFreeIns].Tag = CodeOperator;
      Program[FirstFreeIns++].CodeOperator.op = op;
    }
  else
    errorAbort ("Internal codeOp: overflow");
}

void
codeSymbol (Symbol *symbol)
{
  if (FirstFreeIns < CODE_SIZECODE)
    {
      if ((symbol->type & SYMBOL_DEFINED) && !(symbol->type & SYMBOL_AREA))
	{
	  if (symbol->value.Tag.v != ValueIllegal)
	    exprNotConst = true;
	  switch (symbol->value.Tag.t)
	    {
	    case ValueInt:
	    case ValueFloat:
	    case ValueString:
	    case ValueBool:
	    case ValueAddr:
	      Program[FirstFreeIns].Tag = CodeValue;
	      Program[FirstFreeIns++].CodeValue.value = symbol->value;
	      symbol->used = 0; /* Mark as used.  */
	      break;
	    case ValueCode:
	      if (FirstFreeIns + symbol->value.ValueCode.len <= CODE_SIZECODE)
		{
		  int i;
		  for (i = 0; i < symbol->value.ValueCode.len; i++)
		    Program[FirstFreeIns++] = symbol->value.ValueCode.c[i];
		  symbol->used = 0; /* Mark as used.  */
		}
	      else
		errorAbort ("Internal codeSymbol: overflow (n)");
	      break;
	    default:
	      errorAbort ("Internal codeSymbol: illegal symbol value");
	      break;
	    }
	}
      else
	{
	  Program[FirstFreeIns].Tag = CodeSymbol;
	  Program[FirstFreeIns++].CodeSymbol.symbol = symbol;
	  symbol->used = 0; /* Mark as used.  */
	}
    }
  else
    errorAbort ("Internal codeSymbol: overflow (1)");
}

void
codePosition (Symbol *area)
{
  if (area)
    {
      codeSymbol (area);
      codeInt (area->value.ValueInt.i);
      codeOperator (Op_add);
    }
  else
    errorAbort ("'.' found, but no area is defined");
}

void
codeStorage (void)
{
  Value value = storageValue ();	/* Must be ValueInt */
  codeInt (value.ValueInt.i);
}

void
codeString (int len, const char *str)
{
  if (FirstFreeIns < CODE_SIZECODE)
    {
      Program[FirstFreeIns].Tag = CodeValue;
      Program[FirstFreeIns].CodeValue.value.Tag.t = ValueString;
      Program[FirstFreeIns].CodeValue.value.ValueString.len = len;
      Program[FirstFreeIns++].CodeValue.value.ValueString.s = str;
    }
  else
    errorAbort ("Internal codeString: overflow");
}

void
codeInt (int value)
{
  if (FirstFreeIns < CODE_SIZECODE)
    {
      Program[FirstFreeIns].Tag = CodeValue;
      Program[FirstFreeIns].CodeValue.value.Tag.t = ValueInt;
      Program[FirstFreeIns++].CodeValue.value.ValueInt.i = value;
    }
  else
    errorAbort ("Internal codeInt: overflow");
}

void
codeFloat (ARMFloat value)
{
  if (FirstFreeIns < CODE_SIZECODE)
    {
      Program[FirstFreeIns].Tag = CodeValue;
      Program[FirstFreeIns].CodeValue.value.Tag.t = ValueFloat;
      Program[FirstFreeIns++].CodeValue.value.ValueFloat.f = value;
    }
  else
    errorAbort ("Internal codeFloat: overflow");
}

void
codeBool (bool value)
{
  if (FirstFreeIns < CODE_SIZECODE)
    {
      Program[FirstFreeIns].Tag = CodeValue;
      Program[FirstFreeIns].CodeValue.value.Tag.t = ValueBool;
      Program[FirstFreeIns++].CodeValue.value.ValueBool.b = value;
    }
  else
    errorAbort ("Internal codeBool: overflow");
}


/* true if codeEvalLowest succeeded */
static bool
codeEvalLowest (int size, const Code *program)
{
  int Pp;
  for (Pp = 0; Pp < size; Pp++)
    {
      switch (program[Pp].Tag)
	{
	case CodeOperator:
	  if (isUnop (program[Pp].CodeOperator.op))
	    {
	      if (!evalUnop (program[Pp].CodeOperator.op, &Stack[Sp]))
		return false;
	    }
	  else
	    {
	      if (!evalBinop (program[Pp].CodeOperator.op, &Stack[Sp - 1], &Stack[Sp]))
		return false;
	      Sp--;
	    }
	  break;
	case CodeValue:
	  Stack[++Sp] = program[Pp].CodeValue.value;
	  break;
	case CodeSymbol:
	  if ((program[Pp].CodeSymbol.symbol->type & SYMBOL_DEFINED)
	      && !(program[Pp].CodeSymbol.symbol->type & SYMBOL_AREA))
	    {
	      /* This can happen in the relocation phase. */
	      switch (program[Pp].CodeSymbol.symbol->value.Tag.t)
		{
		case ValueInt:
		case ValueFloat:
		case ValueString:
		case ValueBool:
		  Stack[++Sp] = program[Pp].CodeSymbol.symbol->value;
		  break;
		case ValueCode:
		  if (!codeEvalLowest (program[Pp].CodeSymbol.symbol->value.ValueCode.len,
				       program[Pp].CodeSymbol.symbol->value.ValueCode.c))
		    return false;
		  break;
		case ValueAddr:
		  errorAbort ("Register offset labels must be defined before use");
		  break;
		default:
		  errorAbort ("Internal codeEvalLow: illegal value for symbol %s",
			      program[Pp].CodeSymbol.symbol->str);
		  break;
		}
	    }
	  else
	    {			/* Undefined or area or late register */
	      Stack[++Sp].Tag.t = ValueLateLabel;
	      Stack[Sp].ValueLate.i = 0;
	      Stack[Sp].ValueLate.late = codeNewLateInfo (program[Pp].CodeSymbol.symbol);
	    }
	  break;
	default:
	  errorAbort ("Internal codeEvalLow: illegal expression");
	  break;
	}
    }
  return true;
}

Value
codeEval (ValueTag legal)
{
  return codeEvalLow (legal, FirstFreeIns, Program);
}

Value
codeEvalLow (ValueTag legal, int size, Code *program)
{
  Value Result;
  Sp = -1;
  if (!codeEvalLowest (size, program))
    {
      Result.Tag.t = ValueCode;
      Result.ValueCode.len = size;
      Result.ValueCode.c = program;
    }
  else if (Sp == 0)
    Result = Stack[0];
  else
    Result.Tag.t = ValueIllegal;

  if (!(Result.Tag.t & legal))
    {
      if (option_autocast && (legal & ValueFloat) && Result.Tag.t == ValueInt)
	{
	  ARMFloat f = Result.ValueInt.i;
	  if (option_fussy > 1)
	    error (ErrorInfo, "Changing integer %d to float %1.1f", Result.ValueInt.i, f);
	  Result.Tag.t = ValueFloat;
	  Result.ValueFloat.f = f;
	}
      else
	Result.Tag.t = ValueIllegal;
    }
  return Result;
}

Code *
codeCopy (int len, const Code *code)
{
  Code *newCode;
  if ((newCode = malloc (len * sizeof (Code))) == NULL)
    errorOutOfMem ();

  for (int i = 0; i < len; i++)
    {
      switch (newCode[i].Tag = code[i].Tag)
	{
	  case CodeOperator:
	    newCode[i].CodeOperator.op = code[i].CodeOperator.op;
	    break;
	  case CodeValue:
	    newCode[i].CodeValue.value = valueCopy (code[i].CodeValue.value);
	    break;
	  case CodeSymbol:
	    newCode[i].CodeSymbol.symbol = code[i].CodeSymbol.symbol;
	    break;
	}
    }
  return newCode;
}

bool
codeEqual (int len, const Code *a, const Code *b)
{
  int i;
  for (i = 0; i < len; i++)
    {
      if (a[i].Tag != b[i].Tag)
	return false;
      switch (a[i].Tag)
	{
	case CodeOperator:
	  if (a[i].CodeOperator.op != b[i].CodeOperator.op)
	    return false;
	  break;
	case CodeValue:
	  if (!valueEqual (&a[i].CodeValue.value, &b[i].CodeValue.value))
	    return false;
	  break;
	case CodeSymbol:
	  if (a[i].CodeSymbol.symbol != b[i].CodeSymbol.symbol)
	    return false;
	  break;
	}
    }
  return true;
}
