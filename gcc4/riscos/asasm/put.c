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
 * put.c
 */

#include "config.h"

#include <assert.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#endif
#ifdef HAVE_INTTYPES_H
#  include <inttypes.h>
#endif
#include <ieee754.h>

#include "area.h"
#include "error.h"
#include "main.h"
#include "put.h"


/* FIXME: too similar as Fix_Int */
static void
ReportOverflow (unsigned dataSize, uint64_t dataValue)
{
  int64_t dataValueMin, dataValueMax;
  switch (dataSize)
    {
      case 1:
	dataValueMin = INT8_MIN;
	dataValueMax = UINT8_MAX;
	break;
      case 2:
	dataValueMin = INT16_MIN;
	dataValueMax = UINT16_MAX;
	break;
      case 4:
	dataValueMin = INT32_MIN;
	dataValueMax = UINT32_MAX;
	break;
      case 8:
	return;
    }
  if ((int64_t)dataValue < dataValueMin || (int64_t)dataValue > dataValueMax)
    error (ErrorWarning, "Size value %" PRId64 " exceeds %d byte%s", (int64_t)dataValue,
           dataSize, dataSize == 1 ? "" : "s");
}


/**
 * Write 1, 2, 4 or 8 bytes of data at given offset in the current area.
 * The size of the current area will automatically increase when necessary.
 * \entry offset Offset where data needs to be written in the current area.
 * \entry dataSize Size in bytes of the data to be written, should be 1, 2, 4 or 8.
 * \entry dataValue Data value to be written.
 * \entry times Number of times to be done.
 */
static void
Put_DataWithOffset (uint32_t offset, unsigned dataSize, uint64_t dataValue,
                    uint32_t times)
{
  const uint32_t newOffset = offset + dataSize*times;

  if (!Area_IsNoInit (areaCurrentSymbol->area.info))
    {
      ReportOverflow (dataSize, dataValue);

      if (newOffset > areaCurrentSymbol->area.info->curIdx)
	Area_EnsureExtraSize (areaCurrentSymbol, newOffset - areaCurrentSymbol->area.info->curIdx);

      for (uint32_t i = offset; i != newOffset; i += dataSize)
	{
	  switch (dataSize)
	    {
	      case 8:
		areaCurrentSymbol->area.info->image[i + 7] = (dataValue >> 56) & 0xff;
		areaCurrentSymbol->area.info->image[i + 6] = (dataValue >> 48) & 0xff;
		areaCurrentSymbol->area.info->image[i + 5] = (dataValue >> 40) & 0xff;
		areaCurrentSymbol->area.info->image[i + 4] = (dataValue >> 32) & 0xff;
		/* Fall through.  */

	      case 4:
		areaCurrentSymbol->area.info->image[i + 3] = (dataValue >> 24) & 0xff;
		areaCurrentSymbol->area.info->image[i + 2] = (dataValue >> 16) & 0xff;
		/* Fall through.  */

	      case 2:
		areaCurrentSymbol->area.info->image[i + 1] = (dataValue >> 8) & 0xff;
		/* Fall through.  */

	      case 1:
		areaCurrentSymbol->area.info->image[i + 0] = dataValue & 0xff;
		break;

	      default:
		assert (0);
		break;
	    }
	}
    }
  else if (dataValue)
    error (ErrorError, "Trying to define a non-zero value in an uninitialised area");

  /* Update current AREA index when necessary.  */
  if (newOffset > areaCurrentSymbol->area.info->curIdx)
    areaCurrentSymbol->area.info->curIdx = newOffset;
}


void
Put_AlignDataWithOffset (uint32_t offset, unsigned size, ARMWord data,
                         uint32_t times, bool alignBefore)
{
  if (alignBefore)
    {
      switch (size)
	{
	  case 2:
	    offset = Area_AlignTo (offset, 2, "halfword");
	    break;

	  case 4:
	    offset = Area_AlignTo (offset, 4, "word");
	    break;
	}
    }

  Put_DataWithOffset (offset, size, data, times);
}


