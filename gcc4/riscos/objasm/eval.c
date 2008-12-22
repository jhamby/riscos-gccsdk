/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas R�emo
 * Copyright (c) 2002-2008 GCCSDK Developersrs
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
      if(lvalue->Tag.t == ValueFloat && rvalue->Tag.t == ValueFloat) { \
	lvalue->ValueBool.b = lvalue->ValueFloat.f OP rvalue->ValueFloat.f; \
	lvalue->Tag.t = ValueBool; \
	return TRUE; \
      }	  \
      if(lvalue->Tag.t == ValueString && rvalue->Tag.t == ValueString) { \
	lvalue->ValueBool.b = ememcmp(lvalue,rvalue) OP 0; \
	lvalue->Tag.t = ValueBool; \
	return TRUE; \
      }	  \
      if(!(lvalue->Tag.t & (ValueInt|ValueAddr|ValueLateLabel)) || \
	 !(rvalue->Tag.t & (ValueInt|ValueAddr|ValueLateLabel))) \
	{ fprintf (stderr, ": %i %i\n", lvalue->Tag.t, rvalue->Tag.t);return FALSE;} \
 \
      help_evalSubLate(lvalue,rvalue); \
 /* Might not be a ValueInt, but ValueLate* has i at the same place */ \
      if(!(lvalue->Tag.t & (ValueInt | ValueAddr))) \
	return FALSE; \
 \
      lvalue->ValueBool.b = lvalue->ValueInt.i OP rvalue->ValueInt.i; \
      lvalue->Tag.t = ValueBool; \
      return TRUE		/* Last ; is where macro is used */


