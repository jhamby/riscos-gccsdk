/*
 * AsAsm an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2002-2013 GCCSDK Developers
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
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "area.h"
#include "code.h"
#include "common.h"
#include "error.h"
#include "eval.h"
#include "filestack.h"
#include "global.h"
#include "include.h"
#include "main.h"
#include "phase.h"
#include "state.h"

static const char oLowcaseCCodes[] = "eqnecsccmiplvsvchilsgeltgtlealnvhslo";

/**
 * \param str Pointer to condition code (might be NULL but then len should be
 * 0 as well).
 * \param len Length of the condition code.
 * \return The condition code.  Values 16 and 17 are respectively "HS" and
 * "LO" (those correspond to condition codes 2 and 3).
 * Is UINT_MAX when condition code is not known.
 */
static unsigned
GetCCode (const char *str, size_t len)
{
  if (len == 0)
    return 14;
  if (len != 2)
    return UINT_MAX;
  if ((str[0] & 0x20) != (str[1] & 0x20))
    return -1;
  const char lowCase0 = str[0] | 0x20;
  const char lowCase1 = str[1] | 0x20;
  for (size_t result = 0; result != sizeof (oLowcaseCCodes)-1; result += 2)
    {
      if (lowCase0 == oLowcaseCCodes[result + 0] && lowCase1 == oLowcaseCCodes[result + 1])
	return (unsigned)result / 2;
    }
  return UINT_MAX;
}

/* No validation checking on value types! */
static int
ememcmp (const Value *lv, const Value *rv)
{
  const size_t lvl = lv->Data.String.len;
  const size_t rvl = rv->Data.String.len;
  int a = memcmp (lv->Data.String.s, rv->Data.String.s, lvl < rvl ? lvl : rvl);
  return a ? a : (int)(lvl - rvl);
}

/**
 * Get integer value of a ValueInt and one character ValueString object.
 * Note absolute area symbols are already resolved to integers by
 * having called Value_ResolveSymbol before feeding the evaluation stack.
 * \return true when it concerns a ValueInt or one character ValueString object.
 */
static bool
GetInt (const Value *valP, uint32_t *i)
{
  switch (valP->Tag)
    {
      case ValueInt:
	/* A CPU register list is not a meaningful integer value, so exclude
	   that int type.  */
	if (valP->Data.Int.type != eIntType_CPURList)
	  {
	    *i = (unsigned)valP->Data.Int.i;
	    return true;
	  }
	break;

      case ValueString:
	if (valP->Data.String.len == 1)
	  {
	    *i = (uint8_t)valP->Data.String.s[0];
	    return true;
	  }
	break;

      default:
	break;
    }

  return false;
}

#define STRINGIFY(OP)	#OP
/* Core implementation for '<', '<=', '>', '>=', '==' and '!='.
   Works for ValueFloat, ValueString, ValueAddr and ValueInt.  */
#define COMPARE(OP) \
  do \
    { \
      uint32_t lint, rint;\
      if (lvalue->Tag == ValueFloat && rvalue->Tag == ValueFloat) \
	result = Value_Bool (lvalue->Data.Float.f OP rvalue->Data.Float.f); \
      else if (lvalue->Tag == ValueString && rvalue->Tag == ValueString) \
	result = Value_Bool (ememcmp (lvalue, rvalue) OP 0); \
      else if (GetInt (lvalue, &lint) && GetInt (rvalue, &rint)) \
	result = Value_Bool (lint OP rint); /* Comparing integers happens *unsigned* ! */ \
      else if (lvalue->Tag == ValueAddr && rvalue->Tag == ValueAddr) \
	{ \
	  /* ObjAsm implementation: when base register differs, always return false */ \
	  result = Value_Bool (lvalue->Data.Addr.r == rvalue->Data.Addr.r && lvalue->Data.Addr.i OP rvalue->Data.Addr.i); \
	} \
      else \
	{ \
	  if (gPhase != ePassOne) \
	    Error (ErrorError, "Bad operand types for %s", STRINGIFY(OP)); \
	  result = Value_Illegal (); \
	} \
    } while (0)

/**
 * <result> = <lvalue> <op> <rvalue>
 * \return Value of result.  Is ValueIllegal when operation can not be
 * computed.
 */