void
Put_Data (unsigned size, ARMWord data)
{
  Put_DataWithOffset (areaCurrentSymbol->area.info->curIdx, size, data, 1);
}


/**
 * Append single or double float at the end of current area.
 * \entry size Size of the float data to be written, should be 4 (single float) or 8 (double float).
 * \entry data Float value to be written.
 */
void
Put_FloatDataWithOffset (uint32_t offset, unsigned size, ARMFloat data, bool alignBefore)
{
  const union ieee754_float flt = { .f = (float)data };
  const union ieee754_double dbl = { .d = data };

  /* float : ARM/FPA and ARM/VFP.  */
  const union arm_float
    {
      uint32_t i;
      struct
	{
	  unsigned int mantissa:23;
	  unsigned int exponent:8;
	  unsigned int negative:1;
	} flt;
    } armflt =
    {
      .flt =
	{
	  .mantissa = flt.ieee.mantissa,
	  .exponent = flt.ieee.exponent,
	  .negative = flt.ieee.negative
	}
    };
  assert (sizeof (armflt) == 4);

  /* double : ARM/FPA.  */
  const union arm_double_fpa
    {
      uint64_t i;
      struct
	{
	  unsigned int mantissa0:20;
	  unsigned int exponent:11;
	  unsigned int negative:1;
	  unsigned int mantissa1:32;
	} dbl;
    } armdbl_fpa =
    {
      .dbl =
	{
	  .mantissa0 = dbl.ieee.mantissa0,
	  .exponent = dbl.ieee.exponent,
	  .negative = dbl.ieee.negative,
	  .mantissa1 = dbl.ieee.mantissa1
	}
    };
  assert (sizeof (armdbl_fpa) == 8);

#if 0
  /* FIXME: support VPA ! */
  /* double : ARM/VFP.  */
  union arm_double_vfp
    {
      char c[8];
      struct
	{
	  unsigned int mantissa1:32;
	  unsigned int mantissa0:20;
	  unsigned int exponent:11;
	  unsigned int negative:1;
	} dbl;
    } armdbl_vfp =
    {
      .dbl =
	{
	  .mantissa1 = dbl.ieee.mantissa1
	  .mantissa0 = dbl.ieee.mantissa0,
	  .exponent = dbl.ieee.exponent,
	  .negative = dbl.ieee.negative,
	}
    };
  assert (sizeof (armdbl_vfp) == 8);
#endif

  uint64_t toWrite;
  switch (size)
    {
      case 4:
	if (alignBefore)
	  offset = Area_AlignTo (offset, 4, "float single");
	toWrite = armflt.i;
	break;
      case 8:
	if (alignBefore)
	  offset = Area_AlignTo (offset, 4 /* yes, 4, not 8 */, "float double");
	toWrite = armdbl_fpa.i;
	break;
      default:
	errorAbort ("Internal Put_FloatDataWithOffset: illegal size %u", size);
	break;
    }

  Put_DataWithOffset (offset, size, toWrite, 1);
}

void
Put_InsWithOffset (uint32_t offset, ARMWord ins)
{
  offset = Area_AlignTo (offset, 4, "instruction");

  Put_DataWithOffset (offset, 4, ins, 1);
}

/**
 * Append ARM instruction at the end of current area.
 * \entry ins ARM instruction value to be written.
 */
void
Put_Ins (ARMWord ins)
{
  Put_InsWithOffset (areaCurrentSymbol->area.info->curIdx, ins);
}

ARMWord
GetWord (uint32_t offset)
{
  assert (offset <= areaCurrentSymbol->area.info->imagesize - 4);
  const uint8_t *p = &areaCurrentSymbol->area.info->image[offset];
  return p[0] + (p[1]<<8) + (p[2]<<16) + (p[3]<<24);
}
