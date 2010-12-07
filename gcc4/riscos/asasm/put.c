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

#include <assert.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif

#include "area.h"
#include "error.h"
#include "main.h"
#include "put.h"

/**
 * Append 1, 2 or 4 bytes of data at the end of current area.
 * \entry size Size in bytes of the data to be written, should be 1, 2 or 4.
 * \entry data Data value to be written.
 */
void
Put_DataWithOffset (size_t offset, size_t size, ARMWord data)
{
  assert (offset + size <= areaCurrentSymbol->value.Data.Int.i || offset == areaCurrentSymbol->value.Data.Int.i); // FIXME: Limitation.
  if (offset == areaCurrentSymbol->value.Data.Int.i)
    putData (size, data);
  else
    {
      switch (size)
	{
	  case 4:
	    areaCurrentSymbol->area.info->image[offset + 3] = (data >> 24) & 0xff;
	    areaCurrentSymbol->area.info->image[offset + 2] = (data >> 16) & 0xff;
	  case 2:
	    areaCurrentSymbol->area.info->image[offset + 1] = (data >> 8) & 0xff;
	  case 1:
	    areaCurrentSymbol->area.info->image[offset + 0] = data & 0xff;
	    break;
	  default:
	    errorAbort ("Internal putData: illegal size");
	    break;
	}
    }
}

void
putData (size_t size, ARMWord data)
{
  if (AREA_IMAGE (areaCurrentSymbol->area.info))
    {
      if (option_align)
	{
	  switch (size)
	    {
	      case 1:
		break;

	      case 2:
		Area_AlignTo (2, "halfword");
		break;

	      case 4:
		Area_AlignTo (4, "word");
		break;

	      default:
		errorAbort ("Internal putData: illegal size");
		break;
	    }
	}

      if (AREA_NOSPACE (areaCurrentSymbol->area.info, areaCurrentSymbol->value.Data.Int.i + size))
	areaGrow (areaCurrentSymbol->area.info, size);

      switch (size)
	{
	  case 4:
	    areaCurrentSymbol->area.info->image[areaCurrentSymbol->value.Data.Int.i++] = data & 0xff;
	    data >>= 8;
	    areaCurrentSymbol->area.info->image[areaCurrentSymbol->value.Data.Int.i++] = data & 0xff;
	    data >>= 8;
	    /* Fall through.  */
	  case 2:
	    areaCurrentSymbol->area.info->image[areaCurrentSymbol->value.Data.Int.i++] = data & 0xff;
	    data >>= 8;
	    /* Fall through.  */
	  case 1:
	    areaCurrentSymbol->area.info->image[areaCurrentSymbol->value.Data.Int.i++] = data & 0xff;
	    break;

	  default:
	    errorAbort ("Internal putData: illegal size");
	    break;
	}
    }
  else if (data)
    error (ErrorError, "Trying to define a non-zero value in an uninitialised area");
}


/**
 * Append single or double float at the end of current area.
 * \entry size Size of the float data to be written, should be 4 (single float) or 8 (double float).
 * \entry data Float value to be written.
 */
void
putDataFloat (size_t size, ARMFloat data)
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

  switch (size)
    {
      case 4:
	translate.f = (float) data;
	Area_AlignTo (4, "float single");
	break;
      case 8:
	translate.d = (double) data;
	Area_AlignTo (4, "float double");
	break;
      default:
	errorAbort ("Internal putDataFloat: illegal size %zd", size);
	break;
    }
  
  if (AREA_IMAGE (areaCurrentSymbol->area.info))
    {
      if (AREA_NOSPACE (areaCurrentSymbol->area.info, areaCurrentSymbol->value.Data.Int.i + size))
	areaGrow (areaCurrentSymbol->area.info, size);
      for (size_t i = 0; i < size; i++)
	areaCurrentSymbol->area.info->image[areaCurrentSymbol->value.Data.Int.i++] = translate.u.c[i];
    }
  else if (data)
    error (ErrorError, "Trying to define a non-zero value in an uninitialised area");
}

void
Put_InsWithOffset (size_t offset, ARMWord ins)
{
  assert (offset + 4 <= areaCurrentSymbol->value.Data.Int.i || offset == areaCurrentSymbol->value.Data.Int.i); // FIXME: Limitation.
  if (offset == areaCurrentSymbol->value.Data.Int.i)
    putIns (ins);
  else
    {
      offset = (offset + 3) & -4;
      areaCurrentSymbol->area.info->image[offset++] = ins & 0xff;
      areaCurrentSymbol->area.info->image[offset++] = (ins >> 8) & 0xff;
      areaCurrentSymbol->area.info->image[offset++] = (ins >> 16) & 0xff;
      areaCurrentSymbol->area.info->image[offset++] = (ins >> 24) & 0xff;
    }
}

/**
 * Append ARM instruction at the end of current area.
 * \entry ins ARM instruction value to be written.
 */
void
putIns (ARMWord ins)
{
  Area_AlignTo (4, "instruction");

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

ARMWord
GetWord (size_t offset)
{
  assert (offset <= areaCurrentSymbol->area.info->imagesize - 4);
  const uint8_t *p = &areaCurrentSymbol->area.info->image[offset];
  return p[0] + (p[1]<<8) + (p[2]<<16) + (p[3]<<24);
}

void
PutWord (size_t offset, ARMWord val)
{
  assert (offset <= areaCurrentSymbol->area.info->imagesize - 4);
  uint8_t *p = &areaCurrentSymbol->area.info->image[offset];
  p[0] = (val >>  0) & 0xff;
  p[1] = (val >>  8) & 0xff;
  p[2] = (val >> 16) & 0xff;
  p[3] = (val >> 24) & 0xff;
}
