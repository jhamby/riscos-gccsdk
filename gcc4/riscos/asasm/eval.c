/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2002-2010 GCCSDK Developers
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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "code.h"
#include "error.h"
#include "eval.h"
#include "global.h"
#include "include.h"
#include "main.h"
#include "os.h"

/* No validation checking on value types! */
static int
ememcmp (const Value *lv, const Value *rv)
{
  const int lvl = lv->Data.String.len;
  const int rvl = rv->Data.String.len;
  int a = memcmp (lv->Data.String.s, rv->Data.String.s, lvl < rvl ? lvl : rvl);
  return a ? a : lvl - rvl;
}

#define STRINGIFY(OP)	#OP
/* Core implementation for '<', '<=', '>', '>=', '==' and '!='.
   Works for ValueFloat, ValueString, ValueInt, ValueAdd.  */
#define COMPARE(OP) \
  do \
    { \
      if (lvalue->Tag == ValueFloat && rvalue->Tag == ValueFloat) \
        lvalue->Data.Bool.b = lvalue->Data.Float.f OP rvalue->Data.Float.f; \
      else if (lvalue->Tag == ValueString && rvalue->Tag == ValueString) \
        lvalue->Data.Bool.b = ememcmp (lvalue, rvalue) OP 0; \
      else if (lvalue->Tag == ValueInt && rvalue->Tag == ValueInt) \
        { \
	  /* Comparing integers happens *unsigned* ! */ \
          lvalue->Data.Bool.b = (uint32_t)lvalue->Data.Int.i OP (uint32_t)rvalue->Data.Int.i; \
        } \
      else if (lvalue->Tag == ValueAddr && rvalue->Tag == ValueAddr) \
        { \
	  /* First compare on base register, then its index.  */ \
	  if (lvalue->Data.Addr.r != rvalue->Data.Addr.r) \
	    lvalue->Data.Bool.b = lvalue->Data.Addr.r OP rvalue->Data.Addr.r; \
	  else \
	    lvalue->Data.Bool.b = lvalue->Data.Addr.i OP rvalue->Data.Addr.i; \
        } \
      else \
        { \
	  error (ErrorError, "Bad operand types for " STRINGIFY(OP)); \
          return false; \
        } \
      lvalue->Tag = ValueBool; \
    } while (0)
		   
