/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2011 GCCSDK Developers
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
//#  define DEBUG_CODE
#endif

#define CODE_SIZECODE  (1024)
#define CODE_SIZESTACK (1024)

static void Code_Normalize (Code *values, size_t numElem);
static bool Code_ExpandCode (const Value *value, int *sp);
static void Code_ExpandCurrAreaSymbolAsAddr (Value *value, ARMWord instrOffset);
static bool Code_EvalLowest (size_t size, const Code *program, const ARMWord *instrOffsetP, int *sp, bool doNormalize);
static bool Code_HasUndefSymbols (const Code *code, size_t len);

static Code Program[CODE_SIZECODE]; /**< No ownership of Value objects.  */
static int FirstFreeIns; /**< Index for Program[] array for first free slot.  */
static Code Stack[CODE_SIZESTACK]; /**< Ownership of Value object.  */

void
codeInit (void)
{
  FirstFreeIns = 0;
}

void
codeOperator (Operator op)
{
  if (FirstFreeIns < CODE_SIZECODE)
    {
      Program[FirstFreeIns].Tag = CodeOperator;
      Program[FirstFreeIns].Data.op = op;
      ++FirstFreeIns;
    }
  else
    errorAbort ("Internal codeOp: overflow");
}

void
codeSymbol (Symbol *symbol)
{
  if (FirstFreeIns < CODE_SIZECODE)
    {
      /* Only during evaluation, we're going to expand the symbols.  */
      Program[FirstFreeIns].Tag = CodeValue;
      Program[FirstFreeIns].Data.value = Value_Symbol (symbol, 1);
      ++FirstFreeIns;
    }
  else
    errorAbort ("Internal codeSymbol: overflow");
}

void
codePosition (Symbol *area, int offset)
{
  if (area->area.info->type & AREA_ABS)
    codeInt (Area_GetBaseAddress (area) + offset);
  /*  FIXME: little hack as we don't sufficiently realise the equivalence
      of sym and code(sym + 0) */
  /* else if (offset == 0)
    codeSymbol (area); */
  else
    {
      if (FirstFreeIns >= CODE_SIZECODE)
	errorAbort ("Internal codePosition: overflow");

      const Code values[] =
	{
	  { .Tag = CodeValue,
	    .Data.value = { .Tag = ValueSymbol, .Data.Symbol = { .factor = 1, .symbol = area } } },
	  { .Tag = CodeValue,
	    .Data.value = { .Tag = ValueInt, .Data.Int = { .i = offset } } },
	  { .Tag = CodeOperator,
	    .Data.op = Op_add }
	};
      Program[FirstFreeIns].Tag = CodeValue;
      Program[FirstFreeIns].Data.value = Value_Code (sizeof (values)/sizeof (values[0]), values);
      ++FirstFreeIns;
    }
}

void
codeStorage (void)
{
  if (FirstFreeIns < CODE_SIZECODE)
    {
      Program[FirstFreeIns].Tag = CodeValue;
      Program[FirstFreeIns].Data.value = *storageValue ();
      ++FirstFreeIns;
    }
  else
    errorAbort ("Internal codeStorage: overflow");
}

void
codeString (const char *str, size_t len)
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
    errorAbort ("Internal codeString: overflow");
}

void
codeInt (int value)
{
  if (FirstFreeIns < CODE_SIZECODE)
    {
      Program[FirstFreeIns].Tag = CodeValue;
      Program[FirstFreeIns].Data.value = Value_Int (value);
      ++FirstFreeIns;
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
      Program[FirstFreeIns].Data.value.Tag = ValueFloat;
      Program[FirstFreeIns].Data.value.Data.Float.f = value;
      ++FirstFreeIns;
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
      Program[FirstFreeIns].Data.value.Tag = ValueBool;
      Program[FirstFreeIns].Data.value.Data.Bool.b = value;
      ++FirstFreeIns;
    }
  else
    errorAbort ("Internal codeBool: overflow");
}

/**
 * Adds "Push ValueAddr object" code element.
 */
