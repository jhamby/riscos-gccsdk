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
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "error.h"
#include "expr.h"
#include "get.h"
#include "input.h"
#include "main.h"
#include "storage.h"
#include "value.h"

static Value storageV;

Value
storageValue (void)
{
  if (storageV.Tag != ValueAddr)
    {
      error (ErrorError, "No storage declared (# or @ before ^)"); /* FIXME: I don't think this should give an error.  */
      storageV.Tag = ValueAddr;
      storageV.ValueAddr.i = 0;
      storageV.ValueAddr.r = -1;
    }
  if (storageV.ValueAddr.r == -1)
    {
      Value value;
      value.Tag = ValueInt;
      value.ValueInt.i = storageV.ValueAddr.i;
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
  exprBuild ();
  Value value = exprEval (ValueInt);
  storageV.Tag = ValueAddr;
  switch (value.Tag)
    {
      case ValueInt:
        storageV.ValueAddr.i = value.ValueInt.i;
        break;
      default:
        storageV.ValueAddr.i = 0;
        errorAbort ("^ cannot evaluate its offset expression");
        break;
    }
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
      storageV.ValueAddr.r = getCpuReg ();
    }
  else
    storageV.ValueAddr.r = -1;
}

/**
 * Implementation for '#'.
 */
void
c_alloc (Symbol *sym)
{
  if (sym)
    {
      sym->type |= SYMBOL_ABSOLUTE | SYMBOL_DEFINED;
      sym->area.ptr = NULL;
      sym->value = storageValue ();
    }
  /* FIXME: we should store in the symbol how many bytes it represents in the
     output file in order to implement '?'.  */

  /* Determine how much we should allocate.  */
  exprBuild ();
  Value value = exprEval (ValueInt);
  switch (value.Tag)
    {
      case ValueInt:
        if (value.ValueInt.i >= 0)
	  {
	    if (option_pedantic && value.ValueInt.i == 0)
	      error (ErrorInfo, "You are reserving zero bytes?");
	    storageV.ValueAddr.i += value.ValueInt.i;
	  }
        else
	  error (ErrorError, "Cannot reserve negative amount of space %d", value.ValueInt.i);
        break;
      default:
        error (ErrorError, "Illegal expression after #");
        break;
    }
}
