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
 * code.c
 */

#include "config.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif
#include <string.h>

#include "area.h"
#include "code.h"
#include "directive_storagemap.h"
#include "error.h"
#include "eval.h"
#include "input.h"
#include "main.h"

#ifdef DEBUG
//#  define DEBUG_CODE
#endif

#define CODE_SIZECODE  (1024)
#define CODE_SIZESTACK (1024)

static void Code_ExpandCurrAreaSymbolAsAddr (Value *value, ARMWord instrOffset);
static bool Code_EvalLowest (size_t size, const Code *program, const ARMWord *instrOffsetP, int *sp);

#ifdef DEBUG_CODE
static void Code_PrintValueArray (size_t size, const Value *value);
#endif

static Code Program[CODE_SIZECODE]; /**< No ownership of Value objects.  */
static unsigned FirstFreeIns; /**< Index for Program[] array for first free slot.  */
static Value Stack[CODE_SIZESTACK]; /**< Ownership of Value object.  */

void
Code_Init (void)
{
  FirstFreeIns = 0;
}

void
Code_Operator (Operator_e op)
{
  if (FirstFreeIns < CODE_SIZECODE)
    {
      Program[FirstFreeIns].Tag = CodeOperator;
      Program[FirstFreeIns].Data.op = op;
      ++FirstFreeIns;
    }
  else
    Error_Abort ("Internal Code_Operator: overflow");
}

void
Code_Symbol (Symbol *symbol, int offset)
{
  if (FirstFreeIns < CODE_SIZECODE)
    {
      /* Only during evaluation, we're going to expand the symbols.  */
      Program[FirstFreeIns].Tag = CodeValue;
      Program[FirstFreeIns].Data.value = Value_Symbol (symbol, 1, offset);
      ++FirstFreeIns;
    }
  else
    Error_Abort ("Internal Code_Symbol: overflow");
}

void
Code_Position (Symbol *area, int offset)
{
  if (area->area->type & AREA_ABS)
    Code_Int (Area_GetBaseAddress (area) + offset);
  else
    Code_Symbol (area, offset);
}

void
Code_Storage (void)
{
  if (FirstFreeIns < CODE_SIZECODE)
    {
      Program[FirstFreeIns].Tag = CodeValue;
      Program[FirstFreeIns].Data.value = *StorageMap_Value ();
      ++FirstFreeIns;
    }
  else
    Error_Abort ("Internal Code_Storage: overflow");
}

void
Code_String (const char *str, size_t len)
{
  if (FirstFreeIns < CODE_SIZECODE)
    {
      Program[FirstFreeIns].Tag = CodeValue;
      Program[FirstFreeIns].Data.value.Tag = ValueString;
      Program[FirstFreeIns].Data.value.Data.String.len = len;
      Program[FirstFreeIns].Data.value.Data.String.s = str;
      ++FirstFreeIns;
    }
  else
    Error_Abort ("Internal Code_String: overflow");
}

void
Code_Int (int value)
{
  if (FirstFreeIns < CODE_SIZECODE)
    {
      Program[FirstFreeIns].Tag = CodeValue;
      Program[FirstFreeIns].Data.value = Value_Int (value, eIntType_PureInt);
      ++FirstFreeIns;
    }
  else
    Error_Abort ("Internal Code_Int: overflow");
}

void
Code_Float (ARMFloat value)
{
  if (FirstFreeIns < CODE_SIZECODE)
    {
      Program[FirstFreeIns].Tag = CodeValue;
      Program[FirstFreeIns].Data.value = Value_Float (value);
      ++FirstFreeIns;
    }
  else
    Error_Abort ("Internal Code_Float: overflow");
}

void
Code_Bool (bool value)
{
  if (FirstFreeIns < CODE_SIZECODE)
    {
      Program[FirstFreeIns].Tag = CodeValue;
      Program[FirstFreeIns].Data.value = Value_Bool (value);
      ++FirstFreeIns;
    }
  else
    Error_Abort ("Internal Code_Bool: overflow");
}

/**
 * Adds "Push ValueAddr object" code element.
 */
void
Code_Addr (unsigned reg, int offset)
{
  if (FirstFreeIns < CODE_SIZECODE)
    {
      Program[FirstFreeIns].Tag = CodeValue;
      Program[FirstFreeIns].Data.value = Value_Addr (reg, offset);
      ++FirstFreeIns;
    }
  else
    Error_Abort ("Internal Code_Addr: overflow");
}

void
Code_Value (const Value *value, bool expCode)
{
  if (expCode && value->Tag == ValueCode)
    {
      if (value->Data.Code.len + FirstFreeIns < CODE_SIZECODE)
	{
	  memcpy (&Program[FirstFreeIns], value->Data.Code.c, value->Data.Code.len*sizeof (Code));
	  FirstFreeIns += value->Data.Code.len;
	}
      else
	Error_Abort ("Internal Code_Value: overflow");
    }
  else if (FirstFreeIns < CODE_SIZECODE)
    {
      Program[FirstFreeIns].Tag = CodeValue;
      Program[FirstFreeIns].Data.value = *value;
      ++FirstFreeIns;
    }
  else
    Error_Abort ("Internal Code_Value: overflow");
}

