/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2012 GCCSDK Developers
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
#include "expr.h"
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
      case ValueInt64:
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
      case ValueInt64:
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


/**
 * Resolve the defined value behind a ValueSymbol object.
 * \return true When resolving failed.
 */
bool
Value_ResolveSymbol (Value *valueP)
{
  /* Resolve all defined symbols and absolute area symbols.  */
  /* FIXME: this can probably loop forever: label1 -> label2 -> label1 */
  while (valueP->Tag == ValueSymbol /* Only replace symbols... */
         && ((valueP->Data.Symbol.symbol->type & SYMBOL_DEFINED) != 0
             || ((valueP->Data.Symbol.symbol->type & SYMBOL_AREA) != 0
		 && (valueP->Data.Symbol.symbol->type & SYMBOL_ABSOLUTE) != 0)))
    {
      int offset = valueP->Data.Symbol.offset;
      int factor = valueP->Data.Symbol.factor;
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
	    *valueP = Value_Int (factor * newValueP->Data.Int.i + offset, eIntType_PureInt);
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
valueEqual (const Value *a, const Value *b)
{
  Value aCp, bCp;

  if (a->Tag == ValueCode || a->Tag == ValueSymbol
      || b->Tag == ValueCode || b->Tag == ValueSymbol)
    {
      if (a->Tag == ValueCode && b->Tag == ValueCode
	  && a->Data.Code.len == b->Data.Code.len
	  && codeEqual (a->Data.Code.len, a->Data.Code.c, b->Data.Code.c))
	return true;

      if (a->Tag == ValueSymbol && b->Tag == ValueSymbol
	  && a->Data.Symbol.factor == b->Data.Symbol.factor
	  && a->Data.Symbol.symbol == b->Data.Symbol.symbol
          && a->Data.Symbol.offset == b->Data.Symbol.offset)
	return true;

      aCp.Tag = ValueIllegal;
      Value_Assign (&aCp, a);
      bCp.Tag = ValueIllegal;
      Value_Assign (&bCp, b);

      codeInit ();
      codeValue (&aCp, true);
      Value_Assign (&aCp, exprEval (ValueAll));

      codeInit ();
      codeValue (&bCp, true);
      Value_Assign (&bCp, exprEval (ValueAll));

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
	/* FIXME? Check on Value::Data.Int.type ? */
	result = (b->Tag == ValueInt && a->Data.Int.i == b->Data.Int.i)
		   || (b->Tag == ValueFloat && (ARMFloat)a->Data.Int.i == b->Data.Float.f);
	break;

      case ValueInt64:
	result = b->Tag == ValueInt64 && a->Data.Int64.i == b->Data.Int64.i;
	break;
	
      case ValueFloat:
	result = (b->Tag == ValueFloat && a->Data.Float.f == b->Data.Float.f)
		   || (b->Tag == ValueInt && a->Data.Float.f == (ARMFloat)b->Data.Int.i);
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
		   && codeEqual (a->Data.Code.len, a->Data.Code.c, b->Data.Code.c);
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
	errorAbort ("Internal valueEqual: illegal value");
	break;
    }

  if (a == &aCp)
    valueFree (&aCp);
  if (b == &bCp)
    valueFree (&bCp);
  
  return result;
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
        str = "integer/register/coprocessornumber";
        break;
      case ValueInt64:
	str = "int64";
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
	{
	  if (v->Data.Int.type == eIntType_PureInt)
	    printf ("Int <%d = 0x%x>", v->Data.Int.i, v->Data.Int.i);
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
	      printf ("%s %c%d", isReg ? "Reg" : "CoPro num ", type, v->Data.Int.i);
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
