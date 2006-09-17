/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas R�emo
 * Copyright (c) 2000-2006 GCCSDK Developers
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
#include "lex.h"
#include "main.h"
#include "storage.h"
#include "value.h"

static int storageD = 0;
static Value storageV;

Value
storageValue (void)
{
  if (!storageD)
    {
      error (ErrorError, TRUE, "No storage declared (# or @ before ^)");
      storageV.Tag.t = ValueAddr;
      storageV.ValueAddr.i = 0;
      storageV.ValueAddr.r = 15;
    }
  return valueCopy (storageV);
}

void
c_record (void)
{
  Value value;
  storageD = TRUE;
  exprBuild ();
  value = exprEval (ValueInt);
  storageV.ValueAddr.r = 15;
  storageV.Tag.t = ValueAddr;
  switch (value.Tag.t)
    {
    case ValueInt:
      storageV.ValueAddr.i = value.ValueInt.i;
      break;
    default:
      storageV.ValueAddr.i = 0;
      error (ErrorError, FALSE, "^ cannot evaluate its offset expression");
      break;
    }
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
      storageV.ValueAddr.r = getCpuReg ();
    }
}

void
c_alloc (Symbol * sym)
{
  Value value;
  if (sym)
    {
      sym->type |= SYMBOL_ABSOLUTE | SYMBOL_DEFINED;
      sym->area.ptr = NULL;
      sym->value = storageValue ();
    }
  exprBuild ();
  value = exprEval (ValueInt);
  switch (value.Tag.t)
    {
    case ValueInt:
      if (value.ValueInt.i >= 0)
	{
	  if (pedantic > 1 && value.ValueInt.i == 0)
	    error (ErrorInfo, TRUE, "You are reserving zero bytes?");
	  storageV.ValueAddr.i += value.ValueInt.i;
	  /* ValueInt & ValueAddr have i in the same place */
	  value.ValueAddr.r = storageV.ValueAddr.r;
	}
      else
	error (ErrorError, TRUE, "Cannot reserve negative amount of space %d", value.ValueInt.i);
      break;
    default:
      error (ErrorError, FALSE, "Illegal expression after #");
      break;
    }
}