Value
Eval_Binop (Operator_e op,
	    const Value * restrict lvalue, const Value * restrict rvalue)
{
  assert (lvalue != rvalue);
  Value result;
  switch (op)
    {
      case eOp_Mul: /* * */
	{
	  /* Promotion for ValueFloat, ValueAddr and ValueSymbol.  */
	  if (rvalue->Tag == ValueFloat || rvalue->Tag == ValueAddr || rvalue->Tag == ValueSymbol)
	    {
	      const Value * restrict tmp = lvalue;
	      lvalue = rvalue;
	      rvalue = tmp;
	    }
	  uint32_t lval, rval;
	  bool l_isint = GetInt (lvalue, &lval);
	  bool r_isint = GetInt (rvalue, &rval);
	  if (lvalue->Tag == ValueAddr && r_isint) /* ValueAddr <= ValueAddr * ValueInt  <or>  ValueInt * ValueAddr */
	    result = Value_Addr (lvalue->Data.Addr.r, lvalue->Data.Addr.i * (signed)rval);
	  else if (lvalue->Tag == ValueSymbol && r_isint) /* ValueSymbol <= ValueSymbol * ValueInt  <or>  ValueInt * ValueSymbol */
	    result = Value_Symbol (lvalue->Data.Symbol.symbol, lvalue->Data.Symbol.factor * (signed)rval, lvalue->Data.Symbol.offset * (signed)rval);
	  else if (lvalue->Tag == ValueFloat && (r_isint || rvalue->Tag == ValueFloat)) /* ValueFloat <= ValueFloat * ValueInt  <or>  ValueInt * ValueFloat */
	    result = Value_Float (lvalue->Data.Float.f * (r_isint ? (signed)rval : rvalue->Data.Float.f));
	  else if (l_isint && r_isint)
	    result = Value_Int (lval * rval, eIntType_PureInt);
	  else
	    {
	      if (gPhase != ePassOne)
		Error (ErrorError, "Bad operand type for %s", "multiplication");
	      result = Value_Illegal ();
	    }
	  break;
	}
      
      case eOp_Div: /* / */
	{
	  uint32_t divident, divisor; /* Division is *unsigned*.  */
	  bool divident_isint = GetInt (lvalue, &divident);
	  bool divisor_isint = GetInt (rvalue, &divisor);
	  if ((!divident_isint && lvalue->Tag != ValueFloat)
	      || (!divisor_isint && rvalue->Tag != ValueFloat))
	    {
	      if (gPhase != ePassOne)
		Error (ErrorError, "Bad operand type for %s", "division");
	      result = Value_Illegal ();
	    }
	  else
	    {
	      double divisor_dbl = divisor_isint ? (double)(signed)divisor : rvalue->Data.Float.f;
	      if (divisor_dbl == 0.)
		{
		  if (gPhase != ePassOne)
		    Error (ErrorError, "Division by zero");
		  result = Value_Illegal ();
		}
	      if (divident_isint && divisor_isint)
		result = Value_Int (divident / divisor, eIntType_PureInt);
	      else if ((divident_isint || lvalue->Tag == ValueFloat)
	        && (divisor_isint || rvalue->Tag == ValueFloat))
		{
		  /* Floating point division.  */
		  double divident_dbl = divident_isint ? (double)(signed)divident : lvalue->Data.Float.f;
		  result = Value_Float (divident_dbl / divisor_dbl);
		}
	    }
	  break;
	}

      case eOp_Mod: /* :MOD: */
	{
	  uint32_t divident, modulus; /* Modulo is *unsigned*.  */
	  if (GetInt (lvalue, &divident) && GetInt (rvalue, &modulus))
	    {
	      if (modulus == 0)
		{
		  if (gPhase != ePassOne)
		    Error (ErrorError, "Division by zero");
		  result = Value_Illegal ();
		}
	      else
		result = Value_Int (divident % modulus, eIntType_PureInt);
	    }
	  else
	    {
	      if (gPhase != ePassOne)
		Error (ErrorError, "Bad operand type for %s", "modulo");
	      result = Value_Illegal ();
	    }
	  break;
	}
      
      case eOp_Add: /* + */
	{
	  /* Promotion for ValueFloat, ValueAddr and ValueSymbol.  */
	  if (rvalue->Tag == ValueFloat || rvalue->Tag == ValueAddr
	      || rvalue->Tag == ValueSymbol)
	    {
	      const Value * restrict tmp = lvalue;
	      lvalue = rvalue;
	      rvalue = tmp;
	    }

	  uint32_t lval, rval;
	  bool l_isint = GetInt (lvalue, &lval);
	  bool r_isint = GetInt (rvalue, &rval);

	  if (lvalue->Tag == ValueAddr && r_isint) /* ValueAddr <= ValueAddr + ValueInt  <or>  ValueInt + ValueAddr */
	    result = Value_Addr (lvalue->Data.Addr.r, lvalue->Data.Addr.i + rval);
	  else if (lvalue->Tag == ValueSymbol && rvalue->Tag == ValueSymbol) /* ValueSymbol <or> ValueInt <= ValueSymbol + ValueSymbol */
	    {
	      if (lvalue->Data.Symbol.symbol != rvalue->Data.Symbol.symbol)
		result = Value_Illegal ();
	      else
		{
		  int factor = lvalue->Data.Symbol.factor + rvalue->Data.Symbol.factor;
		  int offset = lvalue->Data.Symbol.offset + rvalue->Data.Symbol.offset;
		  result = factor == 0 ? Value_Int (offset, eIntType_PureInt) : Value_Symbol (lvalue->Data.Symbol.symbol, factor, offset);
		}
	    }
	  else if (lvalue->Tag == ValueSymbol && r_isint)
	    result = Value_Symbol (lvalue->Data.Symbol.symbol, lvalue->Data.Symbol.factor, lvalue->Data.Symbol.offset + rval);
	  else if (l_isint && r_isint) /* ValueInt <= ValueInt + ValueInt */
	    result = Value_Int (lval + rval, eIntType_PureInt);
	  else if (lvalue->Tag == ValueFloat
		   && (r_isint || rvalue->Tag == ValueFloat)) /* ValueFloat <= ValueFloat + ValueInt  <or>  ValueInt + ValueFloat */
	    result = Value_Float (lvalue->Data.Float.f + (r_isint ? (signed)rval : rvalue->Data.Float.f)); 
	  else
	    {
	      if (gPhase != ePassOne)
		Error (ErrorError, "Bad operand type for %s", "addition");
	      result = Value_Illegal ();
	    }
	  break;
	}

      case eOp_Sub: /* - */
	{
	  uint32_t lval, rval;
	  bool l_isint = GetInt (lvalue, &lval);
	  bool r_isint = GetInt (rvalue, &rval);

	  if (lvalue->Tag == ValueFloat && rvalue->Tag == ValueFloat) /* ValueFloat <= ValueFloat - ValueFloat */
	    result = Value_Float (lvalue->Data.Float.f - rvalue->Data.Float.f);
	  else if (lvalue->Tag == ValueFloat && r_isint)
	    result = Value_Float (lvalue->Data.Float.f - (signed)rval); /* ValueFloat <= ValueFloat - ValueInt */
	  else if (l_isint && rvalue->Tag == ValueFloat) /* ValueFloat <= ValueInt - ValueFloat */
	    result = Value_Float ((signed)lval - rvalue->Data.Float.f);
	  else if (l_isint && r_isint) /* ValueInt <= ValueInt - ValueInt */
	    result = Value_Int (lval - rval, eIntType_PureInt);
	  else if (lvalue->Tag == ValueAddr && rvalue->Tag == ValueAddr) /* ValueInt <= ValueAddr - ValueAddr */
	    {
	      if (lvalue->Data.Addr.r != rvalue->Data.Addr.r)
		{
		  if (gPhase != ePassOne)
		    Error (ErrorError, "Base registers are different in subtraction ([r%d, #x] - [r%d, #y])",
			   lvalue->Data.Addr.r, rvalue->Data.Addr.r);
		  result = Value_Illegal ();
		}
	      else
		result = Value_Int (lvalue->Data.Addr.i - rvalue->Data.Addr.i, eIntType_PureInt);
	    }
	  else if (lvalue->Tag == ValueAddr && r_isint) /* ValueAddr <= ValueAddr - ValueInt */
	    result = Value_Addr (lvalue->Data.Addr.r, lvalue->Data.Addr.i - (signed)rval);
	  else if ((lvalue->Tag == ValueSymbol || l_isint)
	           && (rvalue->Tag == ValueSymbol || r_isint))
	    {
	      if (lvalue->Tag == ValueSymbol && rvalue->Tag == ValueSymbol
	          && lvalue->Data.Symbol.symbol != rvalue->Data.Symbol.symbol)
		result = Value_Illegal ();
	      else
		{
		  assert (!(l_isint && r_isint) && "Needs to be handled elsewhere");
		  Symbol *symbol = (lvalue->Tag == ValueSymbol) ? lvalue->Data.Symbol.symbol : rvalue->Data.Symbol.symbol;
		  int factor = (lvalue->Tag == ValueSymbol) ? lvalue->Data.Symbol.factor : 0;
		  if (rvalue->Tag == ValueSymbol)
		    factor -= rvalue->Data.Symbol.factor;
		  int offset = (lvalue->Tag == ValueSymbol) ? lvalue->Data.Symbol.offset : (signed)lval;
		  offset -= (rvalue->Tag == ValueSymbol) ? rvalue->Data.Symbol.offset : (signed)rval;
		  result = factor ? Value_Symbol (symbol, factor, offset) : Value_Int (offset, eIntType_PureInt);
		}
	    }
	  else
	    {
	      if (gPhase != ePassOne)
		Error (ErrorError, "Bad operand type for %s", "subtraction");
	      result = Value_Illegal ();
	    }
	  break;
	}
      
      case eOp_Concat: /* :CC: */
	{
	  if (lvalue->Tag != ValueString || rvalue->Tag != ValueString)
	    {
	      if (gPhase != ePassOne)
		Error (ErrorError, "Bad operand type for %s", "string concatenation");
	      result = Value_Illegal ();
	    }
	  else
	    {
	      size_t len = lvalue->Data.String.len + rvalue->Data.String.len;
	      char *c;
	      if ((c = malloc (len)) == NULL)
		Error_OutOfMem ();
	      memcpy (c, lvalue->Data.String.s, lvalue->Data.String.len);
	      memcpy (c + lvalue->Data.String.len,
	        rvalue->Data.String.s, rvalue->Data.String.len);
	      result = Value_String (c, len, true);
	    }
	  break;
	}
      
      case eOp_And: /* :AND: & */
	{
	  uint32_t lval, rval;
	  bool l_isint = GetInt (lvalue, &lval);
	  if (lvalue->Tag == ValueAddr) /* FIXME: why this support, and not in :OR: or :EOR: ? */
	    {
	      lval = (unsigned)lvalue->Data.Addr.i;
	      l_isint = true;
	    }
	  bool r_isint = GetInt (rvalue, &rval);
	  if (rvalue->Tag == ValueAddr)
	    {
	      rval = rvalue->Data.Addr.i;
	      r_isint = true;
	    }
	  if (l_isint && r_isint)
	    result = Value_Int (lval & rval, eIntType_PureInt);
	  else
	    {
	      if (gPhase != ePassOne)
		Error (ErrorError, "Bad operand type for %s", ":AND:");
	      result = Value_Illegal ();
	    }
	  break;
	}
      
      case eOp_Or: /* :OR: | */
	{
	  uint32_t lval, rval;
	  bool l_isint = GetInt (lvalue, &lval);
	  bool r_isint = GetInt (rvalue, &rval);
	  if (l_isint && r_isint)
	    result = Value_Int (lval | rval, eIntType_PureInt);
	  else
	    {
	      if (gPhase != ePassOne)
		Error (ErrorError, "Bad operand type for %s", ":OR:");
	      result = Value_Illegal ();
	    }
	  break;
	}
      
      case eOp_XOr: /* :EOR: ^ */
	{
	  uint32_t lval, rval;
	  bool l_isint = GetInt (lvalue, &lval);
	  bool r_isint = GetInt (rvalue, &rval);
	  if (l_isint && r_isint)
	    result = Value_Int (lval ^ rval, eIntType_PureInt);
	  else
	    {
	      if (gPhase != ePassOne)
		Error (ErrorError, "Bad operand type for %s", ":EOR:");
	      result = Value_Illegal ();
	    }
	  break;
	}
      
      case eOp_ASR: /* >>> */
	{
	  uint32_t lval, rval;
	  bool l_isint = GetInt (lvalue, &lval);
	  bool r_isint = GetInt (rvalue, &rval);
	  if (l_isint && r_isint)
	    {
	      unsigned numbits = rval >= 32 ? 1 : 32 - rval;
	      unsigned mask = 1U << (numbits - 1);
	      uint32_t nosign = lval >> (32 - numbits);
	      result = Value_Int ((nosign ^ mask) - mask, eIntType_PureInt);
	    }
	  else
	    {
	      if (gPhase != ePassOne)
		Error (ErrorError, "Bad operand type for %s", ">>>");
	      result = Value_Illegal ();
	    }
	  break;
	}
      
      case eOp_SHR: /* >> :SHR: */
	{
	  uint32_t lval, rval;
	  bool l_isint = GetInt (lvalue, &lval);
	  bool r_isint = GetInt (rvalue, &rval);
	  if (l_isint && r_isint)
	    result = Value_Int (rval >= 32 ? 0 : lval >> rval, eIntType_PureInt);
	  else
	    {
	      if (gPhase != ePassOne)
		Error (ErrorError, "Bad operand type for %s", ">> or :SHR:");
	      result = Value_Illegal ();
	    }
	  break;
	}
      
      case eOp_SHL: /* << :SHL: */
	{
	  uint32_t lval, rval;
	  bool l_isint = GetInt (lvalue, &lval);
	  bool r_isint = GetInt (rvalue, &rval);
	  if (l_isint && r_isint)
	    result = Value_Int (rval >= 32 ? 0 : lval << rval, eIntType_PureInt);
	  else
	    {
	      if (gPhase != ePassOne)
		Error (ErrorError, "Bad operand type for %s", "<< or :SHR:");
	      result = Value_Illegal ();
	    }
	  break;
	}
      
      case eOp_ROR: /* :ROR: */
	{
	  uint32_t lval, rval;
	  bool l_isint = GetInt (lvalue, &lval);
	  bool r_isint = GetInt (rvalue, &rval);
	  if (l_isint && r_isint)
	    {
	      unsigned numbits = rval & 31;
	      result = Value_Int ((lval >> numbits) | (lval << (32 - numbits)), eIntType_PureInt);
	    }
	  else
	    {
	      if (gPhase != ePassOne)
		Error (ErrorError, "Bad operand type for %s", ":ROR:");
	      result = Value_Illegal ();
	    }
	  break;
	}
      
      case eOp_ROL: /* :ROL: */
	{
	  uint32_t lval, rval;
	  bool l_isint = GetInt (lvalue, &lval);
	  bool r_isint = GetInt (rvalue, &rval);
	  if (l_isint && r_isint)
	    {
	      unsigned numbits = rval & 31;
	      result = Value_Int ((lval << numbits) | (lval >> (32 - numbits)), eIntType_PureInt);
	    }
	  else
	    {
	      if (gPhase != ePassOne)
		Error (ErrorError, "Bad operand type for %s", ":ROL:");
	      result = Value_Illegal ();
	    }
	  break;
	}

      case eOp_LE: /* <= */
	COMPARE (<=);
	break;

      case eOp_GE: /* >= */
	COMPARE (>=);
	break;

      case eOp_LT: /* < */
	COMPARE (<);
	break;

      case eOp_GT: /* > */
	COMPARE (>);
	break;
      
      case eOp_EQ: /* = == */
	{
	  if ((lvalue->Tag & (ValueBool | ValueSymbol | ValueCode)) != 0
	      && (rvalue->Tag & (ValueBool | ValueSymbol | ValueCode)) != 0)
	    result = Value_Bool (Value_Equal (lvalue, rvalue));
	  else
	    COMPARE (==);
	  break;
	}
      
      case eOp_NE: /* <> /= != */
	{
	  if ((lvalue->Tag & (ValueBool | ValueSymbol | ValueCode)) != 0
	      && (rvalue->Tag & (ValueBool | ValueSymbol | ValueCode)) != 0)
	    result = Value_Bool (!Value_Equal (lvalue, rvalue));
	  else
	    COMPARE (!=);
	  break;
	}
      
      case eOp_LAnd: /* :LAND: && */
	{
	  if (lvalue->Tag != ValueBool || rvalue->Tag != ValueBool)
	    {
	      if (gPhase != ePassOne)
		Error (ErrorError, "Bad operand type for %s", ":LAND:");
	      result = Value_Illegal ();
	    }
	  else
	    result = Value_Bool (lvalue->Data.Bool.b && rvalue->Data.Bool.b);
	  break;
	}
      
      case eOp_LOr: /* :LOR: || */
	{
	  if (lvalue->Tag != ValueBool || rvalue->Tag != ValueBool)
	    {
	      if (gPhase != ePassOne)
		Error (ErrorError, "Bad operand type for %s", ":LOR:");
	      result = Value_Illegal ();
	    }
	  else
	    result = Value_Bool (lvalue->Data.Bool.b || rvalue->Data.Bool.b);
	  break;
	}
      
      case eOp_LEOr: /* :LEOR: */
	{
	  if (lvalue->Tag != ValueBool || rvalue->Tag != ValueBool)
	    {
	      if (gPhase != ePassOne)
		Error (ErrorError, "Bad operand type for %s", ":LEOR:");
	      result = Value_Illegal ();
	    }
	  else
	    result = Value_Bool (lvalue->Data.Bool.b ^ rvalue->Data.Bool.b);
	  break;
	}
      
      case eOp_Left: /* :LEFT: */
	{
	  uint32_t rval;
	  bool r_isint = GetInt (rvalue, &rval);
	  if (lvalue->Tag != ValueString || !r_isint)
	    {
	      if (gPhase != ePassOne)
		Error (ErrorError, "Bad operand type for %s", ":LEFT:");
	      result = Value_Illegal ();
	    }
	  else
	    {
	      if ((signed)rval < 0 || rval > lvalue->Data.String.len)
		{
		  if (gPhase != ePassOne)
		    Error (ErrorError, "Wrong number of characters (%d) specified for :LEFT:",
			   rval);
		  result = Value_Illegal ();
		}
	      else
		{
		  char *strP;
		  if ((strP = malloc (rval)) == NULL)
		    Error_OutOfMem ();
		  memcpy (strP, lvalue->Data.String.s, rval);
		  result = Value_String (strP, rval, true);
		}
	    }
	  break;
	}
      
      case eOp_Right: /* :RIGHT: */
	{
	  uint32_t rval;
	  bool r_isint = GetInt (rvalue, &rval);
	  if (lvalue->Tag != ValueString || !r_isint)
	    {
	      if (gPhase != ePassOne)
		Error (ErrorError, "Bad operand type for %s", ":RIGHT:");
	      result = Value_Illegal ();
	    }
	  else
	    {
	      if ((signed)rval < 0 || rval > lvalue->Data.String.len)
		{
		  if (gPhase != ePassOne)
		    Error (ErrorError, "Wrong number of characters (%d) specified for :RIGHT:",
			   rval);
		  result = Value_Illegal ();
		}
	      else
		{
		  char *strP;
		  if ((strP = malloc (rval)) == NULL)
		    Error_OutOfMem ();
		  memcpy (strP,
			  lvalue->Data.String.s + lvalue->Data.String.len - rval,
			  rval);
		  result = Value_String (strP, rval, true);
		}
	    }
	  break;
	}
      
      default:
	assert (0);
        result = Value_Illegal ();
	break;
    }

  return result;
}


