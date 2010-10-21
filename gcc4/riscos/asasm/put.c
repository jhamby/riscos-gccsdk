/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2010 GCCSDK Developersrs
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


/**
 * Align the size of current area.
 * \entry align Align factor - 1, typically 1 (for 2 byte alignment) or 3 (for 4 byte alignment)
 * \entry msg NUL terminated reason for this alignment.
 */
static void
putAlign (int align, const char *msg)
{
  error (ErrorInfo, "Unaligned %s", msg);
  if (areaCurrentSymbol)
    while (areaCurrentSymbol->value.Data.Int.i & align)
      areaCurrentSymbol->area.info->image[areaCurrentSymbol->value.Data.Int.i++] = 0;
  else
    areaError ();
}

/**
 * Append 1, 2 or 4 bytes of data at the end of current area.
 * \entry size Size in bytes of the data to be written, should be 1, 2 or 4.
 * \entry data Data value to be written.
 */
void
putData (int size, ARMWord data)
{
  if (!areaCurrentSymbol)
    {
      areaError ();
      return;
    }
  if (option_align)
    {
      if (AREA_NOSPACE (areaCurrentSymbol->area.info,
			areaCurrentSymbol->value.Data.Int.i + size))
	areaGrow (areaCurrentSymbol->area.info, size);
      switch (size)
	{
	case 1:
	  break;
	case 2:
	  if (areaCurrentSymbol->value.Data.Int.i & 1)
	    putAlign (1, "halfword");
	  break;
	case 4:
	  if (areaCurrentSymbol->value.Data.Int.i & 3)
	    putAlign (3, "word");
	  break;
	default:
	  errorAbort ("Internal putData: illegal size");
	  break;
	}
    }

  if (AREA_IMAGE (areaCurrentSymbol->area.info))
    {
      if (AREA_NOSPACE (areaCurrentSymbol->area.info, areaCurrentSymbol->value.Data.Int.i + size))
	areaGrow (areaCurrentSymbol->area.info, size);
      switch (size)
	{
	case 4:
	  areaCurrentSymbol->area.info->image[areaCurrentSymbol->value.Data.Int.i + 3] = (data >> 24) & 0xff;
	  areaCurrentSymbol->area.info->image[areaCurrentSymbol->value.Data.Int.i + 2] = (data >> 16) & 0xff;
	case 2:
	  areaCurrentSymbol->area.info->image[areaCurrentSymbol->value.Data.Int.i + 1] = (data >> 8) & 0xff;
	case 1:
	  areaCurrentSymbol->area.info->image[areaCurrentSymbol->value.Data.Int.i + 0] = data & 0xff;
	}
    }
  else if (data)
    {
      error (ErrorError, "Trying to define a non-zero value in an uninitialised area");
      return;
    }
  areaCurrentSymbol->value.Data.Int.i += size;
}


/**
 * Append single or double float at the end of current area.
 * \entry size Size of the float data to be written, should be 4 (single float) or 8 (double float).
 * \entry data Float value to be written.
 */
void
putDataFloat (int size, ARMFloat data)
{
  /* FIXME: this wrongly assumes that the native float/double layout corresponds
     with the ARM FPA layout.  */
  union
  {
    double d;
    float f;
    struct
      {
	char c[8];		/* endianness? */
      } u;
  } translate;

  if (!areaCurrentSymbol)
    {
      areaError ();
      return;
    }
  switch (size)
    {
    case 4:
      translate.f = (float) data;
      if (areaCurrentSymbol->value.Data.Int.i & 3)
	putAlign (3, "float single");
      break;
    case 8:
      translate.d = (double) data;
      if (areaCurrentSymbol->value.Data.Int.i & 3)
	putAlign (3, "float double");
      break;
    default:
      errorAbort ("Internal putDataFloat: illegal size %d", size);
      break;
    }

  if (AREA_IMAGE (areaCurrentSymbol->area.info))
    {
      if (AREA_NOSPACE (areaCurrentSymbol->area.info, areaCurrentSymbol->value.Data.Int.i + size))
	areaGrow (areaCurrentSymbol->area.info, size);
      for (int i = 0; i < size; i++)
	areaCurrentSymbol->area.info->image[areaCurrentSymbol->value.Data.Int.i++] = translate.u.c[i];
    }
  else if (data)
    {
      error (ErrorError, "Trying to define a non-zero value in an uninitialised area");
      return;
    }
}


/**
 * Append ARM instruction at the end of current area.
 * \entry ins ARM instruction value to be written.
 */
void
putIns (ARMWord ins)
{
  if (!areaCurrentSymbol)
    {
      areaError ();
      return;
    }
  if (areaCurrentSymbol->value.Data.Int.i & 3)
    putAlign (3, "instruction");

  if (AREA_IMAGE (areaCurrentSymbol->area.info))
    {
      if (AREA_NOSPACE (areaCurrentSymbol->area.info, areaCurrentSymbol->value.Data.Int.i + 4))
	areaGrow (areaCurrentSymbol->area.info, 4);
      areaCurrentSymbol->area.info->image[areaCurrentSymbol->value.Data.Int.i++] = ins & 0xff;
      areaCurrentSymbol->area.info->image[areaCurrentSymbol->value.Data.Int.i++] = (ins >> 8) & 0xff;
      areaCurrentSymbol->area.info->image[areaCurrentSymbol->value.Data.Int.i++] = (ins >> 16) & 0xff;
      areaCurrentSymbol->area.info->image[areaCurrentSymbol->value.Data.Int.i++] = (ins >> 24) & 0xff;
    }
  else
    error (ErrorError, "Trying to define code an uninitialised area");
}
