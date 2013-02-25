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
#include "error.h"
#include "eval.h"
#include "phase.h"
#include "main.h"

#ifdef DEBUG
//#  define DEBUG_CODE
#endif

typedef struct
{
  Value value;
  bool owns;
} StackType_t;

#define CODE_SIZECODE  (1024)
#define CODE_SIZESTACK (1024)

static void Code_FreeCode (Code *codeP);
static bool Code_EvalLowest (size_t size, const Code *program, int *sp);

#ifdef DEBUG_CODE
static void Code_PrintValueArray (size_t size, const StackType_t *stackP);
#endif

static Code Program[CODE_SIZECODE]; /**< Ownership of Value objects.  */
static size_t FirstFreeIns; /**< Index for Program[] array for first free slot.  */
static StackType_t Stack[CODE_SIZESTACK]; /**< Stack objects.  */

void
Code_PrepareForPhase (Phase_e phase)
{
  switch (phase)
    {
      case eStartUp:
      case ePassOne:
	break;

      case ePassTwo:
      case eOutput:
	{
	  /* Time to get rid of our ownership of Value objects.  */
	  for (size_t i = 0; i != sizeof (Program)/sizeof (Program[0]); ++i)
	    Code_FreeCode (&Program[i]);
	  for (size_t i = 0; i != sizeof (Stack)/sizeof (Stack[0]); ++i)
	    {
	      if (Stack[i].owns)
		Value_Free (&Stack[i].value);
	    }
	  break;
	}

      case eCleanUp:
	break;
    }
}

void
Code_Init (void)
{
  FirstFreeIns = 0;
}

static void
Code_FreeCode (Code *codeP)
{
  switch (codeP->Tag)
    {
      case CodeOperator:
	break;

      case CodeValue:
	Value_Free (&codeP->Data.value);
	break;
    }
}

void
Code_Operator (Operator_e op)
{
  if (FirstFreeIns < CODE_SIZECODE)
    {
      Code_FreeCode (&Program[FirstFreeIns]);
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
      Code_FreeCode (&Program[FirstFreeIns]);
      /* Only during evaluation, we're going to expand the symbols.  */
      Program[FirstFreeIns].Tag = CodeValue;
      Program[FirstFreeIns].Data.value = Value_Symbol (symbol, 1, offset);
      ++FirstFreeIns;
    }
  else
    Error_Abort ("Internal Code_Symbol: overflow");
}

void
Code_String (const char *str, size_t len, bool owns)
{
  if (FirstFreeIns < CODE_SIZECODE)
    {
      Code_FreeCode (&Program[FirstFreeIns]);
      Program[FirstFreeIns].Tag = CodeValue;
      Program[FirstFreeIns].Data.value = Value_String (str, len, owns);
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
      Code_FreeCode (&Program[FirstFreeIns]);
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
      Code_FreeCode (&Program[FirstFreeIns]);
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
      Code_FreeCode (&Program[FirstFreeIns]);
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
      Code_FreeCode (&Program[FirstFreeIns]);
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
      size_t numEntries = value->Data.Code.len; 
      if (numEntries + FirstFreeIns < CODE_SIZECODE)
	{
	  for (size_t i = 0; i != numEntries; ++i)
	    Code_FreeCode (&Program[FirstFreeIns + i]);
	  memcpy (&Program[FirstFreeIns], value->Data.Code.c, numEntries*sizeof (Code));
	  FirstFreeIns += numEntries;
	}
      else
	Error_Abort ("Internal Code_Value: overflow");
    }
  else if (FirstFreeIns < CODE_SIZECODE)
    {
      Code_FreeCode (&Program[FirstFreeIns]);
      Program[FirstFreeIns].Tag = CodeValue;
      Program[FirstFreeIns].Data.value = *value;
      ++FirstFreeIns;
    }
  else
    Error_Abort ("Internal Code_Value: overflow");
}

/**
 * \param sp Empty incrementing stack index.  On successful exit, Stack[sp - 1]
 * (with sp at least value 1) is the result, otherwise there is no result.
 * \return false if succeeded, true otherwise.
 */