bool
evalBinop (Operator op, Value *lvalue, const Value *rvalue)
{
  switch (op)
    {
    case Op_mul: /* * */
      /* FIXME: ValueAddr * ValueAddr does not make sense.  */
      /* FIXME: <int> * <float> -> <float> */
      if ((lvalue->Tag == ValueAddr && rvalue->Tag == ValueInt)
	  || (lvalue->Tag == ValueInt && rvalue->Tag == ValueAddr))
	lvalue->Data.Int.i *= rvalue->Data.Int.i;
      else if (lvalue->Tag == rvalue->Tag
	       && lvalue->Tag == ValueInt)
	lvalue->Data.Int.i *= rvalue->Data.Int.i;
      else if (lvalue->Tag == rvalue->Tag
	       && lvalue->Tag == ValueFloat)
	lvalue->Data.Float.f *= rvalue->Data.Float.f;
      else
	{
	  error (ErrorError, "Bad operand type for multiplication");
	  return false;
	}
      break;

    case Op_div: /* / */
      if (lvalue->Tag == rvalue->Tag && lvalue->Tag == ValueInt)
	{
	  if (rvalue->Data.Int.i == 0)
	    {
	      error (ErrorError, "Division by zero");
	      return false;
	    }
	  /* Division is *unsigned*.  */
	  lvalue->Data.Int.i = (unsigned)lvalue->Data.Int.i / (unsigned)rvalue->Data.Int.i;
	}
      else if (lvalue->Tag == rvalue->Tag && lvalue->Tag == ValueFloat)
	{
	  if (rvalue->Data.Float.f == 0.)
	    {
	      error (ErrorError, "Division by zero");
	      return false;
	    }
	  lvalue->Data.Float.f /= rvalue->Data.Float.f;
	}
      else
	{
	  error (ErrorError, "Bad operand type for division");
	  return false;
	}
      break;

    case Op_mod: /* :MOD: */
      if (lvalue->Tag == ValueInt && rvalue->Tag == ValueInt)
	{
	  if (rvalue->Data.Int.i == 0)
	    {
	      error (ErrorError, "Division by zero");
	      return false;
	    }
	  /* Modulo is *unsigned*.  */
	  lvalue->Data.Int.i = (unsigned)lvalue->Data.Int.i % (unsigned)rvalue->Data.Int.i;
	}
      else
	{
	  error (ErrorError, "Bad operand type for modulo");
	  return false;
	}
      break;

    case Op_add: /* + */
	{
	  Value rhs;
	  /* Promotion for ValueFloat and ValueAddr.  */
	  if ((rvalue->Tag == ValueFloat && lvalue->Tag != ValueFloat)
	      || (rvalue->Tag == ValueAddr && lvalue->Tag != ValueAddr))
	    {
	      rhs = *lvalue;
	      *lvalue = *rvalue;
	    }
	  else
	    rhs = *rvalue;

	  if (lvalue->Tag == ValueFloat && rhs.Tag == ValueFloat)
	    lvalue->Data.Float.f += rhs.Data.Float.f; /* <float> + <float> -> <float> */
	  else if (lvalue->Tag == ValueFloat && rhs.Tag == ValueInt)
	    lvalue->Data.Float.f += rhs.Data.Int.i; /* <float> + <signed int> -> <float> */
	  else if (lvalue->Tag == ValueAddr && rhs.Tag == ValueAddr)
	    {
	      if (lvalue->Data.Addr.r != rhs.Data.Addr.r)
		{
		  error (ErrorError, "Base registers are different in addition ([r%d, #x] + [r%d, #y])",
		         lvalue->Data.Addr.r, rvalue->Data.Addr.r);
		  return false;
		}
	      /* <addr> + <addr> (same base reg) -> <addr> */
	      /* FIXME: this is not consistent with Op_sub.  */
	      lvalue->Data.Addr.i += rhs.Data.Addr.i;
	    }
	  else if (lvalue->Tag == ValueAddr && rhs.Tag == ValueInt)
	    lvalue->Data.Addr.i += rhs.Data.Int.i; /* <addr> + <int> -> <addr> */
	  else if (lvalue->Tag == ValueInt && rhs.Tag == ValueInt)
	    lvalue->Data.Int.i += rhs.Data.Int.i; /* <int> + <int> -> <int> */
	  else
	    {
	      error (ErrorError, "Bad operand type for addition");
	      return false;
	    }
	}
      break;

    case Op_sub: /* - */
      if (lvalue->Tag == ValueFloat && rvalue->Tag == ValueFloat)
	lvalue->Data.Float.f -= rvalue->Data.Float.f; /* <float> - <float> -> <float> */
      else if (lvalue->Tag == ValueFloat && rvalue->Tag == ValueInt)
	lvalue->Data.Float.f -= rvalue->Data.Int.i; /* <float> - <signed int> -> <float> */
      else if (lvalue->Tag == ValueInt && rvalue->Tag == ValueFloat)
	{ /* <signed int> - <float> -> <float> */
	  int val = lvalue->Data.Int.i;
	  lvalue->Data.Float.f = val - rvalue->Data.Float.f;
	  lvalue->Tag = ValueFloat;
	}
      else if (lvalue->Tag == ValueInt && rvalue->Tag == ValueInt)
	lvalue->Data.Int.i -= rvalue->Data.Int.i; /* <int> - <int> -> <int> */
      else if (lvalue->Tag == ValueAddr && rvalue->Tag == ValueAddr)
	{ /* <addr> - <addr> -> <int> */
	  if (lvalue->Data.Addr.r != rvalue->Data.Addr.r)
	    {
	      error (ErrorError, "Base registers are different in subtraction ([r%d, #x] - [r%d, #y])",
	             lvalue->Data.Addr.r, rvalue->Data.Addr.r);
	      return false;
	    }
	  /* ValueAddr.i is at the same place as ValueInt.i.  */
	  lvalue->Data.Int.i -= rvalue->Data.Addr.i;
	  lvalue->Tag = ValueInt;
	}
      else if (lvalue->Tag == ValueAddr && rvalue->Tag == ValueInt)
	lvalue->Data.Addr.i -= rvalue->Data.Int.i; /* <addr> - <int> -> <addr> */
      else if (lvalue->Tag == ValueInt && rvalue->Tag == ValueAddr)
	{ /* <int> - <addr> -> <addr> */
	  int val = lvalue->Data.Int.i;
	  lvalue->Tag = ValueAddr;
	  lvalue->Data.Addr.i = val - rvalue->Data.Addr.i;
	  lvalue->Data.Addr.r = rvalue->Data.Addr.r;
	}
      else
	{
	  error (ErrorError, "Bad operand type for subtraction");
	  return false;
	}
      break;

    case Op_concat: /* :CC: */
      {
        if (lvalue->Tag != ValueString || rvalue->Tag != ValueString)
	  {
	    error (ErrorError, "Bad operand type for :CC:");
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
      }
      break;

    case Op_and: /* :AND: & */
      if ((lvalue->Tag == ValueAddr && rvalue->Tag == ValueInt)
	  || (lvalue->Tag == ValueInt && rvalue->Tag == ValueAddr)
	  || (lvalue->Tag == ValueInt && rvalue->Tag == ValueInt))
	{
	  /* ValueInt.i and ValueAddr.i are at the same place.  */
	  lvalue->Data.Int.i &= rvalue->Data.Int.i;
	  lvalue->Tag = ValueInt;
	}
      else
	{
	  error (ErrorError, "Bad operand type for :AND:");
	  return false;
	}
      break;

    case Op_or: /* :OR: | */
      if (lvalue->Tag != ValueInt || rvalue->Tag != ValueInt)
	{
	  error (ErrorError, "Bad operand type for :OR:");
	  return false;
	}
      lvalue->Data.Int.i |= rvalue->Data.Int.i;
      break;

    case Op_xor: /* :EOR: ^ */
      if (lvalue->Tag != ValueInt || rvalue->Tag != ValueInt)
	{
	  error (ErrorError, "Bad operand type for :EOR:");
	  return false;
	}
      lvalue->Data.Int.i ^= rvalue->Data.Int.i;
      break;

    case Op_asr: /* >>> */
      {
        if (lvalue->Tag != ValueInt || rvalue->Tag != ValueInt)
	  {
	    error (ErrorError, "Bad operand type for >>>");
	    return false;
	  }
        unsigned numbits = (unsigned)rvalue->Data.Int.i >= 32 ? 1 : 32 - (unsigned)rvalue->Data.Int.i;
        unsigned mask = 1U << (numbits - 1);
        ARMWord nosign = ((ARMWord) lvalue->Data.Int.i) >> (32 - numbits);
        lvalue->Data.Int.i = (nosign ^ mask) - mask;
      }
      break;

    case Op_sr: /* >> :SHR: */
      if (lvalue->Tag != ValueInt || rvalue->Tag != ValueInt)
	{
	  error (ErrorError, "Bad operand type for >> or :SHR:");
	  return false;
	}
      lvalue->Data.Int.i = (unsigned)rvalue->Data.Int.i >= 32 ? 0 : ((ARMWord) lvalue->Data.Int.i) >> rvalue->Data.Int.i;
      break;

    case Op_sl: /* << :SHL: */
      if (lvalue->Tag != ValueInt || rvalue->Tag != ValueInt)
	{
	  error (ErrorError, "Bad operand type for << or :SHR:");
	  return false;
	}
      lvalue->Data.Int.i = (unsigned)rvalue->Data.Int.i >= 32 ? 0 : ((ARMWord) lvalue->Data.Int.i) << rvalue->Data.Int.i;
      break;

    case Op_ror: /* :ROR: */
      {
        if (lvalue->Tag != ValueInt || rvalue->Tag != ValueInt)
	  {
	    error (ErrorError, "Bad operand type for :ROR:");
	    return false;
	  }
	unsigned numbits = rvalue->Data.Int.i & 31;
        lvalue->Data.Int.i = (((ARMWord) lvalue->Data.Int.i) >> numbits)
			       | (lvalue->Data.Int.i << (32 - numbits));
      }
      break;

    case Op_rol: /* :ROL: */
      {
        if (lvalue->Tag != ValueInt || rvalue->Tag != ValueInt)
	  {
	    error (ErrorError, "Bad operand type for :ROL:");
	    return false;
	  }
	unsigned numbits = rvalue->Data.Int.i & 31;
        lvalue->Data.Int.i = (lvalue->Data.Int.i << numbits)
				| (((ARMWord) lvalue->Data.Int.i) >> (32 - numbits));
      }
      break;

    case Op_le: /* <= */
      COMPARE (<=);
      break;

    case Op_ge: /* >= */
      COMPARE (>=);
      break;

    case Op_lt: /* < */
      COMPARE (<);
      break;

    case Op_gt: /* > */
      COMPARE (>);
      break;

    case Op_eq: /* = == */
      if (lvalue->Tag == ValueBool && rvalue->Tag == ValueBool)
	lvalue->Data.Bool.b = lvalue->Data.Bool.b == rvalue->Data.Bool.b;
      else
        COMPARE (==);
      break;

    case Op_ne: /* <> /= != :LEOR: */
      if (lvalue->Tag == ValueBool && rvalue->Tag == ValueBool)
	lvalue->Data.Bool.b = lvalue->Data.Bool.b != rvalue->Data.Bool.b;
      else
        COMPARE (!=);
      break;

    case Op_land: /* :LAND: && */
      if (lvalue->Tag != ValueBool || rvalue->Tag != ValueBool)
	{
	  error (ErrorError, "Bad operand type for :LAND:");
	  return false;
	}
      lvalue->Data.Bool.b = lvalue->Data.Bool.b && rvalue->Data.Bool.b;
      break;

    case Op_lor: /* :LOR: || */
      if (lvalue->Tag != ValueBool || rvalue->Tag != ValueBool)
	{
	  error (ErrorError, "Bad operand type for :LOR:");
	  return false;
	}
      lvalue->Data.Bool.b = lvalue->Data.Bool.b || rvalue->Data.Bool.b;
      break;

    case Op_left: /* :LEFT: */
      if (lvalue->Tag != ValueString || rvalue->Tag != ValueInt)
	{
	  error (ErrorError, "Bad operand type for :LEFT:");
	  return false;
	}
      if (rvalue->Data.Int.i < 0 || (size_t)rvalue->Data.Int.i > lvalue->Data.String.len)
	{
	  error (ErrorError, "Wrong number of characters (%d) specified for :LEFT:",
	         rvalue->Data.Int.i);
	  return false;
	}
      lvalue->Data.String.len = rvalue->Data.Int.i;
      break;

    case Op_right: /* :RIGHT: */
      {
        if (lvalue->Tag != ValueString || rvalue->Tag != ValueInt)
	  {
	    error (ErrorError, "Bad operand type for :RIGHT:");
	    return false;
	  }
        if (rvalue->Data.Int.i < 0 || (size_t)rvalue->Data.Int.i > lvalue->Data.String.len)
	  {
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
        lvalue->Data.String.s = c;
        lvalue->Data.String.len = rvalue->Data.Int.i;
      }
      break;

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
      case ValueFloat:
	value->Data.Float.f = -value->Data.Float.f;
	break;
      case ValueAddr:
	value->Data.Addr.i = -value->Data.Addr.i;
	break;
      case ValueSymbol:
	value->Data.Symbol.factor = - value->Data.Symbol.factor;
	break;
      case ValueCode:
	for (size_t i = 0; i != value->Data.Code.len; ++i)
	  {
	    /* Negate only the values (not operations).  */
	    if (value->Data.Code.c[i].Tag == CodeValue)
	     {
	        if (!Eval_NegValue (&value->Data.Code.c[i].Data.value))
		  return false;
	      }
	  }
	break;
      default:
	return false;
    }
  return true;
}

bool
evalUnop (Operator op, Value *value)
{
  switch (op)
    {
      case Op_fattr:
	if (value->Tag != ValueString)
	  return false;
	error (ErrorError, "%s not implemented", "fattr");
	break;
      case Op_fexec:
	if (value->Tag != ValueString)
	  return false;
	/* TODO: Real exec address. For now, just fill with zeros */
	value->Data.Int.i = 0;
	value->Tag = ValueInt;
	break;
      case Op_fload:
	if (value->Tag != ValueString)
	  return false;
	/* TODO: Real load address. For now, type everything as text */
	value->Data.Int.i = 0xFFFfff00;
	value->Tag = ValueInt;
	break;
      case Op_fsize:
	{
	  if (value->Tag != ValueString)
	    return false;
	  char *s;
	  if ((s = strndup (value->Data.String.s, value->Data.String.len)) == NULL)
	    errorOutOfMem();
	  FILE *fp;
	  if ((fp = getInclude (s, NULL)) == NULL)
	    {
	      error (ErrorError, "Cannot open file \"%s\"", s ? s : "");
	      free (s);
	      return false;
	    }
	  if (fseek (fp, 0l, SEEK_END))
	    {
	      error (ErrorError, "Cannot seek to end of file \"%s\"", s ? s : "");
	      free (s);
	      return false;
	    }
	  value->Data.Int.i = (int) ftell (fp);
	  if (value->Data.Int.i == -1)
	    {
	      error (ErrorError, "Cannot find size of file \"%s\"", s ? s : "");
	      free (s);
	      return false;
	    }
	  fclose (fp);
	  free (s);
	  value->Tag = ValueInt;
	}
	break;

      case Op_lnot: /* :LNOT: ! */
	if (value->Tag != ValueBool)
	  {
	    error (ErrorError, "Bad operand type for :LNOT:");
	    return false;
	  }
	value->Data.Bool.b = !value->Data.Bool.b;
	break;

      case Op_not: /* :NOT: ~ */
	if (value->Tag != ValueInt)
	  {
	    error (ErrorError, "Bad operand type for :NOT:");
	    return false;
	  }
	value->Data.Int.i = ~value->Data.Int.i;
	break;

      case Op_neg: /* - */
	if (!Eval_NegValue (value))
	  {
	    error (ErrorError, "Bad operand type for negation");
	    return false;
	  }
	break;

      case Op_base: /* :BASE: */
	if (value->Tag != ValueAddr)
	  {
	    error (ErrorError, "Bad operand type for :BASE:");
	    return false;
	  }
	value->Tag = ValueInt;
	value->Data.Int.i = value->Data.Addr.r;
	break;
	
      case Op_index: /* :INDEX: */
	if (value->Tag != ValueAddr && value->Tag != ValueInt)
	  {
	    error (ErrorError, "Bad operand type for :INDEX:");
	    return false;
	  }
	value->Tag = ValueInt; /* ValueAddr.i at same place as ValueInt.i.  */
	break;

      case Op_len: /* :LEN: */
	if (value->Tag != ValueString)
	  {
	    error (ErrorError, "Bad operand type for :LEN:");
	    return false;
	  }
	value->Tag = ValueInt; /* ValueString.len at same place as ValueInt.i.  */
	break;

      case Op_str: /* :STR: */
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
	          error (ErrorError, "Bad operand type for :STR:");
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

      case Op_chr: /* :CHR: */
	{
	  if (value->Tag != ValueInt)
	    {
	      error (ErrorError, "Bad operand type for :CHR:");
	      return false;
	    }
	  if ((value->Data.Int.i < 0 || value->Data.Int.i >= 256) && option_pedantic)
	    error (ErrorWarning, "Value %d will be truncated for :CHR: use",
	           value->Data.Int.i);
	  char *c;
	  if ((c = malloc (1)) == NULL)
	    errorOutOfMem ();
	  *c = value->Data.Int.i;
	  value->Data.String.s = c;
	  value->Data.String.len = 1;
	  value->Tag = ValueString;
	}
	break;

      case Op_size: /* ?<label> */
	{
	  if (value->Tag != ValueSymbol)
	    {
	      error (ErrorError, "Bad operand type for ? operator");
	      return false;
	    }
	  if (value->Data.Symbol.symbol->type & SYMBOL_DEFINED)
	    {
	      value->Tag = ValueInt;
	      value->Data.Int.i = value->Data.Symbol.symbol->codeSize;
	    }
	  else
	    {
	      error (ErrorError, "? is not supported for non defined labels");
	      return false;
	    }
	}
	break;

      default:
	errorAbort ("Internal evalUnop: illegal fall through");
	return false;
    }

  return true;
}