static void
Code_ExpandCurrAreaSymbolAsAddr (Value *value, ARMWord instrOffset)
{
  if (value->Tag == ValueSymbol && value->Data.Symbol.symbol == areaCurrentSymbol
      && value->Data.Symbol.factor == 1)
    *value = Value_Addr (15, -(instrOffset + 8) + value->Data.Symbol.offset);
}  

/**
 * \param sp Empty incrementing stack index.  On successful exit, Stack[sp - 1]
 * (with sp at least value 1) is the result, otherwise there is no result.
 * \return false if succeeded, true otherwise.
 */
static bool
Code_EvalLowest (size_t size, const Code *program, const ARMWord *instrOffsetP,
		 int *sp)
{
#ifdef DEBUG_CODE
  printf ("vvv Code_EvalLowest\n");
#endif
  const int spStart = *sp;
  for (size_t i = 0; i != size; i++)
    {
#ifdef DEBUG_CODE
      /* Dump stack entries.  */
      printf ("+++ Eval %zd/%zd : Stack: ", i, size);
      if (spStart == *sp)
	printf ("[ <EMPTY> ] ");
      else
	{
	  printf ("[ ");
	  Code_PrintValueArray (*sp - spStart, &Stack[spStart]);
	  printf (" ] ");
	}
      printf (": doing ");
#endif

      switch (program[i].Tag)
	{
	  case CodeOperator:
	    {
#ifdef DEBUG_CODE
	      printf ("[Operator %s] ", Lex_OperatorAsStr (program[i].Data.op));
#endif
	      if (Lex_IsUnop (program[i].Data.op))
		{
		  assert (spStart < *sp); /* At least one entry on the stack.  */
		  if (!Eval_Unop (program[i].Data.op, &Stack[*sp - 1]))
		    {
#ifdef DEBUG_CODE
		      printf ("FAILED\n");
#endif
		      return true;
		    }
		  /* No stack adjusting is needed, as one operand is consumed,
		     one result is produced.  */
		}
	      else
		{
		  assert (spStart < *sp - 1); /* At least two entries on the stack.  */

		  Operator_e op = program[i].Data.op;
		  if (!Eval_Binop (op, &Stack[*sp - 2], &Stack[*sp - 1]))
		    {
#ifdef DEBUG_CODE
		      printf ("FAILED\n");
#endif
		      return true;
		    }
		  /* Stack adjusting by one, as two operands are consumed, one
		     result is produced.  */
		  Value_Free (&Stack[*sp - 1]);
		  --(*sp);
		}
	      break;
	    }
		
	  case CodeValue:
	    {
	      Value value = program[i].Data.value;
#ifdef DEBUG_CODE
	      printf ("[Push value: ");
	      Value_Print (&value);
	      printf ("] ");
#endif
	      /* Resolve a defined symbol (but only when the next operator
		 isn't the OpSize one).  */
	      bool nextIsOpSize = i + 1 != size
				    && program[i + 1].Tag == CodeOperator
				    && program[i + 1].Data.op == eOp_Size;
	      if (!nextIsOpSize && Value_ResolveSymbol (&value))
		{
#ifdef DEBUG_CODE
		  printf ("FAILED\n");
#endif
		  return true;
		}
	      Value_Assign (&Stack[*sp], &value);
	      ++(*sp);
	      break;
	    }

	  default:
	    Error_Abort ("Internal codeEvalLow: illegal expression");
	    break;
	}
#ifdef DEBUG_CODE
      printf ("\n");
#endif
    }
#ifdef DEBUG_CODE
  /* Dump the final stack entries.  */
  printf ("+++ Eval %zd/%zd : Stack: ", size, size);
  if (spStart == *sp)
    printf ("[ <EMPTY> ] ");
  else
    {
      printf ("[ ");
      Code_PrintValueArray (*sp - spStart, &Stack[spStart]);
      printf (" ] ");
    }
  printf ("\n");
#endif
    
  if (instrOffsetP != NULL)
    {
#ifdef DEBUG_CODE
      printf ("+++ Converting current AREA symbol to ValueInt/ValueAddr.\n");
#endif
      /* Search for all current AREA symbols and convert them into ValueAddr.  */
      for (int s = spStart; s != *sp; ++s)
	Code_ExpandCurrAreaSymbolAsAddr (&Stack[s], *instrOffsetP);
    }
   
#ifdef DEBUG_CODE
  printf ("Final stack result: [ ");
  Code_PrintValueArray (*sp - spStart, &Stack[spStart]);
  printf ("]\n^^^\n");
#endif
    
  return false;
}

/**
 * \param instrOffsetP It points to the instruction offset
 * which can be use to convert the current AREA symbol into a ValueAddr
 * [PC, #-(<instr offset> + 8)].
 * \return Result of evaluation.  Only to be used before next evaluation.
 * Use Value_Assign() to keep a non-temporary copy of it.
 */
