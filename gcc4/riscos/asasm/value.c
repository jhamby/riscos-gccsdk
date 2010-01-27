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
valueLateToCode(int offset, LateInfo *late)
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
  value.Tag.v = ValueConst;
  if ((value.ValueCode.c = malloc(size*sizeof(Code))) == NULL)
    errorOutOfMem ();

  value.Tag.t = ValueCode;
  value.ValueCode.len = size;
  size = 0;
  if (offset != 0)
    {
      value.ValueCode.c[size  ].Tag = CodeValue;
      value.ValueCode.c[size  ].CodeValue.value.Tag.t = ValueInt;
      value.ValueCode.c[size++].CodeValue.value.ValueInt.i = offset;
    }
  for (l = late; l != NULL; l = l->next)
    {
      if ((factor = l->factor) == -1 || factor == 1)
	{
	  value.ValueCode.c[size  ].Tag = CodeSymbol;
	  value.ValueCode.c[size++].CodeSymbol.symbol = l->symbol;
	  if (size != 1)
	    { /* Add offset */
	      value.ValueCode.c[size  ].Tag = CodeOperator;
	      value.ValueCode.c[size++].CodeOperator.op = (factor > 0) ? Op_add : Op_sub;
	    }
	  else if (factor == -1)
	    {
	      value.ValueCode.c[size  ].Tag = CodeOperator;
	      value.ValueCode.c[size++].CodeOperator.op = Op_neg;
	    }
	}
      else if (factor != 0)
	{
	  value.ValueCode.c[size  ].Tag = CodeSymbol;
	  value.ValueCode.c[size++].CodeSymbol.symbol = l->symbol;
	  value.ValueCode.c[size  ].Tag = CodeValue;
	  value.ValueCode.c[size  ].CodeValue.value.Tag.t = ValueInt;
	  value.ValueCode.c[size++].CodeValue.value.ValueInt.i = factor;
	  value.ValueCode.c[size  ].Tag = CodeOperator;
	  value.ValueCode.c[size++].CodeOperator.op = Op_mul;
	  if (size != 3)
	    { /* Add offset */
	      value.ValueCode.c[size  ].Tag = CodeOperator;
	      value.ValueCode.c[size++].CodeOperator.op = Op_add;
	    }
	}
    }

  return value;
}


Value
valueCopy (Value value)
{
  switch (value.Tag.t)
    {
    case ValueIllegal:
    case ValueInt:
    case ValueFloat:
    case ValueBool:
    case ValueAddr:
      break;
    case ValueString:
      if ((value.ValueString.s = strndup (value.ValueString.s, value.ValueString.len)) == NULL)
	errorOutOfMem ();
      break;
    case ValueCode:
      value.ValueCode.c = codeCopy (value.ValueCode.len, value.ValueCode.c);
      break;
    case ValueLateLabel:
      value = valueLateToCode (value.ValueLate.i, value.ValueLate.late);
      break;
    default:
      errorAbort ("Internal valueCopy: illegal value");
      value.Tag.t = ValueIllegal;
      break;
    }

  return value;
}

static void
valueFree(Value *value)
{
  switch (value->Tag.t)
    {
      case ValueIllegal:
      case ValueInt:
      case ValueFloat:
      case ValueBool:
      case ValueAddr: /* Needed here? */
	break;
      case ValueString:
	if (value->Tag.v == ValueString)
	  {
	    free((void *)value->ValueString.s);
	    value->ValueString.s = NULL;
	  }
	else
	  errorAbort ("Internal valueFree: cannot handle %s", "string");
	break;
      case ValueCode:
	free(value->ValueCode.c);
	value->ValueCode.c = NULL;
	break;
      case ValueLateLabel:
	errorAbort ("Internal valueFree: cannot handle %s", "late label");
	break;
      default:
	errorAbort ("Internal valueFree: illegal value");
	break;
    }
}

static BOOL
lateInfoEqual(const LateInfo *a, const LateInfo *b)
{
  for (/* */; a || b; a = a->next, b = b->next)
    {
      if (!a || !b
	  || a->factor != b->factor
	  || a->symbol != b->symbol)
	return FALSE;
    }
  return TRUE;
}

BOOL
valueEqual(const Value *a, const Value *b)
{
  if (a->Tag.t == ValueLateLabel)
    {
      /* Prevent valueEqual(ValueLateLabel,ValueCode) */
      const Value *t = a; a = b; b = t;
    }

  switch (a->Tag.t)
    {
      case ValueIllegal:
	return b->Tag.t == ValueIllegal;
      case ValueInt:
	return b->Tag.t == ValueInt && a->ValueInt.i == b->ValueInt.i;
      case ValueFloat:
	return b->Tag.t == ValueFloat && a->ValueFloat.f == b->ValueFloat.f;
      case ValueString:
	return b->Tag.t == ValueString
		 && a->ValueString.len == b->ValueString.len
		 && !memcmp(a->ValueString.s, b->ValueString.s, a->ValueString.len);
      case ValueBool:
	return b->Tag.t == ValueBool && a->ValueBool.b  == b->ValueBool.b;
      case ValueCode:
	if (b->Tag.t == ValueLateLabel)
	  {
	    Value v = valueLateToCode(b->ValueLate.i,b->ValueLate.late);
	    BOOL res = a->ValueCode.len == v.ValueCode.len && codeEqual(a->ValueCode.len,a->ValueCode.c,v.ValueCode.c);
	    valueFree(&v);
	    return res;
	  }
	else
	  return b->Tag.t == ValueCode
		   && a->ValueCode.len == b->ValueCode.len
		   && codeEqual(a->ValueCode.len,a->ValueCode.c,b->ValueCode.c);
      case ValueLateLabel:
	return b->Tag.t == ValueLateLabel
		 && a->ValueLate.i == b->ValueLate.i
		 && lateInfoEqual(a->ValueLate.late,b->ValueLate.late);
      default:
	errorAbort ("Internal valueEqual: illegal value");
	break;
    }

  return FALSE;
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
valuePrint(const Value *v)
{
  if (v == NULL)
    return;
  printf("Value %d:%d : ", v->Tag.t, v->Tag.v);
  switch (v->Tag.t)
    {
      case ValueIllegal:
	printf("<illegal>\n");
	break;
      case ValueInt:
	printf("<%d> (int)\n", v->ValueInt.i);
	break;
      case ValueFloat:
	printf("<%g> (float)\n", v->ValueFloat.f);
	break;
      case ValueString:
	printf("<%.*s> (string)\n", v->ValueString.len, v->ValueString.s);
	break;
      case ValueBool:
	printf("<%s> (bool)\n", v->ValueBool.b ? "TRUE" : "FALSE");
	break;
      case ValueCode:
	printf("(code)\n");
	break;
      case ValueLateLabel:
	printf("(late label)\n");
	break;
      case ValueAddr:
	printf("(addr)\n");
	break;
      default:
	printf("???\n");
	break;
    }
}
#endif
