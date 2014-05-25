/*
 * AsAsm an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2014 GCCSDK Developers
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
#include <stdint.h>

#include "code.h"
#include "directive_storagemap.h"
#include "error.h"
#include "expr.h"
#include "filestack.h"
#include "get.h"
#include "input.h"
#include "main.h"
#include "symbol.h"
#include "value.h"

static Value oStorageMapValue =
{
  .Tag = ValueInt,
  .Data.Int = { .i = 0, .type = eIntType_PureInt }
};

const Value *
StorageMap_Value (void)
{
  assert ((oStorageMapValue.Tag == ValueInt && oStorageMapValue.Data.Int.type == eIntType_PureInt) || oStorageMapValue.Tag == ValueAddr || oStorageMapValue.Tag == ValueSymbol);
  return &oStorageMapValue;
}

/**
 * Implementation for '^' / MAP.
 */
bool
c_record (void)
{
  const Value *value = Expr_BuildAndEval (ValueInt | ValueAddr | ValueSymbol);
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
	{
	  /* When a symbol gets redefined with Symbol_Define, we will only test
	     its value remains the same.  For mapping labels, this isn't good enough
	     as its 'code size' depends on the '#' argument, we can't allow, e.g.
	        foo # 0
	        foo # 4
	     Twice foo # 0 would be ok (code size being 0 for both) but there
	     isn't much value to allow this, hence, don't allow a mapping label to
	     be redefined at all.  */
	  Symbol *mapSymbol = Symbol_Get (lex->Data.Id.str, lex->Data.Id.len);
	  if (gPhase == ePassOne && (mapSymbol->attr.type & SYMBOL_DEFINED) != 0)
	    {
	      Error (ErrorError, "Mapping label can not be redefined");
	      if (mapSymbol->attr.fileName != FS_GetCurFileName() || mapSymbol->attr.lineNumber != FS_GetCurLineNumber ())
		Error_Line (mapSymbol->attr.fileName, mapSymbol->attr.lineNumber, ErrorError, "note: previous definition was done here");
	      return false;
	    }
	  if (Symbol_Define (mapSymbol, SYMBOL_ABSOLUTE | SYMBOL_NO_EXPORT, StorageMap_Value ()))
	    return false;
	  break;
	}

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