void
codeAddr (int reg, int offset)
{
  if (FirstFreeIns < CODE_SIZECODE)
    {
      Program[FirstFreeIns].Tag = CodeValue;
      Program[FirstFreeIns].Data.value = Value_Addr (reg, offset);
      ++FirstFreeIns;
    }
  else
    errorAbort ("Internal codeAddr: overflow");
}

void
codeValue (const Value *value, bool expCode)
{
  if (expCode && value->Tag == ValueCode)
    {
      if (value->Data.Code.len + FirstFreeIns < CODE_SIZECODE)
	{
	  memcpy (&Program[FirstFreeIns], value->Data.Code.c, value->Data.Code.len*sizeof (Code));
	  FirstFreeIns += value->Data.Code.len;
	}
      else
	errorAbort ("Internal codeValue: overflow");
    }
  else if (FirstFreeIns < CODE_SIZECODE)
    {
      Program[FirstFreeIns].Tag = CodeValue;
      Program[FirstFreeIns].Data.value = *value;
      ++FirstFreeIns;
    }
  else
    errorAbort ("Internal codeValue: overflow");
}

/**
 * Sum all given Value objects and return that as ValueSymbol, ValueInt,
 * ValueAddr or ValueCode at values[0].
 * The ValueCode will contain zero or one ValueInt or ValueAddr object,
 * followed by zero or more ValueSymbols (all objects are added or subtracted).
 */
static void
Code_Normalize (Code *values, size_t numElem)
{
  assert (numElem != 0);

  /* Find the first ValueAddr (if there is one) and move this to the front.  */
  size_t addrIndex;
  for (addrIndex = 0;
       addrIndex != numElem && values[addrIndex].Data.value.Tag != ValueAddr;
       ++addrIndex)
    /* */;
  if (addrIndex != numElem)
    {
      /* Swap values[0] and values[addrIndex].  */
      const Code tmp = values[addrIndex];
      values[addrIndex] = values[0];
      values[0] = tmp;
      ++addrIndex;

      /* Search for other ValueAddr, they should share the same base reg.  If
         not, this is considered as a failure.  Merge them with the first
         one found.  */
      while (addrIndex != numElem)
	{
	  if (values[addrIndex].Data.value.Tag == ValueAddr)
	    {
	      assert (values[0].Data.value.Data.Addr.r == values[addrIndex].Data.value.Data.Addr.r); /* FIXME: this should become an user error.  */
	      values[0].Data.value.Data.Addr.i += values[addrIndex].Data.value.Data.Addr.i;
	      --numElem;
	      memmove (&values[addrIndex], &values[addrIndex + 1], (numElem - addrIndex)*sizeof (Code));
	    }
	  else
	    ++addrIndex;
	}
    }
  
  /* Find first ValueInt and move this to the front or merge it with the
     ValueAddr found.  */
  size_t intIndex;
  if (values[0].Data.value.Tag == ValueAddr)
    intIndex = 1;
  else
    {
      for (intIndex = 0;
           intIndex != numElem && values[intIndex].Data.value.Tag != ValueInt;
           ++intIndex)
	/* */;
      if (intIndex != numElem)
	{
	  /* Swap values[0] and values[intIndex].  */
	  const Code tmp = values[intIndex];
	  values[intIndex] = values[0];
	  values[0] = tmp;
	  ++intIndex;
	}
    }
  /* Search for other ValueInt and add this to the first ValueInt/ValueAddr.  */
  while (intIndex != numElem)
    {
      if (values[intIndex].Data.value.Tag == ValueInt)
	{
	  if (values[0].Data.value.Tag == ValueInt)
	    values[0].Data.value.Data.Int.i += values[intIndex].Data.value.Data.Int.i;
	  else
	    values[0].Data.value.Data.Addr.i += values[intIndex].Data.value.Data.Int.i;
	  --numElem;
	  memmove (&values[intIndex], &values[intIndex + 1], (numElem - intIndex)*sizeof (Code));
	}
      else
	++intIndex;
    }

  const bool gotIntAddr = values[0].Data.value.Tag == ValueInt || values[0].Data.value.Tag == ValueAddr;
  /* Merge same symbols together.  */
  size_t startSymIndex = gotIntAddr ? 1 : 0;
  for (size_t i = startSymIndex; i + 1 < numElem; /* */)
    {
      assert (values[i].Data.value.Tag == ValueSymbol);
      for (size_t s = numElem - 1; i < s; --s)
	{
	  assert (values[s].Data.value.Tag == ValueSymbol);
	  if (values[i].Data.value.Data.Symbol.symbol == values[s].Data.value.Data.Symbol.symbol)
	    {
	      values[i].Data.value.Data.Symbol.factor += values[s].Data.value.Data.Symbol.factor;
	      --numElem;
	      memmove (&values[s], &values[s+1], (numElem - s)*sizeof (Code));
	    }
	}
      if (values[i].Data.value.Data.Symbol.factor == 0)
	{
	  /* Symbol got away.  */
	  --numElem;
	  memmove (&values[i], &values[i+1], (numElem - i)*sizeof (Code));
	}
      else
	++i;
    }

  if (numElem == 0)
    {
      /* E.g. no constant nor any symbols got left.  */
      values[0].Data.value = Value_Int (0);
      return;
    }
  if (numElem == 1)
    return;

  /* Mixture of ValueInt, ValueAddr and ValueSymbol's.  */
  size_t size = numElem*2 - 1;
  Code *codeP;
  if ((codeP = malloc (size*sizeof(Code))) == NULL)
    errorOutOfMem ();

  size_t size2 = 0;
  codeP[size2].Tag = CodeValue;
  codeP[size2].Data.value = values[0].Data.value;
  ++size2;
  for (size_t i = 1; i != numElem; ++i)
    {
      codeP[size2].Tag = CodeValue;
      codeP[size2].Data.value = values[i].Data.value;
      ++size2;
      codeP[size2].Tag = CodeOperator;
      codeP[size2].Data.op = Op_add;
      ++size2;
    }
  assert (size == size2);

  values[0].Data.value.Tag = ValueCode;
  values[0].Data.value.Data.Code.len = size;
  values[0].Data.value.Data.Code.c = codeP;
}

