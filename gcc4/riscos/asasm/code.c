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

#include <assert.h>
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

#ifdef DEBUG
#  define DEBUG_CODE
#endif

#ifdef DEBUG_CODE
static void codePrint (int size, const Code *program);
#endif

#define CODE_SIZECODE  (1024)
#define CODE_SIZESTACK (1024)
#define CODE_SIZELATE  (1024)

static Code Program[CODE_SIZECODE];
static int FirstFreeIns;
static Value Stack[CODE_SIZESTACK];

static LateInfo LateHeap[CODE_SIZELATE];
static int LateHeapPtr;

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
	  switch (symbol->value.Tag)
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
	      if (FirstFreeIns + symbol->value.Data.Code.len <= CODE_SIZECODE)
		{
		  for (int i = 0; i < symbol->value.Data.Code.len; i++)
		    Program[FirstFreeIns++] = symbol->value.Data.Code.c[i];
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
      codeInt (area->value.Data.Int.i);
      codeOperator (Op_add);
    }
  else
    errorAbort ("'.' found, but no area is defined");
}

void
codeStorage (void)
{
  if (FirstFreeIns < CODE_SIZECODE)
    {
      Program[FirstFreeIns].Tag = CodeValue;
      Program[FirstFreeIns++].CodeValue.value = storageValue ();
    }
  else
    errorAbort ("Internal codeStorage: overflow");
}

void
codeString (int len, const char *str)
{
  if (FirstFreeIns < CODE_SIZECODE)
    {
      Program[FirstFreeIns].Tag = CodeValue;
      Program[FirstFreeIns].CodeValue.value.Tag = ValueString;
      Program[FirstFreeIns].CodeValue.value.Data.String.len = len;
      Program[FirstFreeIns++].CodeValue.value.Data.String.s = str;
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
      Program[FirstFreeIns].CodeValue.value.Tag = ValueInt;
      Program[FirstFreeIns++].CodeValue.value.Data.Int.i = value;
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
      Program[FirstFreeIns].CodeValue.value.Tag = ValueFloat;
      Program[FirstFreeIns++].CodeValue.value.Data.Float.f = value;
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
      Program[FirstFreeIns].CodeValue.value.Tag = ValueBool;
      Program[FirstFreeIns++].CodeValue.value.Data.Bool.b = value;
    }
  else
    errorAbort ("Internal codeBool: overflow");
}


/**
 * \param sp Full incrementing stack index.  On entry and first use of
 * codeEvalLowest this needs to be -1.  On exit, when 0 then Stack[0] is result,
 * otherwise there is no result.
 * \return true if succeeded.
 */
