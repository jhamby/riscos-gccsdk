/*
 * code.c
 * Copyright © 1992 Niklas Röjemo
 */
#include "sdk-config.h"
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "code.h"
#include "error.h"
#include "eval.h"
#include "area.h"
#include "option.h"
#include "input.h"
#include "aoffile.h"
#include "storage.h"

static Code Program[CODE_SIZECODE];
static int FirstFreeIns;
static Value Stack[CODE_SIZESTACK];

static LateInfo LateHeap[CODE_SIZELATE];
static int LateHeapPtr;

BOOL exprNotConst;


LateInfo *
codeNewLateInfo (Symbol * symbol)
{
  if (LateHeapPtr < CODE_SIZELATE)
    {
      LateHeap[LateHeapPtr].next = 0;
      LateHeap[LateHeapPtr].factor = 1;
      LateHeap[LateHeapPtr].symbol = symbol;
    }
  else
    errorOutOfMem ("codeNewLateInfo");
  return &LateHeap[LateHeapPtr++];
}

void 
codeInit (void)
{
  FirstFreeIns = LateHeapPtr = 0;
  exprNotConst = FALSE;
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
    error (ErrorSerious, FALSE, "Internal codeOp: overflow");
}

void 
codeSymbol (Symbol * symbol)
{
  if (FirstFreeIns < CODE_SIZECODE)
    {
      if (symbol->type & SYMBOL_DEFINED && !(symbol->type & SYMBOL_AREA))
	{
	  if (symbol->value.Tag.v != ValueIllegal)
	    exprNotConst = TRUE;
	  switch (symbol->value.Tag.t)
	    {
	    case ValueInt:
	    case ValueFloat:
	    case ValueString:
	    case ValueBool:
	    case ValueAddr:
	      Program[FirstFreeIns].Tag = CodeValue;
	      Program[FirstFreeIns++].CodeValue.value = symbol->value;
	      symbol->used = 0;	/* Mark as used.  */
	      break;
	    case ValueCode:
	      if (FirstFreeIns + symbol->value.ValueCode.len <= CODE_SIZECODE)
		{
		  int i;
		  for (i = 0; i < symbol->value.ValueCode.len; i++)
		    Program[FirstFreeIns++] = symbol->value.ValueCode.c[i];
		  symbol->used = 0;
		}
	      else
		error (ErrorSerious, FALSE, "Internal codeSymbol: overflow (n)");
	      break;
	    default:
	      error (ErrorSerious, FALSE, "Internal codeSymbol: illegal symbol value");
	    }
	}
      else
	{
	  Program[FirstFreeIns].Tag = CodeSymbol;
	  Program[FirstFreeIns++].CodeSymbol.symbol = symbol;
	  symbol->used = 0;
	}
    }
  else
    error (ErrorSerious, FALSE, "Internal codeSymbol: overflow (1)");
}

void 
codePosition (Symbol * area)
{
  if (area)
    {
      codeSymbol (area);
      codeInt (area->value.ValueInt.i);
      codeOperator (Op_add);
    }
  else
    error (ErrorError, FALSE, "'.' found, but no area is defined");
}

void 
codeStorage (void)
{
  Value value = storageValue ();	/* Must be ValueInt */
  codeInt (value.ValueInt.i);
}

void 
codeString (int len, char *str)
{
  if (FirstFreeIns < CODE_SIZECODE)
    {
      Program[FirstFreeIns].Tag = CodeValue;
      Program[FirstFreeIns].CodeValue.value.Tag.t = ValueString;
      Program[FirstFreeIns].CodeValue.value.ValueString.len = len;
      Program[FirstFreeIns++].CodeValue.value.ValueString.s = str;
    }
  else
    error (ErrorSerious, FALSE, "Internal codeString: overflow");
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
    error (ErrorSerious, FALSE, "Internal codeInt: overflow");
}

