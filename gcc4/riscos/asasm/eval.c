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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "error.h"
#include "eval.h"
#include "global.h"
#include "help_eval.h"
#include "include.h"
#include "main.h"
#include "os.h"

/* No validation checking on value types! */
static int
ememcmp (Value * lv, const Value * rv)
{
  const int lvl = lv->ValueString.len;
  const int rvl = rv->ValueString.len;
  int a = memcmp (lv->ValueString.s, rv->ValueString.s, lvl < rvl ? lvl : rvl);
  return a ? a : lvl - rvl;
}

#define STRINGIFY(OP)	#OP

#define COMPARE(OP) \
  do \
    { \
      if (lvalue->Tag == ValueFloat && rvalue->Tag == ValueFloat) \
        lvalue->ValueBool.b = lvalue->ValueFloat.f OP rvalue->ValueFloat.f; \
      else if (lvalue->Tag == ValueString && rvalue->Tag == ValueString) \
        lvalue->ValueBool.b = ememcmp(lvalue, rvalue) OP 0; \
      else if ((lvalue->Tag & (ValueInt | ValueAddr | ValueLateLabel)) \
               || (rvalue->Tag & (ValueInt | ValueAddr | ValueLateLabel))) \
        { \
          help_evalSubLate(lvalue, rvalue); \
          /* Might not be a ValueInt, but ValueLate* has i at the same place */ \
          if (!(lvalue->Tag & (ValueInt | ValueAddr))) \
            return false; \
	  /* Comparing of integers happens *unsigned* ! */ \
          lvalue->ValueBool.b = (uint32_t)lvalue->ValueInt.i OP (uint32_t)rvalue->ValueInt.i; \
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
      if ((lvalue->Tag == ValueAddr && rvalue->Tag == ValueInt)
	  || (lvalue->Tag == ValueInt && rvalue->Tag == ValueAddr))
	lvalue->ValueInt.i *= rvalue->ValueInt.i;
      else if (lvalue->Tag == rvalue->Tag
	       && lvalue->Tag == ValueInt)
	lvalue->ValueInt.i *= rvalue->ValueInt.i;
      else if (lvalue->Tag == rvalue->Tag
	       && lvalue->Tag == ValueFloat)
	lvalue->ValueFloat.f *= rvalue->ValueFloat.f;
      else
	{
	  error (ErrorError, "Bad operand type for multiplication");
	  return false;
	}
      break;

    case Op_div: /* / */
      if (lvalue->Tag == rvalue->Tag && lvalue->Tag == ValueInt)
	{
	  if (rvalue->ValueInt.i == 0)
	    {
	      error (ErrorError, "Division by zero");
	      return false;
	    }
	  /* Division is *unsigned*.  */
	  lvalue->ValueInt.i = (unsigned)lvalue->ValueInt.i / (unsigned)rvalue->ValueInt.i;
	}
      else if (lvalue->Tag == rvalue->Tag && lvalue->Tag == ValueFloat)
	{
	  if (rvalue->ValueFloat.f == 0.)
	    {
	      error (ErrorError, "Division by zero");
	      return false;
	    }
	  lvalue->ValueFloat.f /= rvalue->ValueFloat.f;
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
	  if (rvalue->ValueInt.i == 0)
	    {
	      error (ErrorError, "Division by zero");
	      return false;
	    }
	  /* Modulo is *unsigned*.  */
	  lvalue->ValueInt.i = (unsigned)lvalue->ValueInt.i % (unsigned)rvalue->ValueInt.i;
	}
      else
	{
	  error (ErrorError, "Bad operand type for modulo");
	  return false;
	}
      break;

    case Op_add: /* + */
      /* Note that <addr> + <addr> is not supported (don't think that makes
         sense).  */
      if (lvalue->Tag == ValueFloat && rvalue->Tag == ValueFloat)
	lvalue->ValueFloat.f += rvalue->ValueFloat.f; /* <float> = <float> + <float> */
      else if (lvalue->Tag == ValueFloat && rvalue->Tag == ValueInt)
	lvalue->ValueFloat.f += rvalue->ValueInt.i; /* <float> = <float> + <signed int> */
      else if (lvalue->Tag == ValueInt && rvalue->Tag == ValueFloat)
	{ /* <float> = <signed int> + <float> */
	  int val = lvalue->ValueInt.i;
	  lvalue->ValueFloat.f = rvalue->ValueFloat.f + val;
	  lvalue->Tag = ValueFloat;
	}
      else if (lvalue->Tag == ValueAddr && rvalue->Tag == ValueInt)
	lvalue->ValueAddr.i += rvalue->ValueInt.i; /* <addr> = <addr> + <int> */
      else if (lvalue->Tag == ValueInt && rvalue->Tag == ValueAddr)
	{ /* <addr> = <int> + <addr> */
	  /* ValueAddr and ValueInt have i at the same place.  */
	  lvalue->ValueAddr.i += rvalue->ValueAddr.i;
	  lvalue->Tag = ValueAddr;
	}
      else if ((lvalue->Tag & (ValueInt | ValueLateLabel))
	       && (rvalue->Tag & (ValueInt | ValueLateLabel)))
	{
	  help_evalAddLate (lvalue, rvalue);
	  /* Might not be a ValueInt, but ValueLateLabel has i at the same place.  */
	  lvalue->ValueInt.i += rvalue->ValueInt.i;
	}
      else
	{
	  error (ErrorError, "Bad operand type for addition");
	  return false;
	}
      break;

    case Op_sub: /* - */
      /* Note that <int> - <addr> is not supported (don't think that makes
         sense).  */
      if (lvalue->Tag == ValueFloat && rvalue->Tag == ValueFloat)
	lvalue->ValueFloat.f -= rvalue->ValueFloat.f; /* <float> = <float> - <float> */
      else if (lvalue->Tag == ValueFloat && rvalue->Tag == ValueInt)
	lvalue->ValueFloat.f -= rvalue->ValueInt.i; /* <float> = <float> - <signed int> */
      else if (lvalue->Tag == ValueInt && rvalue->Tag == ValueFloat)
	{ /* <float> = <signed int> - <float> */
	  int val = lvalue->ValueInt.i;
	  lvalue->ValueFloat.f = val - rvalue->ValueFloat.f;
	  lvalue->Tag = ValueFloat;
	}
      else if (lvalue->Tag == ValueAddr && rvalue->Tag == ValueAddr)
	{ /* <int> = <addr> - <addr> */
	  if (lvalue->ValueAddr.r != rvalue->ValueAddr.r)
	    {
	      error (ErrorError, "Base registers are different in subtraction ([r%d, #x] - [r%d, #y])",
	             lvalue->ValueAddr.r, rvalue->ValueAddr.r);
	      return false;
	    }
	  /* ValueAddr.i is at the same place as ValueInt.i.  */
	  lvalue->ValueAddr.i -= rvalue->ValueAddr.i;
	  lvalue->Tag = ValueInt;
	}
      else if (lvalue->Tag == ValueAddr && (rvalue->Tag & (ValueInt | ValueAddr)))
	lvalue->ValueAddr.i -= rvalue->ValueInt.i; /* <addr> = <addr> - <int> */
      else if ((lvalue->Tag & (ValueInt | ValueLateLabel))
	       && (rvalue->Tag & (ValueInt | ValueLateLabel)))
	{
	  help_evalSubLate (lvalue, rvalue);
	  /* Might not be a ValueInt, but ValueLateLabel has i at the same place.  */
	  lvalue->ValueInt.i -= rvalue->ValueInt.i;
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
	if ((c = malloc (lvalue->ValueString.len + rvalue->ValueString.len)) == NULL)
	  errorOutOfMem();
	memcpy (c, lvalue->ValueString.s, lvalue->ValueString.len);
	memcpy (c + lvalue->ValueString.len,
		rvalue->ValueString.s, rvalue->ValueString.len);
	lvalue->ValueString.s = c;
	lvalue->ValueString.len += rvalue->ValueString.len;
      }
      break;

    case Op_and: /* :AND: & */
      if ((lvalue->Tag == ValueAddr && rvalue->Tag == ValueInt)
	  || (lvalue->Tag == ValueInt && rvalue->Tag == ValueAddr)
	  || (lvalue->Tag == ValueInt && rvalue->Tag == ValueInt))
	{
	  /* ValueInt.i and ValueAddr.i are at the same place.  */
	  lvalue->ValueInt.i &= rvalue->ValueInt.i;
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
      lvalue->ValueInt.i |= rvalue->ValueInt.i;
      break;

    case Op_xor: /* :EOR: ^ */
      if (lvalue->Tag != ValueInt || rvalue->Tag != ValueInt)
	{
	  error (ErrorError, "Bad operand type for :EOR:");
	  return false;
	}
      lvalue->ValueInt.i ^= rvalue->ValueInt.i;
      break;

    case Op_asr: /* >>> */
      {
        if (lvalue->Tag != ValueInt || rvalue->Tag != ValueInt)
	  {
	    error (ErrorError, "Bad operand type for >>>");
	    return false;
	  }
        unsigned numbits = (unsigned)rvalue->ValueInt.i >= 32 ? 1 : 32 - (unsigned)rvalue->ValueInt.i;
        unsigned mask = 1U << (numbits - 1);
        ARMWord nosign = ((ARMWord) lvalue->ValueInt.i) >> (32 - numbits);
        lvalue->ValueInt.i = (nosign ^ mask) - mask;
      }
      break;

    case Op_sr: /* >> :SHR: */
      if (lvalue->Tag != ValueInt || rvalue->Tag != ValueInt)
	{
	  error (ErrorError, "Bad operand type for >> or :SHR:");
	  return false;
	}
      lvalue->ValueInt.i = (unsigned)rvalue->ValueInt.i >= 32 ? 0 : ((ARMWord) lvalue->ValueInt.i) >> rvalue->ValueInt.i;
      break;

    case Op_sl: /* << :SHL: */
      if (lvalue->Tag != ValueInt || rvalue->Tag != ValueInt)
	{
	  error (ErrorError, "Bad operand type for << or :SHR:");
	  return false;
	}
      lvalue->ValueInt.i = (unsigned)rvalue->ValueInt.i >= 32 ? 0 : ((ARMWord) lvalue->ValueInt.i) << rvalue->ValueInt.i;
      break;

    case Op_ror: /* :ROR: */
      {
        if (lvalue->Tag != ValueInt || rvalue->Tag != ValueInt)
	  {
	    error (ErrorError, "Bad operand type for :ROR:");
	    return false;
	  }
	unsigned numbits = rvalue->ValueInt.i & 31;
        lvalue->ValueInt.i = (((ARMWord) lvalue->ValueInt.i) >> numbits)
			       | (lvalue->ValueInt.i << (32 - numbits));
      }
      break;

    case Op_rol: /* :ROL: */
      {
        if (lvalue->Tag != ValueInt || rvalue->Tag != ValueInt)
	  {
	    error (ErrorError, "Bad operand type for :ROL:");
	    return false;
	  }
	unsigned numbits = rvalue->ValueInt.i & 31;
        lvalue->ValueInt.i = (lvalue->ValueInt.i << numbits)
				| (((ARMWord) lvalue->ValueInt.i) >> (32 - numbits));
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
	lvalue->ValueBool.b = lvalue->ValueBool.b == rvalue->ValueBool.b;
      else
        COMPARE (==);
      break;

    case Op_ne: /* <> /= != :LEOR: */
      if (lvalue->Tag == ValueBool && rvalue->Tag == ValueBool)
	lvalue->ValueBool.b = lvalue->ValueBool.b != rvalue->ValueBool.b;
      else
        COMPARE (!=);
      break;

    case Op_land: /* :LAND: && */
      if (lvalue->Tag != ValueBool || rvalue->Tag != ValueBool)
	{
	  error (ErrorError, "Bad operand type for :LAND:");
	  return false;
	}
      lvalue->ValueBool.b = lvalue->ValueBool.b && rvalue->ValueBool.b;
      break;

    case Op_lor: /* :LOR: || */
      if (lvalue->Tag != ValueBool || rvalue->Tag != ValueBool)
	{
	  error (ErrorError, "Bad operand type for :LOR:");
	  return false;
	}
      lvalue->ValueBool.b = lvalue->ValueBool.b || rvalue->ValueBool.b;
      break;

    case Op_left: /* :LEFT: */
      if (lvalue->Tag != ValueString || rvalue->Tag != ValueInt)
	{
	  error (ErrorError, "Bad operand type for :LEFT:");
	  return false;
	}
      if (rvalue->ValueInt.i < 0 || rvalue->ValueInt.i > lvalue->ValueString.len)
	{
	  error (ErrorError, "Wrong number of characters (%d) specified for :LEFT:",
	         rvalue->ValueInt.i);
	  return false;
	}
      lvalue->ValueString.len = rvalue->ValueInt.i;
      break;

    case Op_right: /* :RIGHT: */
      {
        if (lvalue->Tag != ValueString || rvalue->Tag != ValueInt)
	  {
	    error (ErrorError, "Bad operand type for :RIGHT:");
	    return false;
	  }
        if (rvalue->ValueInt.i < 0 || rvalue->ValueInt.i > lvalue->ValueString.len)
	  {
	    error (ErrorError, "Wrong number of characters (%d) specified for :RIGHT:",
	           rvalue->ValueInt.i);
	    return false;
	  }
	char *c;
	if ((c = malloc (rvalue->ValueInt.i)) == NULL)
	  errorOutOfMem ();
	memcpy (c,
	        lvalue->ValueString.s + lvalue->ValueString.len - rvalue->ValueInt.i,
	        rvalue->ValueInt.i);
        lvalue->ValueString.s = c;
        lvalue->ValueString.len = rvalue->ValueInt.i;
      }
      break;

    default:
      error (ErrorError, "Illegal binary operator");
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
	value->ValueInt.i = 0;
	value->Tag = ValueInt;
	break;
      case Op_fload:
	if (value->Tag != ValueString)
	  return false;
	/* TODO: Real load address. For now, type everything as text */
	value->ValueInt.i = 0xFFFfff00;
	value->Tag = ValueInt;
	break;
      case Op_fsize:
	{
	  if (value->Tag != ValueString)
	    return false;
	  char *s;
	  if ((s = strndup(value->ValueString.s, value->ValueString.len)) == NULL)
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
	  value->ValueInt.i = (int) ftell (fp);
	  if (value->ValueInt.i == -1)
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
	value->ValueBool.b = !value->ValueBool.b;
	break;

      case Op_not: /* :NOT: ~ */
	if (value->Tag != ValueInt)
	  {
	    error (ErrorError, "Bad operand type for :NOT:");
	    return false;
	  }
	value->ValueInt.i = ~value->ValueInt.i;
	break;

      case Op_neg: /* - */
	if (value->Tag == ValueFloat)
	  value->ValueFloat.f = -value->ValueFloat.f;
	else if (value->Tag & (ValueInt | ValueLateLabel))
	  {
	    help_evalNegLate (value);
	    value->ValueInt.i = -value->ValueInt.i;
	  }
	else
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
	value->ValueInt.i = value->ValueAddr.r;
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
	    *c = value->ValueBool.b ? 'T' : 'F';
	    value->Tag = ValueString;
            value->ValueString.s = c;
	    value->ValueString.len = 1;
	  }
	else
	  {
	    char num[32];
	    switch (value->Tag)
	      {
	        case ValueInt:
		  sprintf (num, "%.8X", value->ValueInt.i);
		  break;
	        case ValueFloat:
		  sprintf (num, "%f", value->ValueFloat.f);
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
	    value->ValueString.s = c;
	    value->ValueString.len = len;
	  }
	break;

      case Op_chr: /* :CHR: */
	{
	  if (value->Tag != ValueInt)
	    {
	      error (ErrorError, "Bad operand type for :CHR:");
	      return false;
	    }
	  if ((value->ValueInt.i < 0 || value->ValueInt.i >= 256) && option_pedantic)
	    error (ErrorWarning, "Value %d will be truncated for :CHR: use",
	           value->ValueInt.i);
	  char *c;
	  if ((c = malloc (1)) == NULL)
	    errorOutOfMem ();
	  *c = value->ValueInt.i;
	  value->ValueString.s = c;
	  value->ValueString.len = 1;
	  value->Tag = ValueString;
	}
	break;

      default:
	errorAbort ("Internal evalUnop: illegal fall through");
	return false;
    }

  return true;
}