static bool
Code_ExpandCode (const Value *value, int *sp)
{
  assert (value->Tag == ValueCode);
  bool fail = Code_EvalLowest (value->Data.Code.len, value->Data.Code.c, NULL, sp, false);
#ifdef DEBUG_CODE
  if (fail)
    printf ("FAILED\n");
#endif
  return fail;
}

static void
Code_ExpandCurrAreaSymbolAsAddr (Value *value, ARMWord instrOffset)
{
  if (value->Tag == ValueSymbol && value->Data.Symbol.symbol == areaCurrentSymbol)
    {
      int factor = value->Data.Symbol.factor;
      value->Tag = ValueAddr;
      value->Data.Addr.i = -(factor*instrOffset + 8);
      value->Data.Addr.r = 15;
    }
  else if (value->Tag == ValueCode)
    for (size_t i = 0; i != value->Data.Code.len; ++i)
      if (value->Data.Code.c[i].Tag == CodeValue)
	Code_ExpandCurrAreaSymbolAsAddr (&value->Data.Code.c[i].Data.value, instrOffset);
}  

#if 0
static void
Code_ExpandCurrAreaSymbolAsOffset (Value *value, int offset)
{
  if (value->Tag == ValueSymbol && value->Data.Symbol.symbol == areaCurrentSymbol)
    {
      value->Tag = ValueInt;
      value->Data.Int.i = areaCurrentSymbol->area.info->curIdx;
    }
  else if (value->Tag == ValueCode)
    for (size_t i = 0; i != value->Data.Code.len; ++i)
      if (value->Data.Code.c[i].Tag == CodeValue)
	Code_ExpandCurrAreaSymbolAsOffset (&value->Data.Code.c[i].Data.value, offset);
}
#endif