const Value *
Code_Eval (ValueTag legal, const ARMWord *instrOffsetP)
{
  return Code_EvalLow (legal, FirstFreeIns, Program, instrOffsetP);
}


/**
 * \param instrOffsetP When non-NULL, it points to the instruction offset
 * which can be use to convert the current AREA symbol into a ValueAddr
 * [PC, #-(<instr offset> + 8)].
 * \return Result of evaluation.  Only to be used before next evaluation.
 * Use Value_Assign() to keep a non-temporary copy of it.
 */
const Value *
Code_EvalLow (ValueTag legal, size_t size, const Code *program,
	      const ARMWord *instrOffsetP)
{
#ifdef DEBUG_CODE
  printf ("*** codeEvalLow(): program is: ");
  codePrint (size, program);
  printf ("\n");
#endif

  int sp = 0;
  if (size == 0 || Code_EvalLowest (size, program, instrOffsetP, &sp))
    {
      if (legal & ValueCode)
	{
	  const Value valueCode = Value_Code (size, program);
	  Value_Assign (&Stack[0], &valueCode);
	}
      else
	Stack[0].Tag = ValueIllegal;
    }
  else
    assert (sp == 1);

  switch (Stack[0].Tag)
    {
      case ValueInt:
	{
	  /* Upgrade integer to float when integer result is not wanted but
	     float result is.  */
	  if ((legal & (ValueInt | ValueFloat)) == ValueFloat)
	    {
	      ARMFloat f = (ARMFloat) Stack[0].Data.Int.i;
	      if (option_fussy)
		Error (ErrorInfo, "Changing integer %d to float %1.1f", Stack[0].Data.Int.i, f);
	      Stack[0] = Value_Float (f);
	    }
	  break;
	}
      case ValueString:
	{
	  /* Convert a one char string into integer when string is not wanted
	     but integer result is.  */
	  if ((legal & (ValueString | ValueInt)) == ValueInt
	      && Stack[0].Data.String.len == 1)
	    Stack[0] = Value_Int ((uint8_t)Stack[0].Data.String.s[0], eIntType_PureInt);
	  break;
	}
      default:
	break;
    }
  if (!(Stack[0].Tag & legal))
    {
#ifdef DEBUG_CODE
      printf ("XXX codeEvalLow(): resulting object (tag 0x%x) is not wanted (allowed tags 0x%x).\n", Stack[0].Tag, legal);
#endif
      Stack[0].Tag = ValueIllegal;
    }
#ifdef DEBUG_CODE
  printf ("--- codeEvalLow() result: [");
  Value_Print (&Stack[0]);
  printf ("]\n\n");
#endif

  return &Stack[0];
}


/**
 * Takes a full copy of current Code program.
 * \return You have ownership.
 */
Value
Code_TakeSnapShot (void)
{
  return Value_Code (FirstFreeIns, Program);
}

void
Code_Free (const Code *code, size_t len)
{
  for (size_t i = 0; i < len; ++i)
    {
      switch (code[i].Tag)
	{
	  case CodeOperator:
	    break;
	  case CodeValue:
	    Value_Free ((void *)&code[i].Data.value);
	    break;
	}
    }
  free ((void *)code);
}

Code *
Code_Copy (size_t len, const Code *code)
{
  Code *newCode;
  if ((newCode = calloc (len, sizeof (Code))) == NULL)
    Error_OutOfMem ();

  for (size_t i = 0; i < len; i++)
    {
      switch (newCode[i].Tag = code[i].Tag)
	{
	  case CodeOperator:
	    newCode[i].Data.op = code[i].Data.op;
	    break;
	  case CodeValue:
	    Value_Assign (&newCode[i].Data.value, &code[i].Data.value);
	    break;
	}
    }

  return newCode;
}

bool
Code_Equal (size_t len, const Code *a, const Code *b)
{
  for (size_t i = 0; i < len; i++)
    {
      if (a[i].Tag != b[i].Tag)
	return false;
      switch (a[i].Tag)
	{
	  case CodeOperator:
	    if (a[i].Data.op != b[i].Data.op)
	      return false;
	    break;
	  case CodeValue:
	    if (!Value_Equal (&a[i].Data.value, &b[i].Data.value))
	      return false;
	    break;
	}
    }

  return true;
}

#ifdef DEBUG
void
Code_Print (size_t size, const Code *program)
{
  for (size_t i = 0; i < size; i++)
    {
      switch (program[i].Tag)
	{
	  case CodeOperator:
	    if (Lex_IsUnop (program[i].Data.op))
	      printf ("[%s] ", Lex_OperatorAsStr (program[i].Data.op));
	    else
	      printf ("[%s] ", Lex_OperatorAsStr (program[i].Data.op));
	    break;

	  case CodeValue:
	    printf ("[");
	    Value_Print (&program[i].Data.value);
	    printf ("] ");
	    break;
	}
    }
}

#  ifdef DEBUG_CODE
static void
Code_PrintValueArray (size_t size, const Value *value)
{
  for (size_t i = 0; i != size; ++i)
    {
      Value_Print (&value[i]);
      if (i + 1 != size)
	putc (' ', stdout);
    }
}
#  endif
#endif
