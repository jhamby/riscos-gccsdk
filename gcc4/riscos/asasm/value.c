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
 * value.c
 */

#include "config.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif

#include "code.h"
#include "error.h"
#include "os.h"
#include "main.h"
#include "value.h"

/**
 * Assigns one Value to another, correctly freeing/claiming memory resources.
 */
void
Value_Assign (Value *dst, const Value *src)
{
  if (dst == src)
    return;

  valueFree (dst);

  *dst = *src;
  switch (src->Tag)
    {
      case ValueIllegal:
      case ValueInt:
      case ValueFloat:
      case ValueBool:
      case ValueAddr:
      case ValueSymbol:
        break;

      case ValueString:
	{
	  char *c;
	  if ((c = malloc (src->Data.String.len)) == NULL)
	    errorOutOfMem ();
	  memcpy (c, src->Data.String.s, src->Data.String.len);
	  dst->Data.String.s = c;
	}
        break;

      case ValueCode:
        dst->Data.Code.c = codeCopy (src->Data.Code.len, src->Data.Code.c);
        break;

      default:
        errorAbort ("Internal Value_Assign: illegal value");
        break;
    }
}

void
valueFree (Value *value)
{
  switch (value->Tag)
    {
      case ValueIllegal:
      case ValueInt:
      case ValueFloat:
      case ValueBool:
      case ValueAddr:
      case ValueSymbol:
	break;

      case ValueString:
	free ((void *)value->Data.String.s);
	break;

      case ValueCode:
	Code_Free (value->Data.Code.c, value->Data.Code.len);
	break;

      default:
	errorAbort ("Internal valueFree: illegal value");
	break;
    }
  value->Tag = ValueIllegal;
}

Value
Value_Code (size_t len, const Code *code)
{
  const Value value =
    {
      .Tag = ValueCode,
      .Data.Code = { .len = len, .c = codeCopy (len, code) }
    };
  return value;
}

bool
valueEqual (const Value *a, const Value *b)
{
  switch (a->Tag)
    {
      case ValueIllegal:
	return b->Tag == ValueIllegal;

      case ValueInt:
	return (b->Tag == ValueInt && a->Data.Int.i == b->Data.Int.i)
	         || (option_autocast && b->Tag == ValueFloat && (ARMFloat)a->Data.Int.i == b->Data.Float.f);

      case ValueFloat:
	return (b->Tag == ValueFloat && a->Data.Float.f == b->Data.Float.f)
	         || (option_autocast && b->Tag == ValueInt && a->Data.Float.f == (ARMFloat)b->Data.Int.i);

      case ValueString:
	return b->Tag == ValueString
		 && a->Data.String.len == b->Data.String.len
		 && !memcmp (a->Data.String.s, b->Data.String.s, a->Data.String.len);

      case ValueBool:
	return b->Tag == ValueBool && a->Data.Bool.b == b->Data.Bool.b;

      case ValueAddr:
	return b->Tag == ValueAddr
		 && a->Data.Addr.i == b->Data.Addr.i
		 && a->Data.Addr.r == b->Data.Addr.r;

      case ValueSymbol:
	return b->Tag == ValueSymbol
		 && a->Data.Symbol.factor == b->Data.Symbol.factor
		 && a->Data.Symbol.symbol == b->Data.Symbol.symbol;

      case ValueCode:
	  // FIXME: code pieces can differ in their components but still end up
	  // with the same semantics.  We should normalize before really binary
	  // comparing them.
	  return b->Tag == ValueCode
		   && a->Data.Code.len == b->Data.Code.len
		   && codeEqual (a->Data.Code.len, a->Data.Code.c, b->Data.Code.c);

      default:
	errorAbort ("Internal valueEqual: illegal value");
	break;
    }

  return false;
}


const char *
valueTagAsString (ValueTag tag)
{
  const char *str;
  switch (tag)
    {
      case ValueIllegal:
        str = "illegal";
        break;
      case ValueInt:
        str = "integer";
        break;
      case ValueFloat:
        str = "float";
        break;
      case ValueString:
        str = "string";
        break;
      case ValueBool:
        str = "bool";
        break;
      case ValueCode:
        str = "code";
        break;
      case ValueAddr:
        str = "register based address";
        break;
      case ValueSymbol:
	str = "symbol";
	break;
      default:
        str = "unknown";
        break;
    }
  return str;
}


#ifdef DEBUG
void
valuePrint (const Value *v)
{
  if (v == NULL)
    return;
  printf ("Value ");
  switch (v->Tag)
    {
      case ValueIllegal:
	printf ("<illegal>");
	break;
      case ValueInt:
	printf ("Int <%d>", v->Data.Int.i);
	break;
      case ValueFloat:
	printf ("Float <%g>", v->Data.Float.f);
	break;
      case ValueString:
	printf ("String <%.*s>", (int)v->Data.String.len, v->Data.String.s);
	break;
      case ValueBool:
	printf ("Bool <%s>", v->Data.Bool.b ? "true" : "false");
	break;
      case ValueCode:
	printf ("Code: ");
	codePrint (v->Data.Code.len, v->Data.Code.c);
	break;
      case ValueAddr:
	printf ("AddrOffset 0x%x, reg %d", v->Data.Addr.i, v->Data.Addr.r);
	break;
      case ValueSymbol:
	printf ("Symbol %d x '%s'", v->Data.Symbol.factor, v->Data.Symbol.symbol->str);
	break;
      default:
	printf ("tag 0x%x ???", v->Tag);
	break;
    }
}
#endif
