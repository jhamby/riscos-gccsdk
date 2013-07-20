/*
 * AsAsm an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2013 GCCSDK Developers
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif

#include "area.h"
#include "code.h"
#include "error.h"
#include "expr.h"
#include "main.h"
#include "value.h"


/**
 * A label is a defining symbol holding an area symbol as value.
 */
bool
Value_IsLabel (const Value *valueP)
{
  return valueP->Tag == ValueSymbol
	   && (valueP->Data.Symbol.symbol->type & SYMBOL_AREA) != 0;
}


/**
 * Copies one Value to another, correctly freeing/claiming memory resources.
 */
void
Value_Assign (Value *dst, const Value *src)
{
  if (dst == src)
    return;

  Value_Free (dst);
  *dst = Value_Copy (src);
}

/**
 * Copies one Value to another, correctly freeing/claiming memory resources.
 */
Value
Value_Copy (const Value *src)
{
  switch (src->Tag)
    {
      case ValueIllegal:
      case ValueInt:
      case ValueInt64:
      case ValueFloat:
      case ValueBool:
      case ValueAddr:
      case ValueSymbol:
	return *src;

      case ValueString:
	{
	  size_t len = src->Data.String.len;
	  char *c;
	  if ((c = malloc (len)) == NULL)
	    Error_OutOfMem ();
	  memcpy (c, src->Data.String.s, len);
	  return Value_String (c, len, true);
	}

      case ValueCode:
        return Value_Code (src->Data.Code.len, src->Data.Code.c);

      default:
        assert (0);
        break;
    }
}

void
Value_Free (Value *value)
{
  switch (value->Tag)
    {
      case ValueIllegal:
      case ValueInt:
      case ValueInt64:
      case ValueFloat:
      case ValueBool:
      case ValueAddr:
      case ValueSymbol:
	break;

      case ValueString:
	if (value->Data.String.owns)
	  free ((void *)value->Data.String.s);
	break;

      case ValueCode:
	Code_Free ((Code *)value->Data.Code.c, value->Data.Code.len);
	break;

      default:
	assert (0);
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
      .Data.Code = { .len = len, .c = Code_Copy (len, code) }
    };
  return value;
}


/**
 * Try to resolve a ValueSymbol object to its defined value.
 * \return true When resolving failed.  No ownership transfered.
 */
bool
Value_ResolveSymbol (Value *valueP, bool resolveAbsArea)
{
  /* Resolve all defined symbols except for symbol labels (if we would resolve
     labels, we're losing relocation info).  */
  /* FIXME: this can probably loop forever: label1 -> label2 -> label1 */
  while (valueP->Tag == ValueSymbol
	 && ((valueP->Data.Symbol.symbol->type & SYMBOL_DEFINED) != 0
	     || (resolveAbsArea
		 && (valueP->Data.Symbol.symbol->type & SYMBOL_AREA) != 0
		 && (valueP->Data.Symbol.symbol->area->type & AREA_ABS) != 0)))
    {
      int offset = valueP->Data.Symbol.offset;
      int factor = valueP->Data.Symbol.factor;

      if (resolveAbsArea
	  && (valueP->Data.Symbol.symbol->type & SYMBOL_AREA) != 0)
	{
	  const Area *areaP = valueP->Data.Symbol.symbol->area;
	  assert ((areaP->type & AREA_ABS) != 0);
	  *valueP = Value_Int (factor * Area_GetBaseAddress (valueP->Data.Symbol.symbol) + offset, eIntType_PureInt);
	  break;
	}

      const Value *newValueP = &valueP->Data.Symbol.symbol->value;
      switch (newValueP->Tag)
	{
	  case ValueBool:
	  case ValueString:
	    {
	      if (factor != 1 || offset != 0)
		return true;
	      *valueP = *newValueP;
	      break;
	    }

	  case ValueInt:
	    if (newValueP->Data.Int.type == eIntType_PureInt)
	      *valueP = Value_Int (factor * newValueP->Data.Int.i + offset, eIntType_PureInt);
	    else if (factor == 1 && offset == 0) /* You can not do maths on registers or register lists.  */
	      *valueP = *newValueP;
	    else
	      return true;
	    break;

	  case ValueInt64:
	    *valueP = Value_Int64 (factor * newValueP->Data.Int.i + offset);
	    break;

	  case ValueFloat:
	    *valueP = Value_Float (factor * newValueP->Data.Float.f + offset);
	    break;

	  case ValueAddr:
	    {
	      if (factor != 1)
		return true;
	      *valueP = Value_Addr (newValueP->Data.Addr.r, newValueP->Data.Addr.i + offset);
	      break;
	    }

	  case ValueSymbol:
	    *valueP = Value_Symbol (newValueP->Data.Symbol.symbol, factor * newValueP->Data.Symbol.factor, factor * newValueP->Data.Symbol.offset + offset);
	    break;

	  default:
	    return true;
	}
    }

  return false;
}


