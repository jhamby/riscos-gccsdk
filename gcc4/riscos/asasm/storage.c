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

#include "error.h"
#include "expr.h"
#include "get.h"
#include "input.h"
#include "main.h"
#include "storage.h"
#include "value.h"

static Value storageV =
{
  .Tag = ValueAddr,
  .Data.Addr = { .i = 0, .r = -1 }
};

Value
storageValue (void)
{
  assert (storageV.Tag == ValueAddr);
  if (storageV.Data.Addr.r == -1)
    {
      const Value value =
	{
	  .Tag = ValueInt,
	  .Data.Int.i = storageV.Data.Addr.i
	};
      return value;
    }
  return storageV;
}

/**
 * Implementation for '^'.
 */
void
c_record (void)
{
  Value value = exprBuildAndEval (ValueInt);
  storageV.Tag = ValueAddr;
  switch (value.Tag)
    {
      case ValueInt:
        storageV.Data.Addr.i = value.Data.Int.i;
        break;
      default:
        storageV.Data.Addr.i = 0;
        errorAbort ("^ cannot evaluate its offset expression");
        break;
    }
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
      storageV.Data.Addr.r = getCpuReg ();
    }
  else
    storageV.Data.Addr.r = -1;
}

/**
 * Implementation for '#'.
 */
void
c_alloc (Symbol *sym)
{
  if (sym)
    {
      assert ((sym->type & (SYMBOL_ABSOLUTE | SYMBOL_DEFINED)) == (SYMBOL_ABSOLUTE | SYMBOL_DEFINED));
      sym->value = storageValue ();
    }

  /* Determine how much we should allocate.  */
  Value value = exprBuildAndEval (ValueInt);
  switch (value.Tag)
    {
      case ValueInt:
        if (value.Data.Int.i >= 0)
	  {
	    if (option_pedantic && value.Data.Int.i == 0)
	      error (ErrorInfo, "You are reserving zero bytes?");
	    storageV.Data.Addr.i += value.Data.Int.i;
	  }
        else
	  error (ErrorError, "Cannot reserve negative amount of space %d", value.Data.Int.i);
        break;
      default:
        error (ErrorError, "Illegal expression after #");
        break;
    }
}