static bool
codeEvalLowest (int size, const Code *program, int *sp)
{
#ifdef DEBUG_CODE
  printf ("vvv codeEvalLowest\n");
#endif
  for (int i = 0; i < size; i++)
    {
#ifdef DEBUG_CODE
      /* Dump stack entries.  */
      printf ("Stack: ");
      for (int s = 0; s <= *sp; ++s)
	{
	  printf ("[");
	  valuePrint (&Stack[s]);
	  printf ("] ");
	}
      printf (" : doing operation ");
#endif
      switch (program[i].Tag)
	{
	  case CodeOperator:
	    if (isUnop (program[i].CodeOperator.op))
	      {
#ifdef DEBUG_CODE
		printf ("[%s] ", OperatorAsStr (program[i].CodeOperator.op));
#endif
		assert (*sp >= 0);
		if (!evalUnop (program[i].CodeOperator.op, &Stack[*sp]))
		  return false;
		/* No stack adjusting is needed, as one operand is consumed,
		   one result is produced.  */
	      }
	    else
	      {
#ifdef DEBUG_CODE
		printf ("[%s] ", OperatorAsStr (program[i].CodeOperator.op));
#endif
		assert (*sp - 1 >= 0);
		if (!evalBinop (program[i].CodeOperator.op, &Stack[*sp - 1], &Stack[*sp]))
		  return false;
		/* Stack adjusting by one, as two operands are consumed, one
		   result is produced.  */
		--(*sp);
	      }
	    break;

	  case CodeValue:
#ifdef DEBUG_CODE
	    printf ("[Push: ");
	    valuePrint (&program[i].CodeValue.value);
	    printf ("] ");
#endif
	    Stack[++(*sp)] = program[i].CodeValue.value;
	    break;

	  case CodeSymbol:
	    if ((program[i].CodeSymbol.symbol->type & SYMBOL_DEFINED)
	        && !(program[i].CodeSymbol.symbol->type & SYMBOL_AREA))
	      {
		/* This can happen in the relocation phase.  */
		switch (program[i].CodeSymbol.symbol->value.Tag)
		  {
		    case ValueInt:
		    case ValueFloat:
		    case ValueString:
		    case ValueBool:
#ifdef DEBUG_CODE
		      printf ("[Symbol %.*s value push: ",
			      program[i].CodeSymbol.symbol->len,
			      program[i].CodeSymbol.symbol->str);
		      valuePrint (&program[i].CodeSymbol.symbol->value);
		      printf ("] ");
#endif
		      Stack[++(*sp)] = program[i].CodeSymbol.symbol->value;
		      break;

		    case ValueCode:
#ifdef DEBUG_CODE
		      printf ("[Code]...\n");
#endif
		      if (!codeEvalLowest (program[i].CodeSymbol.symbol->value.Data.Code.len,
					   program[i].CodeSymbol.symbol->value.Data.Code.c,
					   sp))
			return false;
		      break;

		    case ValueAddr:
		      errorAbort ("Register offset labels must be defined before use");
		      break;

		    default:
		      errorAbort ("Internal codeEvalLow: illegal value for symbol %s",
				  program[i].CodeSymbol.symbol->str);
		      break;
		  }
	      }
	    else
	      { /* Undefined or area or late register.  */
#ifdef DEBUG_CODE
		printf ("[Late symbol %.*s value push] ",
			program[i].CodeSymbol.symbol->len,
			program[i].CodeSymbol.symbol->str);
#endif
		Stack[++(*sp)].Tag = ValueLateLabel;
		Stack[*sp].Data.Late.i = 0;
		Stack[*sp].Data.Late.late = codeNewLateInfo (program[i].CodeSymbol.symbol);
	      }
	    break;

	  default:
	    errorAbort ("Internal codeEvalLow: illegal expression");
	    break;
	}
#ifdef DEBUG_CODE
      printf ("\n");
#endif
    }
#ifdef DEBUG_CODE
  printf ("Final stack result: ");
  for (int s = 0; s <= *sp; ++s)
    {
      printf ("[");
      valuePrint (&Stack[s]);
      printf ("] ");
    }
  printf ("\n^^^\n");
#endif
    
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
#ifdef DEBUG_CODE
  printf ("Code: ");
  codePrint (size, program);
  printf ("\n");
#endif

  int sp = -1;
  Value Result;
  if (!codeEvalLowest (size, program, &sp))
    {
      Result.Tag = ValueCode;
      Result.Data.Code.len = size;
      Result.Data.Code.c = program;
    }
  else if (sp == 0)
    Result = Stack[0];
  else
    {
      assert (0); /* <= FIXME: I've added a CantHappen as I would like to know if this possible.  */
      Result.Tag = ValueIllegal;
    }

  if (!(Result.Tag & legal))
    {
      if (option_autocast && (legal & ValueFloat) && Result.Tag == ValueInt)
	{
	  ARMFloat f = Result.Data.Int.i;
	  if (option_fussy > 1)
	    error (ErrorInfo, "Changing integer %d to float %1.1f", Result.Data.Int.i, f);
	  Result.Tag = ValueFloat;
	  Result.Data.Float.f = f;
	}
      else
	Result.Tag = ValueIllegal;
    }
#ifdef DEBUG_CODE
  printf ("Code result:\n");
  valuePrint (&Result);
#endif

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
  for (int i = 0; i < len; i++)
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

#ifdef DEBUG_CODE
static void
codePrint (int size, const Code *program)
{
  for (int i = 0; i < size; i++)
    {
      switch (program[i].Tag)
	{
	  case CodeOperator:
	    if (isUnop (program[i].CodeOperator.op))
	      printf ("[%s] ", OperatorAsStr (program[i].CodeOperator.op));
	    else
	      printf ("[%s] ", OperatorAsStr (program[i].CodeOperator.op));
	    break;

	  case CodeValue:
	    printf ("[");
	    valuePrint (&program[i].CodeValue.value);
	    printf ("] ");
	    break;

	  case CodeSymbol:
	    if ((program[i].CodeSymbol.symbol->type & SYMBOL_DEFINED)
	        && !(program[i].CodeSymbol.symbol->type & SYMBOL_AREA))
	      {
		switch (program[i].CodeSymbol.symbol->value.Tag)
		  {
		    case ValueInt:
		    case ValueFloat:
		    case ValueString:
		    case ValueBool:
		      printf ("[Symbol %.*s : ",
			      program[i].CodeSymbol.symbol->len,
			      program[i].CodeSymbol.symbol->str);
		      valuePrint (&program[i].CodeSymbol.symbol->value);
		      printf ("] ");
		      break;

		    case ValueCode:
		      printf ("[Code: << ");
		      codePrint (program[i].CodeSymbol.symbol->value.Data.Code.len,
				 program[i].CodeSymbol.symbol->value.Data.Code.c);
		      printf (">>] ");
		      break;
		  }
	      }
	    else
	      {
		printf ("[Late Symbol %.*s : ",
			program[i].CodeSymbol.symbol->len,
			program[i].CodeSymbol.symbol->str);
	      }
	    break;
	}
    }
}
#endif
