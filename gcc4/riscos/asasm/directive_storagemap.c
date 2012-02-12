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
 * directive_storagemap.c
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
  .Data.Int = { .i = 0 }
};

const Value *
StorageMap_Value (void)
{
  assert (oStorageMapValue.Tag == ValueInt || oStorageMapValue.Tag == ValueAddr || oStorageMapValue.Tag == ValueSymbol || oStorageMapValue.Tag == ValueCode);
  return &oStorageMapValue;
}

/**
 * Implementation for '^' / MAP.
 */
bool
c_record (void)
{
  const Value *value = exprBuildAndEval (ValueInt | ValueAddr | ValueSymbol | ValueCode);
  switch (value->Tag)
    {
      case ValueInt:
	{
	  Value src;
	  if (Input_Match (',', true))
	    {
	      src.Tag = ValueAddr;
	      src.Data.Addr.i = value->Data.Int.i;
	      src.Data.Addr.r = (int)getCpuReg ();
	    }
	  else
	    {
	      src.Tag = ValueInt;
	      src.Data.Int.i = value->Data.Int.i;
	    }
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
	  const Value src =
	    {
	      .Tag = ValueInt,
	      .Data.Int = { .i = 0 }
	    };
	  Value_Assign (&oStorageMapValue, &src);
	  error (ErrorError, "^ cannot evaluate its offset expression");
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
  if (lex->tag == LexId)
    {
      if (Symbol_Define (symbolGet (lex), SYMBOL_ABSOLUTE, StorageMap_Value ()))
	return false;
    }
  
  /* Determine how much we should allocate.  */
  const Value *value = exprBuildAndEval (ValueInt);
  switch (value->Tag)
    {
      case ValueInt:
	codeInit ();
	codeValue (StorageMap_Value (), true);
	codeValue (value, true);
	codeOperator (Op_add);
        value = codeEval (ValueInt | ValueAddr | ValueSymbol | ValueCode, NULL);
	if (value->Tag != ValueIllegal)
	  {
	    Value_Assign (&oStorageMapValue, value);
	    break;
	  }
	/* Fall through.  */
	
      default:
        error (ErrorError, "Illegal expression after # or FIELD");
        break;
    }

  return false;
}
