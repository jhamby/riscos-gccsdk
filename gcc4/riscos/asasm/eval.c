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
#include "os.h"

/* No validation checking on value types! */
static int
ememcmp (Value * lv, const Value * rv)
{
  const int lvl = lv->ValueString.len;
  const int rvl = rv->ValueString.len;
  int a = memcmp (lv->ValueString.s, rv->ValueString.s, lvl < rvl ? lvl : rvl);
  if (a == 0 && lvl != rvl)
    a = lvl - rvl;
  return a;
}


#define COMPARE(OP) \
  if (lvalue->Tag.t == ValueFloat && rvalue->Tag.t == ValueFloat) \
    { \
      lvalue->ValueBool.b = lvalue->ValueFloat.f OP rvalue->ValueFloat.f; \
      lvalue->Tag.t = ValueBool; \
      return true; \
    }	  \
  if (lvalue->Tag.t == ValueString && rvalue->Tag.t == ValueString) \
    { \
      lvalue->ValueBool.b = ememcmp(lvalue,rvalue) OP 0; \
      lvalue->Tag.t = ValueBool; \
      return true; \
    } \
  if (!(lvalue->Tag.t & (ValueInt | ValueAddr | ValueLateLabel)) \
      || !(rvalue->Tag.t & (ValueInt | ValueAddr | ValueLateLabel))) \
    { \
      fprintf (stderr, ": %i %i\n", lvalue->Tag.t, rvalue->Tag.t); \
      return false; \
    } \
  \
  help_evalSubLate(lvalue,rvalue); \
  /* Might not be a ValueInt, but ValueLate* has i at the same place */ \
  if (!(lvalue->Tag.t & (ValueInt | ValueAddr))) \
    return false; \
  \
  lvalue->ValueBool.b = lvalue->ValueInt.i OP rvalue->ValueInt.i; \
  lvalue->Tag.t = ValueBool; \
  return true /* Last ; is where macro is used */