BOOL
evalBinop (Operator op, Value * lvalue, const Value * rvalue)
{
  switch (op)
    {
    case Op_mul:
      if ((lvalue->Tag.t == ValueAddr && rvalue->Tag.t == ValueInt)
	  || (lvalue->Tag.t == ValueInt && rvalue->Tag.t == ValueAddr))
	{
	  lvalue->ValueInt.i *= rvalue->ValueInt.i;
	  return TRUE;
	}
      if (lvalue->Tag.t != rvalue->Tag.t)
	return FALSE;
      switch (lvalue->Tag.t)
	{
	case ValueInt:
	  lvalue->ValueInt.i *= rvalue->ValueInt.i;
	  return TRUE;
	case ValueFloat:
	  lvalue->ValueFloat.f *= rvalue->ValueFloat.f;
	  return TRUE;
	default:
	  abort ();
	  break;
	}
      return FALSE;
    case Op_div:
      if (lvalue->Tag.t != rvalue->Tag.t)
	return FALSE;
      switch (lvalue->Tag.t)
	{
	case ValueInt:
	  lvalue->ValueInt.i /= rvalue->ValueInt.i;
	  return TRUE;
	case ValueFloat:
	  lvalue->ValueFloat.f /= rvalue->ValueFloat.f;
	  return TRUE;
	default:
	  abort ();
	  break;
	}
      return FALSE;
    case Op_mod:
      if (lvalue->Tag.t != ValueInt || rvalue->Tag.t != ValueInt)
	return FALSE;
      lvalue->ValueInt.i %= rvalue->ValueInt.i;
      return TRUE;
    case Op_add:
      if (lvalue->Tag.t == ValueFloat && rvalue->Tag.t == ValueFloat)
	{
	  lvalue->ValueFloat.f += rvalue->ValueFloat.f;
	  return TRUE;
	}
      if (lvalue->Tag.t == ValueAddr && rvalue->Tag.t == ValueInt)
	{
	  lvalue->ValueAddr.i += rvalue->ValueInt.i;
	  return TRUE;
	}
      if ((lvalue->Tag.t & (ValueInt | ValueLateLabel)) &&
	  (rvalue->Tag.t & (ValueInt | ValueLateLabel)))
	{
	  help_evalAddLate (lvalue, rvalue);
	  /* Might not be a ValueInt, but ValueLate* has i at the same place */
	  lvalue->ValueInt.i += rvalue->ValueInt.i;
	  return TRUE;
	}
      /* FALL THROUGH */
    case Op_concat:		/* fall thru from Op_add */
      if (lvalue->Tag.t == ValueString && rvalue->Tag.t == ValueString)
	{
	  char *c;
	  if ((c = malloc (lvalue->ValueString.len + rvalue->ValueString.len)) == NULL)
	    {
	      errorOutOfMem("evalBinop");
	      return FALSE;
	    }
	  memcpy (c, lvalue->ValueString.s, lvalue->ValueString.len);
	  memcpy (c + lvalue->ValueString.len,
		  rvalue->ValueString.s, rvalue->ValueString.len);
	  lvalue->ValueString.s = c;	/* string concatenation */
	  lvalue->ValueString.len += rvalue->ValueString.len;
	  return TRUE;
	}
      return FALSE;
    case Op_sub:
      if (lvalue->Tag.t == ValueFloat && rvalue->Tag.t == ValueFloat)
	{
	  lvalue->ValueFloat.f -= rvalue->ValueFloat.f;
	  return TRUE;
	}
      if (lvalue->Tag.t == ValueAddr && (rvalue->Tag.t & (ValueInt | ValueAddr)))
	{
	  if (rvalue->Tag.t == ValueAddr &&
	      lvalue->ValueAddr.r != rvalue->ValueAddr.r)
	    return FALSE;
	  lvalue->ValueAddr.i -= rvalue->ValueInt.i;
	  if (rvalue->Tag.t == ValueAddr)
	    lvalue->Tag.t = ValueInt;
	  return TRUE;
	}
      if (!(lvalue->Tag.t & (ValueInt | ValueLateLabel)) ||
	  !(rvalue->Tag.t & (ValueInt | ValueLateLabel)))
	return FALSE;

      help_evalSubLate (lvalue, rvalue);
      /* Might not be a ValueInt, but ValueLate* has i at the same place */
      lvalue->ValueInt.i -= rvalue->ValueInt.i;
      return TRUE;
    case Op_and:
      if ((lvalue->Tag.t == ValueAddr && rvalue->Tag.t == ValueInt) ||
	  (lvalue->Tag.t == ValueInt && rvalue->Tag.t == ValueAddr) ||
	  (lvalue->Tag.t == ValueInt && rvalue->Tag.t == ValueInt))
	{
	  lvalue->ValueInt.i &= rvalue->ValueInt.i;
	  return TRUE;
	}
      return FALSE;
    case Op_or:
      if (lvalue->Tag.t != ValueInt || rvalue->Tag.t != ValueInt)
	return FALSE;
      lvalue->ValueInt.i |= rvalue->ValueInt.i;
      return TRUE;
    case Op_xor:
      if (lvalue->Tag.t != ValueInt || rvalue->Tag.t != ValueInt)
	return FALSE;
      lvalue->ValueInt.i ^= rvalue->ValueInt.i;
      return TRUE;
    case Op_asr:
      if (lvalue->Tag.t != ValueInt || rvalue->Tag.t != ValueInt)
	return FALSE;
      lvalue->ValueInt.i = ((signed int) lvalue->ValueInt.i) >> rvalue->ValueInt.i;
      return TRUE;
    case Op_sr:
      if (lvalue->Tag.t != ValueInt || rvalue->Tag.t != ValueInt)
	return FALSE;
      lvalue->ValueInt.i = ((WORD) lvalue->ValueInt.i) >> rvalue->ValueInt.i;
      return TRUE;
    case Op_sl:
      if (lvalue->Tag.t != ValueInt || rvalue->Tag.t != ValueInt)
	return FALSE;
      lvalue->ValueInt.i <<= rvalue->ValueInt.i;
      return TRUE;
    case Op_ror:
      if (lvalue->Tag.t != ValueInt || rvalue->Tag.t != ValueInt)
	return FALSE;
      lvalue->ValueInt.i = (((WORD) lvalue->ValueInt.i) >> rvalue->ValueInt.i) |
	((lvalue->ValueInt.i) << (32 - rvalue->ValueInt.i));
    case Op_rol:
      if (lvalue->Tag.t != ValueInt || rvalue->Tag.t != ValueInt)
	return FALSE;
      lvalue->ValueInt.i = ((lvalue->ValueInt.i) << rvalue->ValueInt.i) |
	(((WORD) lvalue->ValueInt.i) >> (32 - rvalue->ValueInt.i));
      return TRUE;
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
	  return TRUE;
	}
      COMPARE ( ==);
    case Op_ne:
      if (lvalue->Tag.t == ValueBool && rvalue->Tag.t == ValueBool)
	{
	  lvalue->ValueBool.b =
	    lvalue->ValueBool.b != rvalue->ValueBool.b;
	  return TRUE;
	}
      COMPARE (!=);
    case Op_land:
      if (lvalue->Tag.t != ValueBool || rvalue->Tag.t != ValueBool)
	return FALSE;
      lvalue->ValueBool.b = lvalue->ValueBool.b && rvalue->ValueBool.b;
      return TRUE;
    case Op_lor:
      if (lvalue->Tag.t != ValueBool || rvalue->Tag.t != ValueBool)
	return FALSE;
      lvalue->ValueBool.b = lvalue->ValueBool.b || rvalue->ValueBool.b;
      return TRUE;
    case Op_left:
      if (lvalue->Tag.t != ValueString || rvalue->Tag.t != ValueInt
	  || rvalue->ValueInt.i < 0)
	return FALSE;
      if (lvalue->ValueString.len > rvalue->ValueInt.i)
	lvalue->ValueString.len = rvalue->ValueInt.i;
      return TRUE;
    case Op_right:
      if (lvalue->Tag.t != ValueString || rvalue->Tag.t != ValueInt
	  || rvalue->ValueInt.i < 0)
	return FALSE;
      if (lvalue->ValueString.len > rvalue->ValueInt.i)
	{
	  lvalue->ValueString.s += lvalue->ValueString.len - rvalue->ValueInt.i;
	  lvalue->ValueString.len = rvalue->ValueInt.i;
	}
      return TRUE;
    default:
      error (ErrorError, TRUE, "Illegal binary operator");
      break;
    }
  error (ErrorError, TRUE, "Internal evalBinop: illegal fall through");
  return FALSE;
}