static bool
Code_HasNonAddOperator (int start, int end, const Code *code)
{
  for (int i = start; i != end; ++i)
    {
      switch (code[i].Tag)
	{
	  case CodeValue:
	    {
	      if (code[i].Data.value.Tag == ValueCode
		  && Code_HasNonAddOperator (0,
					     code[i].Data.value.Data.Code.len,
					     code[i].Data.value.Data.Code.c))
		return true;
	    }
	    break;
	  case CodeOperator:
	    if (code[i].Data.op != Op_add)
	      return true;
	    break;
	}
    }

  return false;
}

/**
 * \param sp Empty incrementing stack index.  On successful exit, Stack[sp - 1]
 * (with sp at least value 1) is the result, otherwise there is no result.
 * \return false if succeeded, true otherwise.
 */
static bool
Code_EvalLowest (size_t size, const Code *program, const ARMWord *instrOffsetP,
		 int *sp, bool doNormalize)
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
	  codePrint (*sp - spStart, &Stack[spStart]);
	  printf (" ] ");
	}
      printf (": doing ");
#endif

      switch (program[i].Tag)
	{
	  case CodeOperator:
#ifdef DEBUG_CODE
	    printf ("[Operator %s] ", Lex_OperatorAsStr (program[i].Data.op));
#endif
	    if (IsUnop (program[i].Data.op))
	      {
		assert (spStart < *sp); /* At least one entry on the stack.  */
		if (Stack[*sp - 1].Tag != CodeValue
		    || (Stack[*sp - 1].Data.value.Tag == ValueSymbol
		        && !(program[i].Data.op == Op_size && (Stack[*sp - 1].Data.value.Data.Symbol.symbol->type & SYMBOL_DEFINED) != 0)
		        && program[i].Data.op != Op_neg))
		  {
		    /* We have an unresolved (or even undefined) symbol
		       on the stack, or previous operation failed because
		       of an unresolved (undefined) symbol.  */
		    Stack[(*sp)++] = program[i];
		  }
		else if (!evalUnop (program[i].Data.op, &Stack[*sp - 1].Data.value))
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

		Operator op = program[i].Data.op;

		/* Expand ValueCode arguments for Op_add / Op_sub operators
		   (left).  */
		if (Stack[*sp - 2].Data.value.Tag == ValueCode
		    && (op == Op_add || op == Op_sub))
		  {
		    Code rvalue = Stack[*sp - 1];
		    Stack[--(*sp)].Data.value.Tag = ValueIllegal;
		    Code lvalue = Stack[*sp - 1];
		    Stack[--(*sp)].Data.value.Tag = ValueIllegal;
		    bool fail = Code_ExpandCode (&lvalue.Data.value, sp);
		    valueFree (&lvalue.Data.value);
		    Stack[(*sp)++] = rvalue;
		    if (fail)
		      {
#ifdef DEBUG_CODE
			  printf ("FAILED\n");
#endif
			  return true;
		      }
		  }

		/* Pre-normalize subtraction into addition.  */
		if (Stack[*sp - 1].Tag == CodeValue && op == Op_sub)
		  {
		    switch (Stack[*sp - 1].Data.value.Tag)
		      {
			case ValueInt:
			  Stack[*sp - 1].Data.value.Data.Int.i = -Stack[*sp - 1].Data.value.Data.Int.i;
			  op = Op_add;
			  break;
			case ValueFloat:
			  Stack[*sp - 1].Data.value.Data.Float.f = -Stack[*sp - 1].Data.value.Data.Float.f;
			  op = Op_add;
			  break;
			case ValueSymbol:
			  Stack[*sp - 1].Data.value.Data.Symbol.factor = -Stack[*sp - 1].Data.value.Data.Symbol.factor;
			  op = Op_add;
			  break;
			case ValueCode:
			  if (!evalUnop (Op_neg, &Stack[*sp - 1].Data.value))
			    {
#ifdef DEBUG_CODE
			      printf ("FAILED\n");
#endif
			      return true;
			    }
			  op = Op_add;
			  break;
			default:
			  break;
		      }
		  }
		
		/* Expand ValueCode arguments for Op_add / Op_sub operators
		   (right).  */
		if (Stack[*sp - 1].Data.value.Tag == ValueCode
		    && (op == Op_sub || op == Op_add))
		  {
		    Code rvalue = Stack[*sp - 1];
		    Stack[--(*sp)].Data.value.Tag = ValueIllegal;
		    bool fail = Code_ExpandCode (&rvalue.Data.value, sp);
		    valueFree (&rvalue.Data.value);
		    if (fail)
		      {
#ifdef DEBUG_CODE
			printf ("FAILED\n");
#endif
			return true;
		      }
		  }
		/* If one of the operands is an undefined symbol, bail out.
		   Also if we have an operator as operand, bail out as well.  */
		if (Stack[*sp - 2].Tag == CodeValue
		    && Stack[*sp - 2].Data.value.Tag != ValueSymbol
		    && Stack[*sp - 1].Tag == CodeValue
		    && Stack[*sp - 1].Data.value.Tag != ValueSymbol)
		  {
		    if (!evalBinop (op,
				    &Stack[*sp - 2].Data.value,
				    &Stack[*sp - 1].Data.value))
		      {
#ifdef DEBUG_CODE
			  printf ("FAILED\n");
#endif
			  return true;
		      }
		    /* Stack adjusting by one, as two operands are consumed, one
		       result is produced.  */
		    valueFree (&Stack[*sp - 1].Data.value);
		    --(*sp);
		  }
		else
		  {
		    const Code code =
		      {
			.Tag = CodeOperator,
			.Data.op = op
		      };
		    Stack[(*sp)++] = code;
		  }
	      }
	    break;

	  case CodeValue:
	    {
	      const Value *valueP = &program[i].Data.value;
#ifdef DEBUG_CODE
	      printf ("[Push value: ");
	      valuePrint (valueP);
	      printf ("] ");
#endif
	      /* Resolve the symbol when defined and when it is not an
	         absolute AREA symbol.
		 Don't do this when the next program element is :SIZE:
	         either.  */
	      bool nextIsOpSize = i + 1 != size
				    && program[i + 1].Tag == CodeOperator
				    && program[i + 1].Data.op == Op_size;
	      /* FIXME: this can probably loop forever: label1 -> label2 -> label1 */
	      while (valueP->Tag == ValueSymbol
		     && (valueP->Data.Symbol.symbol->type & SYMBOL_DEFINED)
	             && ((valueP->Data.Symbol.symbol->type & SYMBOL_AREA) == 0 || (valueP->Data.Symbol.symbol->type & SYMBOL_ABSOLUTE) != 0)
		     && (!nextIsOpSize || valueP->Data.Symbol.symbol->value.Tag == ValueSymbol))
		valueP = &valueP->Data.Symbol.symbol->value;
	      if (Stack[*sp].Tag != CodeValue)
		{
		  Stack[*sp].Tag = CodeValue;
		  Stack[*sp].Data.value.Tag = ValueIllegal;
		}
	      Value_Assign (&Stack[*sp].Data.value, valueP);
	      ++(*sp);
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
  /* Dump the final stack entries.  */
  printf ("+++ Eval %zd/%zd : Stack: ", size, size);
  if (spStart == *sp)
    printf ("[ <EMPTY> ] ");
  else
    {
      printf ("[ ");
      codePrint (*sp - spStart, &Stack[spStart]);
      printf (" ] ");
    }
  printf ("\n");
#endif
  
  /* Check if we only have Op_add operators left on our stack.  If not,
     return what we've already calculated.  */
  if (Code_HasNonAddOperator (spStart, *sp, Stack))
    {
      /* Return ValueCode object.  */
      if (Stack[spStart].Tag == CodeValue
          && Stack[spStart].Data.value.Tag == ValueCode
          && spStart + 1 == *sp)
	return false;
      const Value code = Value_Code (*sp - spStart, &Stack[spStart]);
      if (Stack[spStart].Tag == CodeValue)
	valueFree (&Stack[spStart].Data.value);
      else
	Stack[spStart].Tag = CodeValue;
      Stack[spStart].Data.value = code;
      *sp = spStart + 1;
      return false;
    }

  if (instrOffsetP != NULL)
    {
#ifdef DEBUG_CODE
      printf ("+++ Converting current AREA symbol to ValueInt/ValueAddr.\n");
#endif
      /* Search for all current AREA symbols and convert them into ValueAddr.  */
      for (int s = spStart; s != *sp; ++s)
	{
	  if (Stack[s].Tag != CodeValue)
	    continue;
	  Code_ExpandCurrAreaSymbolAsAddr (&Stack[s].Data.value, *instrOffsetP);
	}
    }
  
  if (doNormalize)
    {
      /* Sum all Symbols, ValueInt's and ValueAddr's.  If that's still more
         then one entry, convert this to a ValueCode object, else that specific
         Value (ValueSymbol, ValueInt).  */
      /* Firstly, expand all ValueCode objects.  */
      for (int s = spStart; s != *sp; ++s)
	{
	  if (Stack[s].Tag != CodeValue)
	    continue;
	  if (Stack[s].Data.value.Tag == ValueCode)
	    {
	      Code code = Stack[s];
	      memmove (&Stack[s], &Stack[s + 1], (*sp - (s + 1))*sizeof (Code));
	      --(*sp);
	      Stack[*sp].Tag = CodeValue;
	      Stack[*sp].Data.value.Tag = ValueIllegal;
	      if (Code_ExpandCode (&code.Data.value, sp))
		{
#ifdef DEBUG_CODE
		  printf ("FAILED\n");
#endif
		  return true;
		}
	      valueFree (&code.Data.value);
	      --s;
	    }
	}
      /* Secondly, skip all operators Op_add.  */
      int w = spStart;
      for (int r = w; r != *sp; ++r)
	{
	  if (Stack[r].Tag == CodeOperator)
	    {
	      assert (Stack[r].Data.op == Op_add);
	      continue;
	    }
	  if (w != r)
	    Stack[w] = Stack[r];
	  ++w;
	}
      const int newSP = w;
      for (/* */; w != *sp; ++w)
	{
	  if (Stack[w].Tag == CodeValue)
	    Stack[w].Data.value.Tag = ValueIllegal;
	}
      *sp = newSP;
      /* Thirdly, normalize all expanded Value objects.  */
      if (spStart + 1 < *sp)
	{
	  Code_Normalize (&Stack[spStart], *sp - spStart);
	  *sp = spStart + 1;
	}
    }
  
#ifdef DEBUG_CODE
  printf ("Final stack result: ");
  codePrint (*sp - spStart, &Stack[spStart]);
  printf ("\n^^^\n");
#endif
    
  return false;
}

#if 0
void
Code_Assign (Code *dst, const Code *src)
{
  if (dst == src)
    return;

  if (dst->Tag == CodeValue)
    {
      if (src->Tag == CodeValue)
	Value_Assign (&dst->Data.value, &src->Data.value);
      else
	{
	  valueFree (&dst->Data.value);
	  *dst = *src;
	}
    }
  else
    {
      if (src->Tag == CodeValue)
	{
	  dst->Tag = CodeValue;
	  dst->Data.value.Tag = ValueIllegal;
	  Value_Assign (&dst->Data.value, &src->Data.value);
	}
      else
	*dst = *src;
    }
}
#endif

/**
 * \param instrOffsetP It points to the instruction offset
 * which can be use to convert the current AREA symbol into a ValueAddr
 * [PC, #-(<instr offset> + 8)].
 * \return Result of evaluation.  Only to be used before next evaluation.
 * Use Value_Assign() to keep a non-temporary copy of it.
 */
const Value *
codeEval (ValueTag legal, const ARMWord *instrOffsetP)
{
  return codeEvalLow (legal, FirstFreeIns, Program, instrOffsetP);
}


/**
 * \param instrOffsetP When non-NULL, it points to the instruction offset
 * which can be use to convert the current AREA symbol into a ValueAddr
 * [PC, #-(<instr offset> + 8)].
 * \return Result of evaluation.  Only to be used before next evaluation.
 * Use Value_Assign() to keep a non-temporary copy of it.
 */
const Value *
codeEvalLow (ValueTag legal, size_t size, Code *program,
	     const ARMWord *instrOffsetP)
{
#ifdef DEBUG_CODE
  printf ("*** codeEvalLow(): program is: ");
  codePrint (size, program);
  printf ("\n");
#endif

  int sp = 0;
  if (size == 0 || Code_EvalLowest (size, program, instrOffsetP, &sp, true))
    {
      Stack[0].Tag = CodeValue;
      Stack[0].Data.value.Tag = ValueIllegal;
    }
  else
    assert (sp == 1);

  if (!(Stack[0].Data.value.Tag & legal))
    {
      if (option_autocast
          && (legal & ValueFloat) && Stack[0].Data.value.Tag == ValueInt)
	{
	  ARMFloat f = Stack[0].Data.value.Data.Int.i;
	  if (option_fussy)
	    error (ErrorInfo, "Changing integer %d to float %1.1f", Stack[0].Data.value.Data.Int.i, f);
	  Stack[0].Data.value.Tag = ValueFloat;
	  Stack[0].Data.value.Data.Float.f = f;
	}
      else
	{
#ifdef DEBUG_CODE
	  printf ("XXX codeEvalLow(): resulting object (tag 0x%x) is not wanted (allowed tags 0x%x).\n", Stack[0].Data.value.Tag, legal);
#endif
	  Stack[0].Data.value.Tag = ValueIllegal;
	}
    }
#ifdef DEBUG_CODE
  printf ("--- codeEvalLow() result: [");
  valuePrint (&Stack[0].Data.value);
  printf ("]\n\n");
#endif

  return &Stack[0].Data.value;
}


/**
 * Detects if there are undefined symbols in the current Code program.
 */
bool
Code_HasUndefinedSymbols (void)
{
  return Code_HasUndefSymbols (Program, FirstFreeIns);
}

/**
 * \return true when we have an undefined symbol in our code.
 */
static bool
Code_HasUndefSymbols (const Code *code, size_t len)
{
  for (size_t i = 0; i < len; ++i)
    {
      switch (code[i].Tag)
	{
	  case CodeOperator:
	    break;

	  case CodeValue:
	    switch (code[i].Data.value.Tag)
	      {
	        case ValueSymbol:
		  if (!(code[i].Data.value.Data.Symbol.symbol->type & SYMBOL_DEFINED)
		      && code[i].Data.value.Data.Symbol.symbol != areaCurrentSymbol)
		    return true;
		  break;

		case ValueCode:
		  if (Code_HasUndefSymbols (code[i].Data.value.Data.Code.c,
					    code[i].Data.value.Data.Code.len))
		    return true;
		  break;

		default:
		  break;
	      }
	    break;
	}
    }

  return false;
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
Code_Free (Code *code, size_t len)
{
  for (size_t i = 0; i < len; ++i)
    {
      switch (code[i].Tag)
	{
	  case CodeOperator:
	    break;
	  case CodeValue:
	    valueFree (&code[i].Data.value);
	    break;
	}
    }
  free (code);
}

Code *
codeCopy (size_t len, const Code *code)
{
  Code *newCode;
  if ((newCode = calloc (len, sizeof (Code))) == NULL)
    errorOutOfMem ();

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
codeEqual (size_t len, const Code *a, const Code *b)
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
	    if (!valueEqual (&a[i].Data.value, &b[i].Data.value))
	      return false;
	    break;
	}
    }

  return true;
}

#ifdef DEBUG
void
codePrint (size_t size, const Code *program)
{
  for (size_t i = 0; i < size; i++)
    {
      switch (program[i].Tag)
	{
	  case CodeOperator:
	    if (IsUnop (program[i].Data.op))
	      printf ("[%s] ", Lex_OperatorAsStr (program[i].Data.op));
	    else
	      printf ("[%s] ", Lex_OperatorAsStr (program[i].Data.op));
	    break;

	  case CodeValue:
	    printf ("[");
	    valuePrint (&program[i].Data.value);
	    printf ("] ");
	    break;
	}
    }
}
#endif
