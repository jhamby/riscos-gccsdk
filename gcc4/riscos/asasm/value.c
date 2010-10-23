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
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "code.h"
#include "error.h"
#include "os.h"
#include "value.h"

/* Code demands at least one Lateinfo */
Value
valueLateToCode (int offset, LateInfo *late)
{
  LateInfo *l;
  int factor;
  int size;
  for (size = 1, l = late; l != NULL; l= l->next)
    {
      if ((factor = l->factor) == -1 || factor == 1)
	size += 2;
      else if (factor != 0)
	size += 4;
    }
  if (offset == 0)
    {
      size -= 2;			/* No need to include offset ... */
      if (late->factor == -1)
	size++;				/* ... but must insert an unary - */
    }

  Value value;
  if ((value.Data.Code.c = malloc(size*sizeof(Code))) == NULL)
    errorOutOfMem ();

  value.Tag = ValueCode;
  value.Data.Code.len = size;
  size = 0;
  if (offset != 0)
    {
      value.Data.Code.c[size  ].Tag = CodeValue;
      value.Data.Code.c[size  ].Data.value.Tag = ValueInt;
      value.Data.Code.c[size++].Data.value.Data.Int.i = offset;
    }
  for (l = late; l != NULL; l = l->next)
    {
      if ((factor = l->factor) == -1 || factor == 1)
	{
	  value.Data.Code.c[size  ].Tag = CodeSymbol;
	  value.Data.Code.c[size++].Data.symbol = l->symbol;
	  if (size != 1)
	    { /* Add offset */
	      value.Data.Code.c[size  ].Tag = CodeOperator;
	      value.Data.Code.c[size++].Data.op = (factor > 0) ? Op_add : Op_sub;
	    }
	  else if (factor == -1)
	    {
	      value.Data.Code.c[size  ].Tag = CodeOperator;
	      value.Data.Code.c[size++].Data.op = Op_neg;
	    }
	}
      else if (factor != 0)
	{
	  value.Data.Code.c[size  ].Tag = CodeSymbol;
	  value.Data.Code.c[size++].Data.symbol = l->symbol;
	  value.Data.Code.c[size  ].Tag = CodeValue;
	  value.Data.Code.c[size  ].Data.value.Tag = ValueInt;
	  value.Data.Code.c[size++].Data.value.Data.Int.i = factor;
	  value.Data.Code.c[size  ].Tag = CodeOperator;
	  value.Data.Code.c[size++].Data.op = Op_mul;
	  if (size != 3)
	    { /* Add offset */
	      value.Data.Code.c[size  ].Tag = CodeOperator;
	      value.Data.Code.c[size++].Data.op = Op_add;
	    }
	}
    }

  return value;
}


Value
valueCopy (Value value)
{
  switch (value.Tag)
    {
      case ValueIllegal:
      case ValueInt:
      case ValueFloat:
      case ValueBool:
      case ValueAddr:
        break;

      case ValueString:
	{
	  char *c;
	  if ((c = malloc (value.Data.String.len)) == NULL)
	    errorOutOfMem ();
	  memcpy (c, value.Data.String.s, value.Data.String.len);
	  value.Data.String.s = c;
	}
        break;

      case ValueCode:
        value.Data.Code.c = codeCopy (value.Data.Code.len, value.Data.Code.c);
        break;

      case ValueLateLabel:
        value = valueLateToCode (value.Data.Late.i, value.Data.Late.late);
        break;

      default:
        errorAbort ("Internal valueCopy: illegal value");
        break;
    }

  return value;
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
      case ValueAddr: /* Needed here? */
	break;

      case ValueString:
	free ((void *)value->Data.String.s);
	value->Data.String.len = 0;
	value->Data.String.s = NULL;
	break;

      case ValueCode:
	free (value->Data.Code.c);
	value->Data.Code.len = 0;
	value->Data.Code.c = NULL;
	break;

      case ValueLateLabel:
	errorAbort ("Internal valueFree: cannot handle %s", "late label");
	break;

      default:
	errorAbort ("Internal valueFree: illegal value");
	break;
    }
}

static bool
lateInfoEqual (const LateInfo *a, const LateInfo *b)
{
  for (/* */; a || b; a = a->next, b = b->next)
    {
      if (!a || !b
	  || a->factor != b->factor
	  || a->symbol != b->symbol)
	return false;
    }
  return true;
}

bool
valueEqual (const Value *a, const Value *b)
{
  if (a->Tag == ValueLateLabel)
    {
      /* Prevent valueEqual(ValueLateLabel,ValueCode) */
      const Value *t = a; a = b; b = t;
    }

  switch (a->Tag)
    {
      case ValueIllegal:
	return b->Tag == ValueIllegal;
      case ValueInt:
	return b->Tag == ValueInt && a->Data.Int.i == b->Data.Int.i;
      case ValueFloat:
	return b->Tag == ValueFloat && a->Data.Float.f == b->Data.Float.f;
      case ValueString:
	return b->Tag == ValueString
		 && a->Data.String.len == b->Data.String.len
		 && !memcmp(a->Data.String.s, b->Data.String.s, a->Data.String.len);
      case ValueBool:
	return b->Tag == ValueBool && a->Data.Bool.b  == b->Data.Bool.b;
      case ValueCode:
	if (b->Tag == ValueLateLabel)
	  {
	    Value v = valueLateToCode(b->Data.Late.i,b->Data.Late.late);
	    bool res = a->Data.Code.len == v.Data.Code.len && codeEqual(a->Data.Code.len,a->Data.Code.c,v.Data.Code.c);
	    valueFree(&v);
	    return res;
	  }
	else
	  return b->Tag == ValueCode
		   && a->Data.Code.len == b->Data.Code.len
		   && codeEqual(a->Data.Code.len,a->Data.Code.c,b->Data.Code.c);
      case ValueLateLabel:
	return b->Tag == ValueLateLabel
		 && a->Data.Late.i == b->Data.Late.i
		 && lateInfoEqual(a->Data.Late.late,b->Data.Late.late);
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
      case ValueLateLabel:
        str = "label";
        break;
      case ValueAddr:
        str = "address";
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
  printf ("Value %d: ", v->Tag);
  switch (v->Tag)
    {
      case ValueIllegal:
	printf ("<illegal>");
	break;
      case ValueInt:
	printf ("<%d> (int)", v->Data.Int.i);
	break;
      case ValueFloat:
	printf ("<%g> (float)", v->Data.Float.f);
	break;
      case ValueString:
	printf ("<%.*s> (string)", (int)v->Data.String.len, v->Data.String.s);
	break;
      case ValueBool:
	printf ("<%s> (bool)", v->Data.Bool.b ? "true" : "false");
	break;
      case ValueCode:
	printf ("(code)");
	break;
      case ValueLateLabel:
	printf ("(late label)");
	break;
      case ValueAddr:
	printf ("offset 0x%x, reg %d (addr)", v->Data.Addr.i, v->Data.Addr.r);
	break;
      default:
	printf ("???");
	break;
    }
}
#endif
