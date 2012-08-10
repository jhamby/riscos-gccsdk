/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2002-2012 GCCSDK Developers
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
 * eval.c
 */

#include "config.h"

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "area.h"
#include "code.h"
#include "common.h"
#include "error.h"
#include "eval.h"
#include "global.h"
#include "include.h"
#include "main.h"
#include "phase.h"

static const char oLowcaseCCodes[] = "eqnecsccmiplvsvchilsgeltgtlealnvhslo";

/**
 * \param str Pointer to condition code (might be NULL but then len should be
 * 0 as well).
 * \param len Length of the condition code.
 * \return The condition code.  Values 16 and 17 are respectively "HS" and
 * "LO" (those correspond to condition codes 2 and 3).
 * Is -1 when condition code is not known.
 */
static int
GetCCode (const char *str, size_t len)
{
  if (len == 0)
    return 14;
  if (len != 2)
    return -1;
  if ((str[0] & 0x20) != (str[1] & 0x20))
    return -1;
  const char lowCase0 = str[0] | 0x20;
  const char lowCase1 = str[1] | 0x20;
  for (int result = 0; result != (int)(sizeof (oLowcaseCCodes)-1); result += 2)
    {
      if (lowCase0 == oLowcaseCCodes[result + 0] && lowCase1 == oLowcaseCCodes[result + 1])
	return result / 2;
    }
  return -1;
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
 * \return true when it concerns a ValueInt or one character ValueString object.
 */
static bool
GetInt (const Value *val, uint32_t *i)
{
  switch (val->Tag)
    {
      case ValueInt:
	*i = (unsigned)val->Data.Int.i;
	return true;
      case ValueString:
	if (val->Data.String.len == 1)
	  {
	    *i = (uint8_t)val->Data.String.s[0];
	    return true;
	  }
	/* Fall through */
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
	lvalue->Data.Bool.b = lvalue->Data.Float.f OP rvalue->Data.Float.f; \
      else if (lvalue->Tag == ValueString && rvalue->Tag == ValueString) \
	lvalue->Data.Bool.b = ememcmp (lvalue, rvalue) OP 0; \
      else if (GetInt (lvalue, &lint) && GetInt (rvalue, &rint)) \
	{ \
	  /* Comparing integers happens *unsigned* ! */ \
	  lvalue->Data.Bool.b = lint OP rint; \
	} \
      else if (lvalue->Tag == ValueAddr && rvalue->Tag == ValueAddr) \
	{ \
	  /* ObjAsm implementation: when base register differs, always return false */ \
	  if (lvalue->Data.Addr.r != rvalue->Data.Addr.r) \
	    lvalue->Data.Bool.b = false; \
	  else \
	    lvalue->Data.Bool.b = lvalue->Data.Addr.i OP rvalue->Data.Addr.i; \
	} \
      else \
	{ \
	  if (gPhase != ePassOne) \
	    error (ErrorError, "Bad operand types for %s", STRINGIFY(OP)); \
	  return false; \
	} \
      lvalue->Tag = ValueBool; \
    } while (0)

/**
 * Do <lvalue> = <lvalue> <op> <rvalue>
 * \return true for success, false for failure (e.g. wrong operand types being
 * used for given operation).
 */
bool
evalBinop (Operator_e op, Value * restrict lvalue, const Value * restrict rvalue)
{
  assert (lvalue != rvalue);
  switch (op)
    {
      case eOp_Mul: /* * */
	{
	  /* ValueAddr * ValueAddr does not make sense.  */
	  uint32_t lval, rval;
	  bool l_isint = GetInt (lvalue, &lval);
	  bool r_isint = GetInt (rvalue, &rval);	 
	  if ((lvalue->Tag == ValueAddr && r_isint)
	      || (l_isint && rvalue->Tag == ValueAddr))
	    {
	      /* ValueInt * ValueAddr and ValueAddr * ValueInt.  */
	      signed val = r_isint ? (signed)rval : (signed)lval;
	      if (!r_isint)
		*lvalue = *rvalue;
	      lvalue->Data.Addr.i *= val;
	    }
	  else if ((lvalue->Tag == ValueSymbol && r_isint)
		   || (l_isint && rvalue->Tag == ValueSymbol))
	    {
	      /* ValueInt * ValueSymbol and ValueSymbol * ValueInt.  */
	      signed val = r_isint ? (signed)rval : (signed)lval;
	      if (!r_isint)
		*lvalue = *rvalue;
	      lvalue->Data.Symbol.factor *= val;
	      lvalue->Data.Symbol.offset *= val;
	    }
	  else if (l_isint && r_isint)
	    {
	      lvalue->Tag = ValueInt;
	      lvalue->Data.Int.i = lval * rval;
	    }
	  else if ((l_isint || lvalue->Tag == ValueFloat) && (r_isint || rvalue->Tag == ValueFloat))
	    {
	      lvalue->Tag = ValueFloat;
	      lvalue->Data.Float.f = (l_isint ? lval : lvalue->Data.Float.f) * (r_isint ? rval : rvalue->Data.Float.f);
	    }
	  else
	    {
	      if (gPhase != ePassOne)
		error (ErrorError, "Bad operand type for %s", "multiplication");
	      return false;
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
		error (ErrorError, "Bad operand type for %s", "division");
	      return false;
	    }
	  double divisor_dbl = divisor_isint ? (double)(signed)divisor : rvalue->Data.Float.f;
	  if (divisor_dbl == 0.)
	    {
	      if (gPhase != ePassOne)
		error (ErrorError, "Division by zero");
	      return false;
	    }
	  if (divident_isint && divisor_isint)
	    {
	      lvalue->Tag = ValueInt;
	      lvalue->Data.Int.i = divident / divisor;
	    }
	  else if ((divident_isint || lvalue->Tag == ValueFloat)
		   && (divisor_isint || rvalue->Tag == ValueFloat))
	    {
	      /* Floating point division.  */
	      double divident_dbl = divident_isint ? (double)(signed)divident : lvalue->Data.Float.f;
	      lvalue->Tag = ValueFloat;
	      lvalue->Data.Float.f = divident_dbl / divisor_dbl;
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
		    error (ErrorError, "Division by zero");
		  return false;
		}

	      lvalue->Tag = ValueInt;
	      lvalue->Data.Int.i = divident % modulus;
	    }
	  else
	    {
	      if (gPhase != ePassOne)
		error (ErrorError, "Bad operand type for %s", "modulo");
	      return false;
	    }
	  break;
	}
      
      case eOp_Add: /* + */
	{
	  Value rhs;
	  /* Promotion for ValueFloat, ValueAddr and ValueSymbol.  */
	  if (rvalue->Tag == ValueFloat || rvalue->Tag == ValueAddr
	      || rvalue->Tag == ValueSymbol)
	    {
	      rhs = *lvalue;
	      *lvalue = *rvalue;
	    }
	  else
	    rhs = *rvalue;

	  uint32_t lval, rval;
	  bool l_isint = GetInt (lvalue, &lval);
	  bool r_isint = GetInt (&rhs, &rval);

	  if (lvalue->Tag == ValueAddr && r_isint)
	    lvalue->Data.Addr.i += rval; /* <addr> + <int> -> <addr> */
	  else if (lvalue->Tag == ValueSymbol && rhs.Tag == ValueSymbol)
	    {
	      if (lvalue->Data.Symbol.symbol != rhs.Data.Symbol.symbol)
		return false;
	      lvalue->Data.Symbol.factor += rhs.Data.Symbol.factor;
	      lvalue->Data.Symbol.offset += rhs.Data.Symbol.offset;
	      if (lvalue->Data.Symbol.factor == 0)
		{
		  lvalue->Tag = ValueInt;
		  lvalue->Data.Int.i = lvalue->Data.Symbol.offset;
		}
	    }
	  else if (lvalue->Tag == ValueSymbol && r_isint)
	    {
	      lvalue->Data.Symbol.offset += (signed)rval;
	    }
	  else if (l_isint && r_isint)
	    {
	      lvalue->Tag = ValueInt;
	      lvalue->Data.Int.i = lval + rval; /* <int> + <int> -> <int> */
	    }
	  else if (lvalue->Tag == ValueFloat
		   && (r_isint || rhs.Tag == ValueFloat))
	    lvalue->Data.Float.f += (r_isint ? (signed)rval : rhs.Data.Float.f); /* <float>/<signed int> + <float>/<signed/int> -> <float> */
	  else
	    {
	      if (gPhase != ePassOne)
		error (ErrorError, "Bad operand type for %s", "addition");
	      return false;
	    }
	  break;
	}

      case eOp_Sub: /* - */
	{
	  uint32_t lval, rval;
	  bool l_isint = GetInt (lvalue, &lval);
	  bool r_isint = GetInt (rvalue, &rval);

	  if (lvalue->Tag == ValueFloat && rvalue->Tag == ValueFloat)
	    lvalue->Data.Float.f -= rvalue->Data.Float.f; /* <float> - <float> -> <float> */
	  else if (lvalue->Tag == ValueFloat && r_isint)
	    lvalue->Data.Float.f -= (signed)rval; /* <float> - <signed int> -> <float> */
	  else if (l_isint && rvalue->Tag == ValueFloat)
	    {
	      /* <signed int> - <float> -> <float> */
	      lvalue->Tag = ValueFloat;
	      lvalue->Data.Float.f = (double)(signed)lval - rvalue->Data.Float.f;
	    }
	  else if (l_isint && r_isint)
	    {
	      lvalue->Tag = ValueInt;
	      lvalue->Data.Int.i = lval - rval; /* <int> - <int> -> <int> */
	    }
	  else if (lvalue->Tag == ValueAddr && rvalue->Tag == ValueAddr)
	    {
	      /* <addr> - <addr> -> <int> */
	      if (lvalue->Data.Addr.r != rvalue->Data.Addr.r)
		{
		  if (gPhase != ePassOne)
		    error (ErrorError, "Base registers are different in subtraction ([r%d, #x] - [r%d, #y])",
			   lvalue->Data.Addr.r, rvalue->Data.Addr.r);
		  return false;
		}
	      lvalue->Tag = ValueInt;
	      lvalue->Data.Int.i = lvalue->Data.Addr.i - rvalue->Data.Addr.i;
	    }
	  else if (lvalue->Tag == ValueAddr && r_isint)
	    {
	      /* <addr> - <int> -> <addr> */
	      lvalue->Tag = ValueAddr;
	      lvalue->Data.Addr.i -= (signed)rval;
	    }
	  else if ((lvalue->Tag == ValueSymbol || l_isint)
	           && (rvalue->Tag == ValueSymbol || r_isint))
	    {
	      if (lvalue->Tag == ValueSymbol && rvalue->Tag == ValueSymbol
	          && lvalue->Data.Symbol.symbol != rvalue->Data.Symbol.symbol)
		return false;
	      assert (!(l_isint && r_isint) && "Needs to be handled elsewhere");
	      Symbol *symbol = (lvalue->Tag == ValueSymbol) ? lvalue->Data.Symbol.symbol : rvalue->Data.Symbol.symbol;
	      int factor = (lvalue->Tag == ValueSymbol) ? lvalue->Data.Symbol.factor : 0;
	      if (rvalue->Tag == ValueSymbol)
		factor -= rvalue->Data.Symbol.factor;
	      int offset = (lvalue->Tag == ValueSymbol) ? lvalue->Data.Symbol.offset : (signed)lval;
	      offset -= (rvalue->Tag == ValueSymbol) ? rvalue->Data.Symbol.offset : (signed)rval;
	      *lvalue = factor ? Value_Symbol (symbol, factor, offset) : Value_Int (offset, eIntType_PureInt);
	    }
	  else
	    {
	      if (gPhase != ePassOne)
		error (ErrorError, "Bad operand type for %s", "subtraction");
	      return false;
	    }
	  break;
	}
      
      case eOp_Concat: /* :CC: */
	{
	  if (lvalue->Tag != ValueString || rvalue->Tag != ValueString)
	    {
	      if (gPhase != ePassOne)
		error (ErrorError, "Bad operand type for %s", "string concatenation");
	      return false;
	    }
	  char *c;
	  if ((c = malloc (lvalue->Data.String.len + rvalue->Data.String.len)) == NULL)
	    errorOutOfMem();
	  memcpy (c, lvalue->Data.String.s, lvalue->Data.String.len);
	  memcpy (c + lvalue->Data.String.len,
		  rvalue->Data.String.s, rvalue->Data.String.len);
	  lvalue->Data.String.s = c;
	  lvalue->Data.String.len += rvalue->Data.String.len;
	  break;
	}
      
      case eOp_And: /* :AND: & */
	{
	  uint32_t lval, rval;
	  bool l_isint = GetInt (lvalue, &lval);
	  bool r_isint = GetInt (rvalue, &rval);
	  if ((lvalue->Tag == ValueAddr && r_isint)
	      || (l_isint && rvalue->Tag == ValueAddr)
	      || (l_isint && r_isint))
	    {
	      lvalue->Tag = ValueInt;
	      lvalue->Data.Int.i = (l_isint ? lval : (unsigned)lvalue->Data.Addr.i) & (r_isint ? rval : (unsigned)rvalue->Data.Addr.i);
	    }
	  else
	    {
	      if (gPhase != ePassOne)
		error (ErrorError, "Bad operand type for %s", ":AND:");
	      return false;
	    }
	  break;
	}
      
      case eOp_Or: /* :OR: | */
	{
	  uint32_t lval, rval;
	  bool l_isint = GetInt (lvalue, &lval);
	  bool r_isint = GetInt (rvalue, &rval);
	  if (l_isint && r_isint)
	    {
	      lvalue->Tag = ValueInt;
	      lvalue->Data.Int.i = lval | rval;
	    }
	  else
	    {
	      if (gPhase != ePassOne)
		error (ErrorError, "Bad operand type for %s", ":OR:");
	      return false;
	    }
	  break;
	}
      
      case eOp_XOr: /* :EOR: ^ */
	{
	  uint32_t lval, rval;
	  bool l_isint = GetInt (lvalue, &lval);
	  bool r_isint = GetInt (rvalue, &rval);
	  if (l_isint && r_isint)
	    {
	      lvalue->Tag = ValueInt;
	      lvalue->Data.Int.i = lval ^ rval;
	    }
	  else
	    {
	      if (gPhase != ePassOne)
		error (ErrorError, "Bad operand type for %s", ":EOR:");
	      return false;
	    }
	  break;
	}
      
      case eOp_ASR: /* >>> */
	{
	  uint32_t lval;
	  bool l_isint = GetInt (lvalue, &lval);
	  if (l_isint && rvalue->Tag == ValueInt)
	    {
	      lvalue->Tag = ValueInt;
	      unsigned numbits = (unsigned)rvalue->Data.Int.i >= 32 ? 1 : 32 - (unsigned)rvalue->Data.Int.i;
	      unsigned mask = 1U << (numbits - 1);
	      uint32_t nosign = lval >> (32 - numbits);
	      lvalue->Data.Int.i = (nosign ^ mask) - mask;
	    }
	  else
	    {
	      if (gPhase != ePassOne)
		error (ErrorError, "Bad operand type for %s", ">>>");
	      return false;
	    }
	  break;
	}
      
      case eOp_SHR: /* >> :SHR: */
	{
	  uint32_t lval;
	  bool l_isint = GetInt (lvalue, &lval);
	  if (l_isint && rvalue->Tag == ValueInt)
	    {
	      lvalue->Tag = ValueInt;
	      lvalue->Data.Int.i = (unsigned)rvalue->Data.Int.i >= 32 ? 0 : lval >> rvalue->Data.Int.i;
	    }
	  else
	    {
	      if (gPhase != ePassOne)
		error (ErrorError, "Bad operand type for %s", ">> or :SHR:");
	      return false;
	    }
	  break;
	}
      
      case eOp_SHL: /* << :SHL: */
	{
	  uint32_t lval;
	  bool l_isint = GetInt (lvalue, &lval);
	  if (l_isint && rvalue->Tag == ValueInt)
	    {
	      lvalue->Tag = ValueInt;
	      lvalue->Data.Int.i = (unsigned)rvalue->Data.Int.i >= 32 ? 0 : lval << rvalue->Data.Int.i;
	    }
	  else
	    {
	      if (gPhase != ePassOne)
		error (ErrorError, "Bad operand type for %s", "<< or :SHR:");
	      return false;
	    }
	  break;
	}
      
      case eOp_ROR: /* :ROR: */
	{
	  uint32_t lval;
	  bool l_isint = GetInt (lvalue, &lval);
	  if (l_isint && rvalue->Tag == ValueInt)
	    {
	      lvalue->Tag = ValueInt;
	      unsigned numbits = rvalue->Data.Int.i & 31;
	      lvalue->Data.Int.i = (lval >> numbits) | (lval << (32 - numbits));
	    }
	  else
	    {
	      if (gPhase != ePassOne)
		error (ErrorError, "Bad operand type for %s", ":ROR:");
	      return false;
	    }
	  break;
	}
      
    case eOp_ROL: /* :ROL: */
	{
	  uint32_t lval;
	  bool l_isint = GetInt (lvalue, &lval);
	  if (l_isint && rvalue->Tag == ValueInt)
	    {
	      lvalue->Tag = ValueInt;
	      unsigned numbits = rvalue->Data.Int.i & 31;
	      lvalue->Data.Int.i = (lval << numbits) | (lval >> (32 - numbits));
	    }
	  else
	    {
	      if (gPhase != ePassOne)
		error (ErrorError, "Bad operand type for %s", ":ROL:");
	      return false;
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
	    {
	      lvalue->Data.Bool.b = valueEqual (lvalue, rvalue);
	      lvalue->Tag = ValueBool;
	    }
	  else
	    COMPARE (==);
	  break;
	}
      
      case eOp_NE: /* <> /= != */
	{
	  if ((lvalue->Tag & (ValueBool | ValueSymbol | ValueCode)) != 0
	      && (rvalue->Tag & (ValueBool | ValueSymbol | ValueCode)) != 0)
	    {
	      lvalue->Data.Bool.b = !valueEqual (lvalue, rvalue);
	      lvalue->Tag = ValueBool;
	    }
	  else
	    COMPARE (!=);
	  break;
	}
      
      case eOp_LAnd: /* :LAND: && */
	{
	  if (lvalue->Tag != ValueBool || rvalue->Tag != ValueBool)
	    {
	      if (gPhase != ePassOne)
		error (ErrorError, "Bad operand type for %s", ":LAND:");
	      return false;
	    }
	  lvalue->Data.Bool.b = lvalue->Data.Bool.b && rvalue->Data.Bool.b;
	  break;
	}
      
      case eOp_LOr: /* :LOR: || */
	{
	  if (lvalue->Tag != ValueBool || rvalue->Tag != ValueBool)
	    {
	      if (gPhase != ePassOne)
		error (ErrorError, "Bad operand type for %s", ":LOR:");
	      return false;
	    }
	  lvalue->Data.Bool.b = lvalue->Data.Bool.b || rvalue->Data.Bool.b;
	  break;
	}
      
      case eOp_LEOr: /* :LEOR: */
	{
	  if (lvalue->Tag != ValueBool || rvalue->Tag != ValueBool)
	    {
	      if (gPhase != ePassOne)
		error (ErrorError, "Bad operand type for %s", ":LEOR:");
	      return false;
	    }
	  lvalue->Data.Bool.b = lvalue->Data.Bool.b ^ rvalue->Data.Bool.b;
	  break;
	}
      
      case eOp_Left: /* :LEFT: */
	{
	  if (lvalue->Tag != ValueString || rvalue->Tag != ValueInt)
	    {
	      if (gPhase != ePassOne)
		error (ErrorError, "Bad operand type for %s", ":LEFT:");
	      return false;
	    }
	  if (rvalue->Data.Int.i < 0 || (size_t)rvalue->Data.Int.i > lvalue->Data.String.len)
	    {
	      if (gPhase != ePassOne)
		error (ErrorError, "Wrong number of characters (%d) specified for :LEFT:",
		       rvalue->Data.Int.i);
	      return false;
	    }
	  lvalue->Data.String.len = rvalue->Data.Int.i;
	  break;
	}
      
      case eOp_Right: /* :RIGHT: */
	{
	  if (lvalue->Tag != ValueString || rvalue->Tag != ValueInt)
	    {
	      if (gPhase != ePassOne)
		error (ErrorError, "Bad operand type for %s", ":RIGHT:");
	      return false;
	    }
	  if (rvalue->Data.Int.i < 0 || (size_t)rvalue->Data.Int.i > lvalue->Data.String.len)
	    {
	      if (gPhase != ePassOne)
		error (ErrorError, "Wrong number of characters (%d) specified for :RIGHT:",
		       rvalue->Data.Int.i);
	      return false;
	    }
	  char *c;
	  if ((c = malloc (rvalue->Data.Int.i)) == NULL)
	    errorOutOfMem ();
	  memcpy (c,
		  lvalue->Data.String.s + lvalue->Data.String.len - rvalue->Data.Int.i,
		  rvalue->Data.Int.i);
	  /* free ((void *)lvalue->Data.String.s); FIXME: enable this ? */
	  lvalue->Data.String.s = c;
	  lvalue->Data.String.len = rvalue->Data.Int.i;
	  break;
	}
      
      default:
        error (ErrorError, "Illegal binary operator");
        return false;
    }

  return true;
}

/**
 * Negates given value.
 */
static bool
Eval_NegValue (Value *value)
{
  switch (value->Tag)
    {
      case ValueInt:
	value->Data.Int.i = -value->Data.Int.i;
	break;

      case ValueInt64:
	value->Data.Int64.i = -value->Data.Int64.i;
	break;
	
      case ValueFloat:
	value->Data.Float.f = -value->Data.Float.f;
	break;

      case ValueString:
	{
	  if (value->Data.String.len != 1)
	    return false;
	  int c = (unsigned char)value->Data.String.s[0];
	  value->Tag = ValueInt;
	  value->Data.Int.i = -c;
	}
	break;

#if 0
      case ValueAddr:
	value->Data.Addr.i = -value->Data.Addr.i;
	break;
#endif

      case ValueSymbol:
	value->Data.Symbol.factor = -value->Data.Symbol.factor;
	value->Data.Symbol.offset = -value->Data.Symbol.offset;
	break;

#if 0
      case ValueCode:
	for (size_t i = 0; i != value->Data.Code.len; ++i)
	  {
	    /* Negate only the values (not operations).  */
	    /* FIXME: this is not always correct.  */
	    if (value->Data.Code.c[i].Tag == CodeValue)
	      {
		if (!Eval_NegValue (&value->Data.Code.c[i].Data.value))
		  return false;
	      }
	  }
	break;
#endif

      default:
	return false;
    }

  return true;
}

/**
 * <value> = <op> <value>
 * \return true for success, false for failure (e.g. wrong operand types being
 * used for given operation).
 */
bool
evalUnop (Operator_e op, Value *value)
{
  switch (op)
    {
      case eOp_FLoad:
	{
	  if (value->Tag != ValueString)
	    return false;
	  char *s;
	  if ((s = strndup (value->Data.String.s, value->Data.String.len)) == NULL)
	    errorOutOfMem();

	  value->Tag = ValueInt;

	  ASFile asFile;
	  if (Include_Find (s, &asFile, true))
	    {
	      error (ErrorError, "Cannot access file \"%s\"", s);
	      value->Data.Int.i = 0;
	    }
	  else
	    value->Data.Int.i = asFile.loadAddress;
	  ASFile_Free (&asFile);
	  free (s);
	  break;
	}
	
      case eOp_FExec:
	{
	  if (value->Tag != ValueString)
	    return false;
	  char *s;
	  if ((s = strndup (value->Data.String.s, value->Data.String.len)) == NULL)
	    errorOutOfMem();

	  value->Tag = ValueInt;

	  ASFile asFile;
	  if (Include_Find (s, &asFile, true))
	    {
	      error (ErrorError, "Cannot access file \"%s\"", s);
	      value->Data.Int.i = 0;
	    }
	  else
	    value->Data.Int.i = asFile.execAddress;
	  ASFile_Free (&asFile);
	  free (s);
	  break;
	}

      case eOp_FSize:
	{
	  if (value->Tag != ValueString)
	    return false;
	  char *s;
	  if ((s = strndup (value->Data.String.s, value->Data.String.len)) == NULL)
	    errorOutOfMem();

	  value->Tag = ValueInt;

	  ASFile asFile;
	  if (Include_Find (s, &asFile, true))
	    {
	      error (ErrorError, "Cannot access file \"%s\"", s);
	      value->Data.Int.i = 0;
	    }
	  else
	    {
	      if (asFile.size > UINT32_MAX)
		{
		  error (ErrorWarning, "File size of \"%s\" is bigger than unsigned 32-bit integer", s);
		  asFile.size = UINT32_MAX;
		}
	      value->Data.Int.i = asFile.size;
	    }
	  ASFile_Free (&asFile);
	  free (s);
	  break;
	}

      case eOp_FAttr:
	{
	  if (value->Tag != ValueString)
	    return false;
	  char *s;
	  if ((s = strndup (value->Data.String.s, value->Data.String.len)) == NULL)
	    errorOutOfMem();

	  value->Tag = ValueInt;

	  ASFile asFile;
	  if (Include_Find (s, &asFile, true))
	    {
	      error (ErrorError, "Cannot access file \"%s\"", s);
	      value->Data.Int.i = 0;
	    }
	  else
	    value->Data.Int.i = asFile.attribs;
	  ASFile_Free (&asFile);
	  free (s);
	  break;
	}

      case eOp_LNot: /* :LNOT: ! */
	{
	  if (value->Tag != ValueBool)
	    {
	      if (gPhase != ePassOne)
		error (ErrorError, "Bad operand type for %s", ":LNOT:");
	      return false;
	    }
	  value->Data.Bool.b = !value->Data.Bool.b;
	  break;
	}

      case eOp_Not: /* :NOT: ~ */
	{
	  uint32_t i;
	  if (!GetInt (value, &i))
	    {
	      if (gPhase != ePassOne)
		error (ErrorError, "Bad operand type for %s", ":NOT:");
	      return false;
	    }
	  *value = Value_Int (~i, eIntType_PureInt);
	  break;
	}

      case eOp_Neg: /* - */
	{
	  if (!Eval_NegValue (value))
	    {
	      if (gPhase != ePassOne)
		error (ErrorError, "Bad operand type for %s", "negation");
	      return false;
	    }
	  break;
	}

      case eOp_None: /* */
	break;

      case eOp_Base: /* :BASE: */
	{
	  switch (value->Tag)
	    {
	      case ValueSymbol:
		{
		  if (value->Data.Symbol.symbol == areaCurrentSymbol
		      && value->Data.Symbol.factor == 1)
		    *value = Value_Int (15, eIntType_PureInt);
		  else
		    {
		      if (gPhase != ePassOne)
			error (ErrorError, "Not current area symbol");
		      return false;
		    }
		  break;
		}

	      case ValueAddr:
		{
		  *value = Value_Int (value->Data.Addr.r, eIntType_PureInt);
		  break;
		}

	      default:
		if (gPhase != ePassOne)
		  error (ErrorError, "Bad operand type for %s", ":BASE:");
		return false;
	    }
	  break;
	}

      case eOp_Index: /* :INDEX: */
	{
	  switch (value->Tag)
	    {
	      case ValueInt:
		break;

	      case ValueAddr:
		*value = Value_Int (value->Data.Addr.i, eIntType_PureInt);
		break;

	      case ValueSymbol:
		{
		  if (value->Data.Symbol.symbol == areaCurrentSymbol
		      && value->Data.Symbol.factor == 1)
		    *value = Value_Int (value->Data.Symbol.offset, eIntType_PureInt);
		  else
		    {
		      if (gPhase != ePassOne)
			error (ErrorError, "Not current area symbol");
		      return false;
		    }
		  break;
		}

	      default:
		if (gPhase != ePassOne)
		  error (ErrorError, "Bad operand type for %s", ":INDEX:");
		return false;
	    }
	  break;
	}

      case eOp_Len: /* :LEN: */
	{
	  if (value->Tag != ValueString)
	    {
	      if (gPhase != ePassOne)
		error (ErrorError, "Bad operand type for %s", ":LEN:");
	      return false;
	    }
	  value->Tag = ValueInt; /* ValueString.len at same place as ValueInt.i.  */
	  break;
	}

      case eOp_Str: /* :STR: */
	{
	  if (value->Tag == ValueBool)
	    {
	      char *c;
	      if ((c = malloc (1)) == NULL)
		errorOutOfMem ();
	      *c = value->Data.Bool.b ? 'T' : 'F';
	      value->Tag = ValueString;
	      value->Data.String.s = c;
	      value->Data.String.len = 1;
	    }
	  else
	    {
	      char num[32];
	      switch (value->Tag)
		{
		  case ValueInt:
		    sprintf (num, "%.8X", value->Data.Int.i);
		    break;
		  case ValueFloat:
		    sprintf (num, "%f", value->Data.Float.f);
		    break;
		  default:
		    if (gPhase != ePassOne)
		      error (ErrorError, "Bad operand type for %s", ":STR:");
		    return false;
		}
	      size_t len = strlen (num);
	      char *c;
	      if ((c = malloc (len)) == NULL)
		errorOutOfMem();
	      memcpy (c, num, len);
	      value->Tag = ValueString;
	      value->Data.String.s = c;
	      value->Data.String.len = len;
	    }
	  break;
	}

      case eOp_Chr: /* :CHR: */
	{
	  if (value->Tag != ValueInt)
	    {
	      if (gPhase != ePassOne)
		error (ErrorError, "Bad operand type for %s", ":CHR:");
	      return false;
	    }
	  if (value->Data.Int.i < 0 || value->Data.Int.i >= 256)
	    error (ErrorWarning, "Value %d will be truncated for :CHR: use",
	           value->Data.Int.i);
	  char *c;
	  if ((c = malloc (1)) == NULL)
	    errorOutOfMem ();
	  *c = value->Data.Int.i;
	  value->Data.String.s = c;
	  value->Data.String.len = 1;
	  value->Tag = ValueString;
	  break;
	}

      case eOp_Size: /* ?<label> */
	{
	  if (value->Tag != ValueSymbol)
	    {
	      error (ErrorError, "Bad operand type for %s", "? operator");
	      return false;
	    }
	  if (value->Data.Symbol.symbol->type & SYMBOL_DEFINED)
	    {
	      value->Tag = ValueInt;
	      value->Data.Int.i = value->Data.Symbol.symbol->codeSize;
	    }
	  else if (value->Data.Symbol.symbol->type & SYMBOL_AREA)
	    {
	      value->Tag = ValueInt;
	      value->Data.Int.i = value->Data.Symbol.symbol->area.info->curIdx;
	      if (gPhase == ePassTwo
		  && value->Data.Symbol.symbol->area.info->curIdx != value->Data.Symbol.symbol->area.info->maxIdx)
		error (ErrorError, "? on area symbol which gets extended later on");
	    }
	  else
	    {
	      if (gPhase != ePassOne)
		error (ErrorError, "? is not supported for non defined labels");
	      return false;
	    }
	  break;
	}

      case eOp_LowerCase: /* :LOWERCASE: */
	{
	  if (value->Tag != ValueString)
	    return false;
	  char *str = (char *)value->Data.String.s;
	  for (size_t i = 0; i != value->Data.String.len; ++i)
	    {
	      if (isupper ((unsigned)str[i]))
		str[i] = tolower ((unsigned)str[i]);
	    }
	  break;
	}

      case eOp_UpperCase: /* :UPPERCASE: */
	{
	  if (value->Tag != ValueString)
	    return false;
	  char *str = (char *)value->Data.String.s;
	  for (size_t i = 0; i != value->Data.String.len; ++i)
	    {
	      if (islower ((unsigned)str[i]))
		str[i] = toupper ((unsigned)str[i]);
	    }
	  break;
	}

      case eOp_RevCC: /* :REVERSE_CC: */
	{
	  if (value->Tag != ValueString)
	    return false;
	  int ccode = GetCCode (value->Data.String.s, value->Data.String.len);
	  if (ccode < 0)
	    {
	      error (ErrorError, "Condition code %.*s is not known",
	             (int)value->Data.String.len, value->Data.String.s);
	      return false;
	    }
	  int revCCode = ccode ^ 1;
	  int isUpcase = (value->Data.String.len ? (value->Data.String.s[0] & 0x20) : 0) ^ 0x20;
	  char *str;
	  if (value->Data.String.len != 2)
	    {
	      free ((void *)value->Data.String.s);
	      if ((value->Data.String.s = malloc (2)) == NULL)
		errorOutOfMem ();
	      value->Data.String.len = 2;
	    }
	  str = (char *)value->Data.String.s;
	  str[0] = oLowcaseCCodes[revCCode*2 + 0] ^ isUpcase;
	  str[1] = oLowcaseCCodes[revCCode*2 + 1] ^ isUpcase;
	  break;
	}

      case eOp_CCEnc: /* :CC_ENCODING: */
	{
	  if (value->Tag != ValueString)
	    return false;
	  int ccode = GetCCode (value->Data.String.s, value->Data.String.len);
	  if (ccode < 0)
	    {
	      error (ErrorError, "Condition code %.*s is not known",
	             (int)value->Data.String.len, value->Data.String.s);
	      return false;
	    }
	  if (ccode >= 16)
	    ccode += -16 + 2;
	  const Value newValue = Value_Int (ccode, eIntType_PureInt);
	  Value_Assign (value, &newValue);
	  break;
	}

      case eOp_RConst: /* :RCONST: */
	{
	  if (value->Tag != ValueInt && value->Data.Int.type == eIntType_PureInt)
	    return false;
	  value->Data.Int.type = eIntType_PureInt;
	  break;
	}

      default:
	assert (0);
	return false;
    }

  return true;
}
