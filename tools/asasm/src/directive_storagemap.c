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
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif

#include "code.h"
#include "directive_storagemap.h"
#include "error.h"
#include "expr.h"
#include "get.h"
#include "input.h"
#include "main.h"
#include "value.h"

static Value oStorageMapValue =
{
  .Tag = ValueInt,
  .Data.Int = { .i = 0, .type = eIntType_PureInt }
};

const Value *
StorageMap_Value (void)
{
  assert ((oStorageMapValue.Tag == ValueInt && oStorageMapValue.Data.Int.type == eIntType_PureInt) || oStorageMapValue.Tag == ValueAddr || oStorageMapValue.Tag == ValueSymbol || oStorageMapValue.Tag == ValueCode);
  return &oStorageMapValue;
}

/**
 * Implementation for '^' / MAP.
 */
bool
c_record (void)
{
  const Value *value = Expr_BuildAndEval (ValueInt | ValueAddr | ValueSymbol | ValueCode);
  switch (value->Tag)
    {
      case ValueInt:
	{
	  const Value src = Input_Match (',', true) ? Value_Addr (Get_CPUReg (), value->Data.Int.i) : Value_Int (value->Data.Int.i, eIntType_PureInt); 
	  Value_Assign (&oStorageMapValue, &src);
	  break;
	}

      case ValueAddr:
      case ValueSymbol:
      case ValueCode:
	Value_Assign (&oStorageMapValue, value);
	break;
	
      default:
	{
	  const Value src = Value_Int (0, eIntType_PureInt);
	  Value_Assign (&oStorageMapValue, &src);
	  Error (ErrorError, "^ cannot evaluate its offset expression");
	  break;
	}
    }

  return false;
}

/**
 * Implementation for '#' / FIELD.
 */
bool
c_alloc (const Lex *lex)
{
  switch (lex->tag)
    {
      default:
	assert (0);
	/* Fall through.  */
      case LexNone:
	break;

      case LexId:
	if (Symbol_Define (Symbol_Get (lex), SYMBOL_ABSOLUTE | SYMBOL_NO_EXPORT, StorageMap_Value ()))
	  return false;
	break;

      case LexLocalLabel:
	Error (ErrorError, "Local label is not allowed here");
	return false;
    }

  /* Determine how much we should allocate.  */
  const Value *value = Expr_BuildAndEval (ValueInt);
  switch (value->Tag)
    {
      case ValueInt:
	Code_Init ();
	Code_Value (StorageMap_Value (), true);
	Code_Value (value, true);
	Code_Operator (eOp_Add);
        value = Code_Eval (ValueInt | ValueAddr | ValueSymbol | ValueCode);
	if (value->Tag != ValueIllegal)
	  {
	    Value_Assign (&oStorageMapValue, value);
	    break;
	  }
	/* Fall through.  */
	
      default:
        Error (ErrorError, "Illegal expression after # or FIELD");
        break;
    }

  return false;
}
