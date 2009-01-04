/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2008 GCCSDK Developersrs
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
 * put.c
 */

#include "config.h"
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "area.h"
#include "error.h"
#include "main.h"
#include "put.h"

static void
putAlign (int align, const char *msg)
{
  error (ErrorInfo, TRUE, "Unaligned %s", msg);
  if (areaCurrentSymbol)
    while (areaCurrentSymbol->value.ValueInt.i & align)
      areaCurrentSymbol->area.info->image[areaCurrentSymbol->value.ValueInt.i++] = 0;
  else
    areaError ();
}


void
putData (int size, WORD data)
{
  if (!areaCurrentSymbol)
    {
      areaError ();
      return;
    }
  if (align)
    {
      if (AREA_NOSPACE (areaCurrentSymbol->area.info,
			areaCurrentSymbol->value.ValueInt.i + size))
	areaGrow (areaCurrentSymbol->area.info, size);
      switch (size)
	{
	case 1:
	  break;
	case 2:
	  if (areaCurrentSymbol->value.ValueInt.i & 1)
	    putAlign (1, "halfword");
	  break;
	case 4:
	  if (areaCurrentSymbol->value.ValueInt.i & 3)
	    putAlign (3, "word");
	  break;
	default:
	  error (ErrorSerious, TRUE, "Internal putData: illegal size");
	  break;
	}
    }

  if (AREA_IMAGE (areaCurrentSymbol->area.info))
    {
      if (AREA_NOSPACE (areaCurrentSymbol->area.info, areaCurrentSymbol->value.ValueInt.i + size))
	areaGrow (areaCurrentSymbol->area.info, size);
      switch (size)
	{
	case 4:
	  areaCurrentSymbol->area.info->image[areaCurrentSymbol->value.ValueInt.i + 3] = (data >> 24) & 0xff;
	  areaCurrentSymbol->area.info->image[areaCurrentSymbol->value.ValueInt.i + 2] = (data >> 16) & 0xff;
	case 2:
	  areaCurrentSymbol->area.info->image[areaCurrentSymbol->value.ValueInt.i + 1] = (data >> 8) & 0xff;
	case 1:
	  areaCurrentSymbol->area.info->image[areaCurrentSymbol->value.ValueInt.i + 0] = data & 0xff;
	}
    }
  else if (data)
    {
      error (ErrorError, TRUE, "Trying to define a non-zero value in an uninitialised area");
      return;
    }
  areaCurrentSymbol->value.ValueInt.i += size;
}


void
putDataFloat (int size, FLOAT data)
{
  union
  {
    double d;
    float f;
    struct
      {
	char c[8];		/* endianness? */
      }
    u;
  }
  translate;
  int i;

  if (!areaCurrentSymbol)
    {
      areaError ();
      return;
    }
  switch (size)
    {
    case 4:
      translate.f = (float) data;
      if (areaCurrentSymbol->value.ValueInt.i & 3)
	putAlign (3, "float single");
      break;
    case 8:
      translate.d = (double) data;
      if (areaCurrentSymbol->value.ValueInt.i & 3)
	putAlign (3, "float double");
      break;
    default:
      error (ErrorSerious, TRUE, "Internal putDataFloat: illegal size %d", size);
      break;
    }

  if (AREA_IMAGE (areaCurrentSymbol->area.info))
    {
      if (AREA_NOSPACE (areaCurrentSymbol->area.info, areaCurrentSymbol->value.ValueInt.i + size))
	areaGrow (areaCurrentSymbol->area.info, size);
      for (i = 0; i < size; i++)
	{
	  areaCurrentSymbol->area.info->image[areaCurrentSymbol->value.ValueInt.i + i] = translate.u.c[i];
	}
    }
  else if (data)
    {
      error (ErrorError, TRUE, "Trying to define a non-zero value in an uninitialised area");
      return;
    }
  areaCurrentSymbol->value.ValueInt.i += size;
}


void
putIns (WORD ins)
{
  if (!areaCurrentSymbol)
    {
      areaError ();
      return;
    }
  if (areaCurrentSymbol->value.ValueInt.i & 3)
    putAlign (3, "instruction");

  if (AREA_IMAGE (areaCurrentSymbol->area.info))
    {
      if (AREA_NOSPACE (areaCurrentSymbol->area.info, areaCurrentSymbol->value.ValueInt.i + 4))
	areaGrow (areaCurrentSymbol->area.info, 4);
      areaCurrentSymbol->area.info->image[areaCurrentSymbol->value.ValueInt.i + 3] = (ins >> 24) & 0xff;
      areaCurrentSymbol->area.info->image[areaCurrentSymbol->value.ValueInt.i + 2] = (ins >> 16) & 0xff;
      areaCurrentSymbol->area.info->image[areaCurrentSymbol->value.ValueInt.i + 1] = (ins >> 8) & 0xff;
      areaCurrentSymbol->area.info->image[areaCurrentSymbol->value.ValueInt.i + 0] = ins & 0xff;
      areaCurrentSymbol->value.ValueInt.i += 4;
    }
  else
    error (ErrorError, TRUE, "Trying to define code an uninitialised area");
}
