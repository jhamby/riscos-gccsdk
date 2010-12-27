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
 * storage.c
 */

#include "config.h"

#include <assert.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif

#include "area.h"
#include "code.h"
#include "error.h"
#include "expr.h"
#include "get.h"
#include "input.h"
#include "main.h"
#include "storage.h"
#include "value.h"

static Value storageV =
{
  .Tag = ValueInt,
  .Data.Int = { .i = 0 }
};

const Value *
storageValue (void)
{
  assert (storageV.Tag == ValueInt || storageV.Tag == ValueAddr || storageV.Tag == ValueCode);
  return &storageV;
}

/**
 * Implementation for '^' / MAP.
 */
bool
c_record (void)
{
  const Value *value = exprBuildAndEval (ValueInt | ValueAddr | ValueCode);
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
	  Value_Assign (&storageV, &src);
	}
	break;

      case ValueAddr:
      case ValueCode:
	Value_Assign (&storageV, value);
	break;
	
      default:
	{
	  const Value src =
	    {
	      .Tag = ValueInt,
	      .Data.Int = { .i = 0 }
	    };
	  Value_Assign (&storageV, &src);
	  error (ErrorError, "^ cannot evaluate its offset expression");
	}
        break;
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
      Symbol *sym = symbolAdd (lex);
      if (sym != NULL && sym->value.Tag == ValueIllegal)
	{
	  assert (sym->type & SYMBOL_DEFINED);
	  sym->type |= SYMBOL_ABSOLUTE;
	  Value_Assign (&sym->value, storageValue ());
	}
    }
  
  /* Determine how much we should allocate.  */
  const Value *value = exprBuildAndEval (ValueInt);
  switch (value->Tag)
    {
      case ValueInt:
        if (value->Data.Int.i < 0)
	  {
	    error (ErrorError, "Cannot reserve negative amount of space %d", value->Data.Int.i);
            break;
	  }
	else if (value->Data.Int.i == 0)
	  {
	    if (option_pedantic)
	      error (ErrorInfo, "You are reserving zero bytes?");
	    break;
	  }
	else
	  {
	    codeInit ();
	    codeValue (storageValue (), true);
	    codeValue (value, true);
	    codeOperator (Op_add);
            value = codeEval (ValueInt | ValueAddr | ValueCode, NULL);
	    if (value->Tag != ValueIllegal)
	      {
	        Value_Assign (&storageV, value);
		break;
	      }
	  }
	/* Fall through.  */
	
      default:
        error (ErrorError, "Illegal expression after #");
        break;
    }

  return false;
}