bool
evalBinop (Operator op, Value * lvalue, const Value * rvalue)
{
  switch (op)
    {
    case Op_mul:
      if ((lvalue->Tag.t == ValueAddr && rvalue->Tag.t == ValueInt)
	  || (lvalue->Tag.t == ValueInt && rvalue->Tag.t == ValueAddr))
	{
	  lvalue->ValueInt.i *= rvalue->ValueInt.i;
	  return true;
	}
      if (lvalue->Tag.t != rvalue->Tag.t)
	return false;
      switch (lvalue->Tag.t)
	{
	case ValueInt:
	  lvalue->ValueInt.i *= rvalue->ValueInt.i;
	  break;
	case ValueFloat:
	  lvalue->ValueFloat.f *= rvalue->ValueFloat.f;
	  break;
	default:
	  abort (); // FIXME:
	  break;
	}
      break;

    case Op_div:
      if (lvalue->Tag.t != rvalue->Tag.t)
	return false;
      switch (lvalue->Tag.t) // FIXME: check div by 0
	{
	case ValueInt:
	  lvalue->ValueInt.i /= rvalue->ValueInt.i;
	  break;
	case ValueFloat:
	  lvalue->ValueFloat.f /= rvalue->ValueFloat.f;
	  break;
	default:
	  abort (); // FIXME:
	  break;
	}
      break;

    case Op_mod:
      if (lvalue->Tag.t != ValueInt || rvalue->Tag.t != ValueInt)
	return false;
      lvalue->ValueInt.i %= rvalue->ValueInt.i; // FIXME: check mod by 0
      break;

    case Op_add:
      if (lvalue->Tag.t == ValueFloat && rvalue->Tag.t == ValueFloat)
	{
	  lvalue->ValueFloat.f += rvalue->ValueFloat.f;
	  return true;
	}
      if (lvalue->Tag.t == ValueAddr && rvalue->Tag.t == ValueInt)
	{
	  lvalue->ValueAddr.i += rvalue->ValueInt.i;
	  return true;
	}
      if ((lvalue->Tag.t & (ValueInt | ValueLateLabel))
	  && (rvalue->Tag.t & (ValueInt | ValueLateLabel)))
	{
	  help_evalAddLate (lvalue, rvalue);
	  /* Might not be a ValueInt, but ValueLate* has i at the same place */
	  lvalue->ValueInt.i += rvalue->ValueInt.i;
	  return true;
	}
      return false;

    case Op_concat: /* :CC: */
      {
        if (lvalue->Tag.t != ValueString || rvalue->Tag.t != ValueString)
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

    case Op_sub:
      if (lvalue->Tag.t == ValueFloat && rvalue->Tag.t == ValueFloat)
	{
	  lvalue->ValueFloat.f -= rvalue->ValueFloat.f;
	  return true;
	}
      if (lvalue->Tag.t == ValueAddr && (rvalue->Tag.t & (ValueInt | ValueAddr)))
	{
	  if (rvalue->Tag.t == ValueAddr &&
	      lvalue->ValueAddr.r != rvalue->ValueAddr.r)
	    return false;
	  lvalue->ValueAddr.i -= rvalue->ValueInt.i;
	  if (rvalue->Tag.t == ValueAddr)
	    lvalue->Tag.t = ValueInt;
	  return true;
	}
      if (!(lvalue->Tag.t & (ValueInt | ValueLateLabel))
	  || !(rvalue->Tag.t & (ValueInt | ValueLateLabel)))
	return false;

      help_evalSubLate (lvalue, rvalue);
      /* Might not be a ValueInt, but ValueLate* has i at the same place */
      lvalue->ValueInt.i -= rvalue->ValueInt.i;
      break;
    case Op_and:
      if ((lvalue->Tag.t == ValueAddr && rvalue->Tag.t == ValueInt)
	  || (lvalue->Tag.t == ValueInt && rvalue->Tag.t == ValueAddr)
	  || (lvalue->Tag.t == ValueInt && rvalue->Tag.t == ValueInt))
	{
	  lvalue->ValueInt.i &= rvalue->ValueInt.i;
	  return true;
	}
      return false;
    case Op_or:
      if (lvalue->Tag.t != ValueInt || rvalue->Tag.t != ValueInt)
	return false;
      lvalue->ValueInt.i |= rvalue->ValueInt.i;
      break;
    case Op_xor:
      if (lvalue->Tag.t != ValueInt || rvalue->Tag.t != ValueInt)
	return false;
      lvalue->ValueInt.i ^= rvalue->ValueInt.i;
      break;
    case Op_asr:
      if (lvalue->Tag.t != ValueInt || rvalue->Tag.t != ValueInt)
	return false;
      lvalue->ValueInt.i = ((signed int) lvalue->ValueInt.i) >> rvalue->ValueInt.i; // FIXME: non predicatable code
      break;
    case Op_sr:
      if (lvalue->Tag.t != ValueInt || rvalue->Tag.t != ValueInt)
	return false;
      lvalue->ValueInt.i = ((ARMWord) lvalue->ValueInt.i) >> rvalue->ValueInt.i;
      break;
    case Op_sl:
      if (lvalue->Tag.t != ValueInt || rvalue->Tag.t != ValueInt)
	return false;
      lvalue->ValueInt.i <<= rvalue->ValueInt.i;
      break;
    case Op_ror:
      if (lvalue->Tag.t != ValueInt || rvalue->Tag.t != ValueInt)
	return false;
      lvalue->ValueInt.i = (((ARMWord) lvalue->ValueInt.i) >> rvalue->ValueInt.i) |
	((lvalue->ValueInt.i) << (32 - rvalue->ValueInt.i));
      break;
    case Op_rol:
      if (lvalue->Tag.t != ValueInt || rvalue->Tag.t != ValueInt)
	return false;
      lvalue->ValueInt.i = ((lvalue->ValueInt.i) << rvalue->ValueInt.i) |
	(((ARMWord) lvalue->ValueInt.i) >> (32 - rvalue->ValueInt.i));
      break;
    case Op_le:
      COMPARE (<=);
    case Op_ge:
      COMPARE (>=);
    case Op_lt:
      COMPARE (<);
    case Op_gt:
      COMPARE (>);
    case Op_eq:
      if (lvalue->Tag.t == ValueBool && rvalue->Tag.t == ValueBool)
	{
	  lvalue->ValueBool.b =
	    lvalue->ValueBool.b == rvalue->ValueBool.b;
	  return true;
	}
      COMPARE (==);
    case Op_ne:
      if (lvalue->Tag.t == ValueBool && rvalue->Tag.t == ValueBool)
	{
	  lvalue->ValueBool.b = lvalue->ValueBool.b != rvalue->ValueBool.b;
	  return true;
	}
      COMPARE (!=);
    case Op_land:
      if (lvalue->Tag.t != ValueBool || rvalue->Tag.t != ValueBool)
	return false;
      lvalue->ValueBool.b = lvalue->ValueBool.b && rvalue->ValueBool.b;
      break;
    case Op_lor:
      if (lvalue->Tag.t != ValueBool || rvalue->Tag.t != ValueBool)
	return false;
      lvalue->ValueBool.b = lvalue->ValueBool.b || rvalue->ValueBool.b;
      break;

    case Op_left: /* :LEFT: */
      if (lvalue->Tag.t != ValueString || rvalue->Tag.t != ValueInt)
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
        if (lvalue->Tag.t != ValueString || rvalue->Tag.t != ValueInt)
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
	if (value->Tag.t != ValueString)
	  return false;
	error (ErrorError, "%s not implemented", "fattr");
	break;
      case Op_fexec:
	if (value->Tag.t != ValueString)
	  return false;
	/* TODO: Real exec address. For now, just fill with zeros */
	value->ValueInt.i = 0;
	value->Tag.t = ValueInt;
	break;
      case Op_fload:
	if (value->Tag.t != ValueString)
	  return false;
	/* TODO: Real load address. For now, type everything as text */
	value->ValueInt.i = 0xFFFfff00;
	value->Tag.t = ValueInt;
	break;
      case Op_fsize:
	{
	  if (value->Tag.t != ValueString)
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
	  value->Tag.t = ValueInt;
	}
	break;
      case Op_lnot:
	if (value->Tag.t != ValueBool)
	  return false;
	value->ValueBool.b = !value->ValueBool.b;
	break;
      case Op_not:
	if (value->Tag.t != ValueInt)
	  return false;
	value->ValueInt.i = ~value->ValueInt.i;
	break;
      case Op_neg:
	if (value->Tag.t == ValueFloat)
	  value->ValueFloat.f = -value->ValueFloat.f;
	else
	  {
	    if (!(value->Tag.t & (ValueInt | ValueLateLabel)))
	      return false;
	    help_evalNegLate (value);
	    value->ValueInt.i = -value->ValueInt.i;
	  }
	break;
      case Op_index:
	if (value->Tag.t != ValueAddr && value->Tag.t != ValueInt)
	  return false;
	value->Tag.t = ValueInt;
	break;
      case Op_len:
	if (value->Tag.t != ValueString)
	  return false;
	value->Tag.t = ValueInt;
	break;

      case Op_str: /* :STR: */
	if (value->Tag.t == ValueBool)
	  {
	    char *c;
	    if ((c = malloc (1)) == NULL)
	      errorOutOfMem ();
	    *c = value->ValueBool.b ? 'T' : 'F';
	    value->Tag.t = ValueString;
            value->ValueString.s = c;
	    value->ValueString.len = 1;
	  }
	else
	  {
	    char num[32];
	    switch (value->Tag.t)
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
	    value->Tag.t = ValueString;
	    value->ValueString.s = c;
	    value->ValueString.len = len;
	  }
	break;

      case Op_chr: /* :CHR: */
	{
	  if (value->Tag.t != ValueInt)
	    {
	      error (ErrorError, "Bad operand type for :CHR:");
	      return false;
	    }
	  if (value->ValueInt.i < 0 || value->ValueInt.i >= 256)
	    error (ErrorWarning, "Value %d will be truncated for :CHR: use",
	           value->ValueInt.i);
	  char *c;
	  if ((c = malloc (1)) == NULL)
	    errorOutOfMem ();
	  *c = value->ValueInt.i;
	  value->ValueString.s = c;
	  value->ValueString.len = 1;
	  value->Tag.t = ValueString;
	}
	break;

      default:
	errorAbort ("Internal evalUnop: illegal fall through");
	return false;
    }

  return true;
}