BOOL
evalUnop (Operator op, Value * value)
{
  switch (op)
    {
    case Op_fattr:
      if (value->Tag.t != ValueString)
	return FALSE;
      error (ErrorError, TRUE, "%s not implemented", "fattr");
      return TRUE;
    case Op_fexec:
      if (value->Tag.t != ValueString)
	return FALSE;
      error (ErrorError, TRUE, "%s not implemented", "fexec");
      return TRUE;
    case Op_fload:
      if (value->Tag.t != ValueString)
	return FALSE;
      error (ErrorError, TRUE, "%s not implemented", "fload");
      return TRUE;
    case Op_fsize:
      {
      const char *s;
      if (value->Tag.t != ValueString)
	return FALSE;
      if ((s = strndup(value->ValueString.s, value->ValueString.len)) == NULL)
        {
          errorOutOfMem("evalUnop");
          return FALSE;
        }
      {
	FILE *fp;
	const char *newS;
	if ((fp = getInclude (s, "r", &newS)) == NULL)
	  {
	    error (ErrorError, TRUE, "Cannot open file \"%s\"", s ? s : "");
	    free ((void *)s);
	    free ((void *)newS);
	    return FALSE;
	  }
	free ((void *)newS);
	if (fseek (fp, 0l, SEEK_END))
	  {
	    error (ErrorError, TRUE, "Cannot seek to end of file \"%s\"", s ? s : "");
	    free ((void *)s);
	    return FALSE;
	  }
	if (-1 == (value->ValueInt.i = (int) ftell (fp)))
	  {
	    error (ErrorError, TRUE, "Cannot find size of file \"%s\"", s ? s : "");
	    free ((void *)s);
	    return FALSE;
	  }
	fclose (fp);
	free ((void *)s);
	value->Tag.t = ValueInt;
      }
      return TRUE;
      }
    case Op_lnot:
      if (value->Tag.t != ValueBool)
	return FALSE;
      value->ValueBool.b = !value->ValueBool.b;
      return TRUE;
    case Op_not:
      if (value->Tag.t != ValueInt)
	return FALSE;
      value->ValueInt.i = ~value->ValueInt.i;
      return TRUE;
    case Op_neg:
      if (value->Tag.t == ValueFloat)
	value->ValueFloat.f = -value->ValueFloat.f;
      else
	{
	  if (!(value->Tag.t & (ValueInt | ValueLateLabel)))
	    return FALSE;
	  help_evalNegLate (value);
	  value->ValueInt.i = -value->ValueInt.i;
	}
      return TRUE;
    case Op_index:
      if (value->Tag.t != ValueAddr)
	return FALSE;
      value->Tag.t = ValueInt;
      return TRUE;
    case Op_len:
      if (value->Tag.t != ValueString)
	return FALSE;
      value->Tag.t = ValueInt;
      return TRUE;
    case Op_str:
      {
	char num[32];
	switch (value->Tag.t)
	  {
	  case ValueInt:
	    sprintf (num, "%i", value->ValueInt.i);
	    break;
	  case ValueFloat:
	    sprintf (num, "%f", value->ValueFloat.f);
	    break;
	  default:
	    return FALSE;
	  }
	if ((value->ValueString.s = strdup (num)) == NULL)
	  {
	    errorOutOfMem("evalUnop");
	    return FALSE;
	  }
	value->ValueString.len = strlen (num);
	value->Tag.t = ValueString;
      }
      return TRUE;
    case Op_chr:
      {
	char num[2];
	if (value->Tag.t != ValueInt)
	  return FALSE;
	if ((num[0] = value->ValueInt.i) == 0)
	  error (ErrorWarning, TRUE, ":CHR:0 is a problem...");
	num[1] = 0;
	if ((value->ValueString.s = strdup (num)) == NULL)
	  {
	    errorOutOfMem("evalUnop");
	    return FALSE;
	  }
	value->ValueString.len = 1;
	value->Tag.t = ValueString;
      }
      return TRUE;
    default:
      error (ErrorError, TRUE, "Illegal unary operator");
      break;
    }
  error (ErrorSerious, FALSE, "Internal evalUnop: illegal fall through");
  return FALSE;
}