bool
Value_Equal (const Value *a, const Value *b)
{
  Value aCp, bCp;

  if (a->Tag == ValueCode || a->Tag == ValueSymbol
      || b->Tag == ValueCode || b->Tag == ValueSymbol)
    {
      if (a->Tag == ValueCode && b->Tag == ValueCode
	  && a->Data.Code.len == b->Data.Code.len
	  && Code_Equal (a->Data.Code.len, a->Data.Code.c, b->Data.Code.c))
	return true;

      if (a->Tag == ValueSymbol && b->Tag == ValueSymbol
	  && a->Data.Symbol.factor == b->Data.Symbol.factor
	  && a->Data.Symbol.symbol == b->Data.Symbol.symbol
          && a->Data.Symbol.offset == b->Data.Symbol.offset)
	return true;

      aCp = Value_Copy (a);
      bCp = Value_Copy (b);

      Code_Init ();
      Code_Value (&aCp, true);
      Value_Assign (&aCp, Code_Eval (ValueAll));

      Code_Init ();
      Code_Value (&bCp, true);
      Value_Assign (&bCp, Code_Eval (ValueAll));

      a = &aCp;
      b = &bCp;
    }

  bool result;
  switch (a->Tag)
    {
      case ValueIllegal:
	result = b->Tag == ValueIllegal;
	break;

      case ValueInt:
	/* For ValueInt: a pure int matches any other int type.  */
	result = (b->Tag == ValueInt
	           && a->Data.Int.i == b->Data.Int.i
	           && (a->Data.Int.type == b->Data.Int.type
		        || a->Data.Int.type == eIntType_PureInt
		        || b->Data.Int.type == eIntType_PureInt))
		 || (b->Tag == ValueFloat
		      && a->Data.Int.type == eIntType_PureInt
		      && (ARMFloat)a->Data.Int.i == b->Data.Float.f);
	break;

      case ValueInt64:
	result = b->Tag == ValueInt64 && a->Data.Int64.i == b->Data.Int64.i;
	break;
	
      case ValueFloat:
	result = (b->Tag == ValueFloat && a->Data.Float.f == b->Data.Float.f)
		   || (b->Tag == ValueInt && b->Data.Int.type == eIntType_PureInt && a->Data.Float.f == (ARMFloat)b->Data.Int.i);
	break;

      case ValueString:
	result = b->Tag == ValueString
		   && a->Data.String.len == b->Data.String.len
		   && !memcmp (a->Data.String.s, b->Data.String.s, a->Data.String.len);
	break;

      case ValueBool:
	result = b->Tag == ValueBool && a->Data.Bool.b == b->Data.Bool.b;
	break;

      case ValueCode:
	result = b->Tag == ValueCode
		   && a->Data.Code.len == b->Data.Code.len
		   && Code_Equal (a->Data.Code.len, a->Data.Code.c, b->Data.Code.c);
	break;

      case ValueAddr:
	result = b->Tag == ValueAddr
		   && a->Data.Addr.i == b->Data.Addr.i
		   && a->Data.Addr.r == b->Data.Addr.r;
	break;

      case ValueSymbol:
	result = b->Tag == ValueSymbol
		   && a->Data.Symbol.factor == b->Data.Symbol.factor
		   && a->Data.Symbol.symbol == b->Data.Symbol.symbol
		   && a->Data.Symbol.offset == b->Data.Symbol.offset;
	break;

      default:
	assert (0);
	break;
    }

  if (a == &aCp)
    Value_Free (&aCp);
  if (b == &bCp)
    Value_Free (&bCp);
  
  return result;
}


#ifdef DEBUG
void
Value_Print (const Value *v)
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
	{
	  if (v->Data.Int.type == eIntType_PureInt)
	    printf ("Int <%d = 0x%x>", v->Data.Int.i, v->Data.Int.i);
	  else
	    {
	      if (v->Data.Int.type == eIntType_CPURList)
		{
		  unsigned regList = v->Data.Int.i & 0xFFFF;
		  printf ("RegList { ");
		  for (unsigned i = 0; i != 16; ++i)
		    if (regList & (1<<i))
		      printf ("R%d ", i);
		  printf ("}");
		}
	      else
		{
		  char type;
		  bool isReg;
		  switch (v->Data.Int.type)
		    {
		      case eIntType_CPU:
			type = 'r';
			isReg = true;
			break;
		      case eIntType_FPU:
			type = 'f';
			isReg = true;
			break;
		      case eIntType_NeonQuadReg:
			type = 'q';
			isReg = true;
			break;
		      case eIntType_NeonOrVFPDoubleReg:
			type = 'd';
			isReg = true;
			break;
		      case eIntType_VFPSingleReg:
			type = 's';
			isReg = true;
			break;
		      case eIntType_CoProReg:
			type = 'p';
			isReg = true;
			break;
		      case eIntType_CoProNum:
			type = 'c';
			isReg = false;
			break;
		      default:
			type = '?';
			isReg = true;
			break;
		    }
		  printf ("%s %c%d", isReg ? "Reg" : "CoPro num", type, v->Data.Int.i);
		}
	    }
	  break;
	}
      case ValueInt64:
	printf ("Int64 <%ld = 0x%lx>", (int64_t)v->Data.Int64.i, v->Data.Int64.i);
	break;
      case ValueFloat:
	printf ("Float <%g>", v->Data.Float.f);
	break;
      case ValueString:
	printf ("String <%.*s> (%s)", (int)v->Data.String.len, v->Data.String.s, v->Data.String.owns ? "owns" : "NO ownership");
	break;
      case ValueBool:
	printf ("Bool <%s>", v->Data.Bool.b ? "true" : "false");
	break;
      case ValueCode:
	printf ("Code: ");
	Code_Print (v->Data.Code.len, v->Data.Code.c);
	break;
      case ValueAddr:
	printf ("AddrOffset reg %d + #0x%x", v->Data.Addr.r, v->Data.Addr.i);
	break;
      case ValueSymbol:
	printf ("Symbol %d x '%s' + #0x%x", v->Data.Symbol.factor, v->Data.Symbol.symbol->str, v->Data.Symbol.offset);
	break;
      default:
	printf ("tag 0x%x ???", v->Tag);
	break;
    }
}
#endif