void 
codeFloat (FLOAT value)
{
  if (FirstFreeIns < CODE_SIZECODE)
    {
      Program[FirstFreeIns].Tag = CodeValue;
      Program[FirstFreeIns].CodeValue.value.Tag.t = ValueFloat;
      Program[FirstFreeIns++].CodeValue.value.ValueFloat.f = value;
    }
  else
    error (ErrorSerious, FALSE, "Internal codeFloat: overflow");
}

void 
codeBool (BOOL value)
{
  if (FirstFreeIns < CODE_SIZECODE)
    {
      Program[FirstFreeIns].Tag = CodeValue;
      Program[FirstFreeIns].CodeValue.value.Tag.t = ValueBool;
      Program[FirstFreeIns++].CodeValue.value.ValueBool.b = value;
    }
  else
    error (ErrorSerious, FALSE, "Internal codeBool: overflow");
}


int Sp;				/* Used by codeEvalLow and codeEvalLowest */

static BOOL 
codeEvalLowest (int size, Code * program)
/* True if codeEvalLowest succeeded */
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
		return FALSE;
	    }
	  else
	    {
	      if (!evalBinop (program[Pp].CodeOperator.op, &Stack[Sp - 1], &Stack[Sp]))
		return FALSE;
	      Sp--;
	    }
	  break;
	case CodeValue:
	  Stack[++Sp] = program[Pp].CodeValue.value;
	  break;
	case CodeSymbol:
	  if (program[Pp].CodeSymbol.symbol->type & SYMBOL_DEFINED &&
	      !(program[Pp].CodeSymbol.symbol->type & SYMBOL_AREA))
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
		    return FALSE;
		  break;
		case ValueAddr:
		  error (ErrorSerious, FALSE,
		       "Register offset labels must be defined before use");
		  break;
		default:
		  error (ErrorSerious, FALSE,
			 "Internal codeEvalLow: illegal value for symbol %s",
			 program[Pp].CodeSymbol.symbol->str);
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
	  error (ErrorSerious, FALSE, "Internal codeEvalLow: illegal expression");
	}
    }
  return TRUE;
}

Value 
codeEval (ValueTag legal)
{
  return codeEvalLow (legal, FirstFreeIns, Program);
}

Value 
codeEvalLow (ValueTag legal, int size, Code * program)
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
      extern int fussy, autocast;
      if (autocast && legal & ValueFloat && Result.Tag.t == ValueInt)
	{
	  FLOAT f = Result.ValueInt.i;
	  if (fussy > 1)
	    error (ErrorInfo, TRUE, "Changing integer %d to float %1.1f", Result.ValueInt.i, f);
	  Result.Tag.t = ValueFloat;
	  Result.ValueFloat.f = f;
	}
      else
	Result.Tag.t = ValueIllegal;
    }
  return Result;
}

Code *
codeCopy (int len, Code * code)
{
  int i;
  Code *new = malloc (len * sizeof (Code));
  if (new)
    for (i = 0; i < len; i++)
      {
	switch (new[i].Tag = code[i].Tag)
	  {
	  case CodeOperator:
	    new[i].CodeOperator.op = code[i].CodeOperator.op;
	    break;
	  case CodeValue:
	    new[i].CodeValue.value = valueCopy (code[i].CodeValue.value);
	    break;
	  case CodeSymbol:
	    new[i].CodeSymbol.symbol = code[i].CodeSymbol.symbol;
	  }
      }
  else
    errorOutOfMem ("codeCopy");
  return new;
}

int 
codeEqual (int len, Code * a, Code * b)
{
  int i;
  for (i = 0; i < len; i++)
    {
      if (a[i].Tag != b[i].Tag)
	return FALSE;
      switch (a[i].Tag)
	{
	case CodeOperator:
	  if (a[i].CodeOperator.op != b[i].CodeOperator.op)
	    return FALSE;
	  break;
	case CodeValue:
	  if (!valueEqual (&a[i].CodeValue.value, &b[i].CodeValue.value))
	    return FALSE;
	  break;
	case CodeSymbol:
	  if (a[i].CodeSymbol.symbol != b[i].CodeSymbol.symbol)
	    return FALSE;
	  break;
	}
    }
  return TRUE;
}