static bool
Code_EvalLowest (size_t size, const Code *program, int *sp)
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
		  const Value result = Eval_Unop (program[i].Data.op, &Stack[*sp - 1].value);
		  if (result.Tag == ValueIllegal)
		    {
#ifdef DEBUG_CODE
		      printf ("FAILED\n");
#endif
		      return true;
		    }
		  if (Stack[*sp - 1].owns)
		    Value_Free (&Stack[*sp - 1].value);
		  Stack[*sp - 1].owns = true;
		  Stack[*sp - 1].value = result;
		}
	      else
		{
		  assert (spStart < *sp - 1); /* At least two entries on the stack.  */
		  const Value result = Eval_Binop (program[i].Data.op, &Stack[*sp - 2].value, &Stack[*sp - 1].value);
		  if (result.Tag == ValueIllegal)
		    {
#ifdef DEBUG_CODE
		      printf ("FAILED\n");
#endif
		      return true;
		    }
		  /* Stack adjusting by one, as two operands are consumed, one
		     result is produced.  */
		  if (Stack[*sp - 2].owns)
		    Value_Free (&Stack[*sp - 2].value);
		  Stack[*sp - 2].owns = true;
		  Stack[*sp - 2].value = result;
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
	      if (Stack[*sp].owns)
		Value_Free (&Stack[*sp].value);
	      Stack[*sp].owns = false; /* Just a copy from code program, stack doesn't have ownership.  */
	      Stack[*sp].value = value;
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
   
#ifdef DEBUG_CODE
  printf ("Final stack result: [ ");
  Code_PrintValueArray (*sp - spStart, &Stack[spStart]);
  printf ("]\n^^^\n");
#endif
    
  return false;
}

/**
 * \return Result of evaluation.  Only to be used before next evaluation.
 * Use Value_Assign()/Value_Copy() to keep a non-temporary copy of it.
 */
const Value *
Code_Eval (ValueTag legal)
{
  return Code_EvalLow (legal, FirstFreeIns, Program);
}


/**
 * \return Result of evaluation.  Only to be used before next evaluation.
 * Use Value_Assign()/Value_Copy() to keep a non-temporary copy of it.
 */
const Value *
Code_EvalLow (ValueTag legal, size_t size, const Code *program)
{
#ifdef DEBUG_CODE
  printf ("*** codeEvalLow(): program is: ");
  Code_Print (size, program);
  printf ("\n");
#endif

  int sp = 0;
  if (size == 0 || Code_EvalLowest (size, program, &sp))
    {
      if (Stack[0].owns)
	Value_Free (&Stack[0].value);
      if ((legal & ValueCode) != 0)
	{
	  Stack[0].owns = true;
	  Stack[0].value = Value_Code (size, program);
	}
      else
	{
	  Stack[0].owns = false;
	  Stack[0].value = Value_Illegal ();
	}
    }
  else
    assert (sp == 1);

  switch (Stack[0].value.Tag)
    {
      case ValueInt:
	{
	  /* Upgrade integer to float when integer result is not wanted but
	     float result is.  */
	  if ((legal & (ValueInt | ValueFloat)) == ValueFloat)
	    {
	      ARMFloat fltVal = (ARMFloat) Stack[0].value.Data.Int.i;
	      if (option_fussy)
		Error (ErrorInfo, "Changing integer %d to float %1.1f", Stack[0].value.Data.Int.i, fltVal);
	      Stack[0].value = Value_Float (fltVal);
	    }
	  break;
	}

      case ValueString:
	{
	  /* Convert a one char string into integer when string is not wanted
	     but integer result is.  */
	  if ((legal & (ValueString | ValueInt)) == ValueInt
	      && Stack[0].value.Data.String.len == 1)
	    {
	      uint32_t intVal = (uint8_t)Stack[0].value.Data.String.s[0];
	      if (Stack[0].owns)
		Value_Free (&Stack[0].value);
	      Stack[0].value = Value_Int (intVal, eIntType_PureInt);
	    }
	  break;
	}

      default:
	break;
    }
  if ((Stack[0].value.Tag & legal) == 0)
    {
#ifdef DEBUG_CODE
      printf ("XXX codeEvalLow(): resulting object (tag 0x%x) is not wanted (allowed tags 0x%x).\n", Stack[0].value.Tag, legal);
#endif
      if (Stack[0].owns)
        Value_Free (&Stack[0].value);
      else
	Stack[0].value = Value_Illegal ();
    }
#ifdef DEBUG_CODE
  printf ("--- codeEvalLow() result: [");
  Value_Print (&Stack[0].value);
  printf ("]\n\n");
#endif

  return &Stack[0].value;
}


void
Code_Free (Code *code, size_t len)
{
  for (size_t i = 0; i < len; ++i)
    {
      switch (code[i].Tag)
	{
	  case CodeOperator:
	    break;

	  case CodeValue:
	    Value_Free (&code[i].Data.value);
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
Code_PrintValueArray (size_t size, const StackType_t *stackP)
{
  for (size_t i = 0; i != size; ++i)
    {
      Value_Print (&stackP[i].value);
      if (i + 1 != size)
	putc (' ', stdout);
    }
}
#  endif
#endif