/**
 * Try to convert a ValueAddr or ValueSymbol into ValueAddr (possibly loosing
 * relocation info.
 */
static Value
GetValueAddrEquivalent (const Value *valP)
{
  const RelocAndAddend_t relocAddend = Reloc_SplitRelocAndAddend (valP,
								  areaCurrentSymbol,
								  Area_CurIdxAligned (),
                                                                  false);
  valP = &relocAddend.addend;
  if (valP->Tag == ValueAddr)
    return *valP;
  Value illVal = { .Tag = ValueIllegal };
  return illVal;
}


/**
 * <result> = <op> <value>
 * \return Value of result.  Is ValueIllegal when operation can not be
 * computed.
 */
Value
Eval_Unop (Operator_e op, const Value *value)
{
  Value result;
  switch (op)
    {
      case eOp_FLoad:
	{
	  if (value->Tag != ValueString)
	    result = Value_Illegal ();
	  else
	    {
	      char *s;
	      if ((s = strndup (value->Data.String.s, value->Data.String.len)) == NULL)
		Error_OutOfMem ();

	      uint32_t i;
	      ASFile asFile;
	      if (Include_Find (s, &asFile))
		{
		  Error (ErrorError, "Cannot access file \"%s\"", s);
		  i = 0;
		}
	      else
		{
		  /* Register canonicalised filename so that it gets output
		     in the depends file.  */
		  (void) FS_StoreFileName (asFile.canonName);

		  i = asFile.loadAddress;
		}
	      result = Value_Int (i, eIntType_PureInt);
	      ASFile_Free (&asFile);
	      free (s);
	    }
	  break;
	}
	
      case eOp_FExec:
	{
	  if (value->Tag != ValueString)
	    result = Value_Illegal ();
	  else
	    {
	      char *s;
	      if ((s = strndup (value->Data.String.s, value->Data.String.len)) == NULL)
		Error_OutOfMem();

	      uint32_t i;
	      ASFile asFile;
	      if (Include_Find (s, &asFile))
		{
		  Error (ErrorError, "Cannot access file \"%s\"", s);
		  i = 0;
		}
	      else
		{
		  /* Register canonicalised filename so that it gets output
		     in the depends file.  */
		  (void) FS_StoreFileName (asFile.canonName);

		  i = asFile.execAddress;
		}
	      result = Value_Int (i, eIntType_PureInt);
	      ASFile_Free (&asFile);
	      free (s);
	    }
	  break;
	}

      case eOp_FSize:
	{
	  if (value->Tag != ValueString)
	    result = Value_Illegal ();
	  else
	    {
	      char *s;
	      if ((s = strndup (value->Data.String.s, value->Data.String.len)) == NULL)
		Error_OutOfMem();

	      uint32_t i;
	      ASFile asFile;
	      if (Include_Find (s, &asFile))
		{
		  Error (ErrorError, "Cannot access file \"%s\"", s);
		  i = 0;
		}
	      else
		{
		  /* Register canonicalised filename so that it gets output
		     in the depends file.  */
		  (void) FS_StoreFileName (asFile.canonName);

		  if (asFile.size > UINT32_MAX)
		    {
		      Error (ErrorWarning, "File size of \"%s\" is bigger than unsigned 32-bit integer", s);
		      i = UINT32_MAX;
		    }
		  else
		    i = asFile.size;
		}
	      result = Value_Int (i, eIntType_PureInt);
	      ASFile_Free (&asFile);
	      free (s);
	    }
	  break;
	}

      case eOp_FAttr:
	{
	  if (value->Tag != ValueString)
	    result = Value_Illegal ();
	  else
	    {
	      char *s;
	      if ((s = strndup (value->Data.String.s, value->Data.String.len)) == NULL)
		Error_OutOfMem();

	      uint32_t i;
	      ASFile asFile;
	      if (Include_Find (s, &asFile))
		{
		  Error (ErrorError, "Cannot access file \"%s\"", s);
		  i = 0;
		}
	      else
		{
		  /* Register canonicalised filename so that it gets output
		     in the depends file.  */
		  (void) FS_StoreFileName (asFile.canonName);

		  i = asFile.attribs;
		}
	      result = Value_Int (i, eIntType_PureInt);
	      ASFile_Free (&asFile);
	      free (s);
	    }
	  break;
	}

      case eOp_LNot: /* :LNOT: ! */
	{
	  if (value->Tag != ValueBool)
	    {
	      if (gPhase != ePassOne)
		Error (ErrorError, "Bad operand type for %s", ":LNOT:");
	      result = Value_Illegal ();
	    }
	  else
	    result = Value_Bool (!value->Data.Bool.b);
	  break;
	}

      case eOp_Not: /* :NOT: ~ */
	{
	  uint32_t i;
	  if (!GetInt (value, &i))
	    {
	      if (gPhase != ePassOne)
		Error (ErrorError, "Bad operand type for %s", ":NOT:");
	      result = Value_Illegal ();
	    }
	  else
	    result = Value_Int (~i, eIntType_PureInt);
	  break;
	}

      case eOp_Neg: /* Unary - */
	{
	  uint32_t i;
	  if (GetInt (value, &i))
	    result = Value_Int (-i, eIntType_PureInt);
	  else if (value->Tag == ValueInt64)
	    result = Value_Int64 (-value->Data.Int64.i);
	  else if (value->Tag == ValueFloat)
	    result = Value_Float (-value->Data.Float.f);
	  else if (value->Tag == ValueSymbol)
	    result = Value_Symbol (value->Data.Symbol.symbol, -value->Data.Symbol.factor, -value->Data.Symbol.offset);
	  else
	    {
	      if (gPhase != ePassOne)
		Error (ErrorError, "Bad operand type for %s", "negation");
	      result = Value_Illegal ();
	    }
	  break;
	}

      case eOp_Pos: /* Unary + */
	{
	  if (value->Tag == ValueInt || value->Tag == ValueInt64
	      || value->Tag == ValueFloat)
	    result = Value_Copy (value);
	  else
	    {
	      if (gPhase != ePassOne)
		Error (ErrorError, "Bad operand type for %s", "+");
	      result = Value_Illegal ();
	    }
	  break;
	}

      case eOp_Base: /* :BASE: */
	{
	  Value addrVal = GetValueAddrEquivalent (value);
	  switch (addrVal.Tag)
	    {
	      case ValueAddr:
		{
		  result = Value_Int (addrVal.Data.Addr.r, eIntType_PureInt);
		  break;
		}

	      default:
		{
		  if (gPhase != ePassOne)
		    Error (ErrorError, "Bad operand type for %s", ":BASE:");
		  result = Value_Illegal ();
		  break;
		}
	    }
	  break;
	}

      case eOp_Index: /* :INDEX: */
	{
	  if (value->Tag == ValueInt)
	    result = *value;
	  else
	    {
	      Value addrVal = GetValueAddrEquivalent (value);
	      switch (addrVal.Tag)
		{
		  case ValueAddr:
		    {
		      result = addrVal.Data.Addr.r ? Value_Int (addrVal.Data.Addr.i, eIntType_PureInt) : *value;
		      break;
		    }

		  default:
		    {
		      if (gPhase != ePassOne)
			Error (ErrorError, "Bad operand type for %s", ":INDEX:");
		      result = Value_Illegal ();
		      break;
		    }
		}
	    }
	  break;
	}

      case eOp_Len: /* :LEN: */
	{
	  if (value->Tag != ValueString)
	    {
	      if (gPhase != ePassOne)
		Error (ErrorError, "Bad operand type for %s", ":LEN:");
	      result = Value_Illegal ();
	    }
	  else
	    result = Value_Int (value->Data.String.len, eIntType_PureInt);
	  break;
	}

      case eOp_Str: /* :STR: */
	{
	  Value tempValue;
	  if (value->Tag == ValueSymbol
	      && (value->Data.Symbol.symbol->attr.type & SYMBOL_AREA) != 0
	      && (value->Data.Symbol.symbol->attr.area->type & AREA_ABS) != 0)
	    {
	      int factor = value->Data.Symbol.factor;
	      int offset= value->Data.Symbol.offset;
	      tempValue = Value_Int (factor * Area_GetBaseAddress (value->Data.Symbol.symbol) + offset, eIntType_PureInt);
	      value = &tempValue;
	    }
	  switch (value->Tag)
	    {
	      default:
		if (gPhase != ePassOne)
		  Error (ErrorError, "Bad operand type for %s", ":STR:");
		result = Value_Illegal ();
		break;

	      case ValueBool:
	      case ValueInt:
	      case ValueFloat:
		{
		  size_t len;
		  char num[32];
		  switch (value->Tag)
		    {
		      case ValueBool:
			len = sprintf (num, "%c", value->Data.Bool.b ? 'T' : 'F');
			break;
		      case ValueInt:
			len = sprintf (num, "%.8X", value->Data.Int.i);
			break;
		      case ValueFloat:
			len = sprintf (num, "%f", value->Data.Float.f);
			break;
		    }
		  char *c;
		  if ((c = malloc (len)) == NULL)
		    Error_OutOfMem();
		  memcpy (c, num, len);
		  result = Value_String (c, len, true);
		}
	    }
	  break;
	}

      case eOp_Chr: /* :CHR: */
	{
	  uint32_t i;
	  if (!GetInt (value, &i))
	    {
	      if (gPhase != ePassOne)
		Error (ErrorError, "Bad operand type for %s", ":CHR:");
	      result = Value_Illegal ();
	    }
	  else
	    {
	      if (i >= 256)
		Error (ErrorWarning, "Value %d will be truncated for :CHR: use", i);
	      char *c;
	      if ((c = malloc (1)) == NULL)
		Error_OutOfMem ();
	      *c = i;
	      result = Value_String (c, 1, true);
	    }
	  break;
	}

      case eOp_Size: /* ?<label> */
	{
	  if (value->Tag != ValueSymbol)
	    {
	      Error (ErrorError, "Bad operand type for %s", "? operator");
	      result = Value_Illegal ();
	    }
	  else if ((value->Data.Symbol.symbol->attr.type & (SYMBOL_DEFINED | SYMBOL_COMMON)) != 0)
	    result = Value_Int (value->Data.Symbol.symbol->attr.codeSize, eIntType_PureInt);
	  else if (value->Data.Symbol.symbol->attr.type & SYMBOL_AREA)
	    {
	      if (gPhase == ePassTwo
		  && value->Data.Symbol.symbol->attr.area->curIdx != value->Data.Symbol.symbol->attr.area->maxIdx)
		Error (ErrorError, "? on area symbol which gets extended later on");
	      result = Value_Int (value->Data.Symbol.symbol->attr.area->curIdx, eIntType_PureInt);
	    }
	  else
	    {
	      if (gPhase != ePassOne)
		Error (ErrorError, "Undefined symbol");
	      result = Value_Illegal ();
	    }
	  break;
	}

      case eOp_LowerCase: /* :LOWERCASE: */
	{
	  if (value->Tag != ValueString)
	    result = Value_Illegal ();
	  else
	    {
	      result = Value_Copy (value);
	      unsigned char *str = (unsigned char *)result.Data.String.s;
	      for (size_t i = 0; i != result.Data.String.len; ++i)
		str[i] = tolower (str[i]);
	    }
	  break;
	}

      case eOp_UpperCase: /* :UPPERCASE: */
	{
	  if (value->Tag != ValueString)
	    result = Value_Illegal ();
	  else
	    {
	      result = Value_Copy (value);
	      unsigned char *str = (unsigned char *)result.Data.String.s;
	      for (size_t i = 0; i != result.Data.String.len; ++i)
		str[i] = toupper (str[i]);
	    }
	  break;
	}

      case eOp_RevCC: /* :REVERSE_CC: */
	{
	  if (value->Tag != ValueString)
	    result = Value_Illegal ();
	  else
	    {
	      unsigned ccode = GetCCode (value->Data.String.s, value->Data.String.len);
	      if (ccode == UINT_MAX)
		{
		  Error (ErrorError, "Condition code %.*s is not known",
			 (int)value->Data.String.len, value->Data.String.s);
		  result = Value_Illegal ();
		}
	      else
		{
		  unsigned revCCode = ccode ^ 1;
		  unsigned isUpcase = (value->Data.String.len ? (value->Data.String.s[0] & 0x20) : 0) ^ 0x20;
		  /* Input string length is 0 or 2.  */
		  char *str;
		  if ((str = malloc (2)) == NULL)
		    Error_OutOfMem ();
		  str[0] = oLowcaseCCodes[revCCode*2 + 0] ^ isUpcase;
		  str[1] = oLowcaseCCodes[revCCode*2 + 1] ^ isUpcase;
		  result = Value_String (str, 2, true);
		}
	    }
	  break;
	}

      case eOp_CCEnc: /* :CC_ENCODING: */
	{
	  if (value->Tag != ValueString)
	    result = Value_Illegal ();
	  else
	    {
	      unsigned ccode = GetCCode (value->Data.String.s, value->Data.String.len);
	      if (ccode == UINT_MAX)
		{
		  Error (ErrorError, "Condition code %.*s is not known",
			 (int)value->Data.String.len, value->Data.String.s);
		  result = Value_Illegal ();
		}
	      else
		{
		  if (ccode >= 16)
		    ccode += -16 + 2;
		  result = Value_Int (ccode << 28, eIntType_PureInt);
		}
	    }
	  break;
	}

      case eOp_RConst: /* :RCONST: */
	{
	  if (value->Tag != ValueInt && value->Data.Int.type == eIntType_PureInt)
	    result = Value_Illegal ();
	  else
	    result = Value_Int (value->Data.Int.i, eIntType_PureInt);
	  break;
	}

      default:
	assert (0);
	result = Value_Illegal ();
	break;
    }

  return result;
}
