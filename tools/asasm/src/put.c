/*
 * AsAsm an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2013 GCCSDK Developers
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
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#endif
#ifdef HAVE_INTTYPES_H
#  include <inttypes.h>
#endif
#include <ieee754.h>

#include "area.h"
#include "error.h"
#include "fix.h"
#include "fpu.h"
#include "main.h"
#include "put.h"
#include "state.h"


static void
ReportOverflow (unsigned dataSize, uint32_t dataValue)
{
  if (Fix_CheckForOverflow (dataSize, dataValue))
    Error (ErrorWarning, "Size value %" PRId32 " (= 0x%" PRIx32 ") exceeds %d byte%s",
           (int32_t)dataValue, (int32_t)dataValue, dataSize, dataSize == 1 ? "" : "s");
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

  if (!Area_IsNoInit (areaCurrentSymbol->area))
    {
      ReportOverflow (dataSize, dataValue);

      if (newOffset > areaCurrentSymbol->area->curIdx)
	Area_EnsureExtraSize (areaCurrentSymbol, newOffset - areaCurrentSymbol->area->curIdx);

      for (uint32_t i = offset; i != newOffset; i += dataSize)
	{
	  switch (dataSize)
	    {
	      case 8:
		areaCurrentSymbol->area->image[i + 7] = (dataValue >> 56) & 0xff;
		areaCurrentSymbol->area->image[i + 6] = (dataValue >> 48) & 0xff;
		areaCurrentSymbol->area->image[i + 5] = (dataValue >> 40) & 0xff;
		areaCurrentSymbol->area->image[i + 4] = (dataValue >> 32) & 0xff;
		/* Fall through.  */

	      case 4:
		areaCurrentSymbol->area->image[i + 3] = (dataValue >> 24) & 0xff;
		areaCurrentSymbol->area->image[i + 2] = (dataValue >> 16) & 0xff;
		/* Fall through.  */

	      case 2:
		areaCurrentSymbol->area->image[i + 1] = (dataValue >> 8) & 0xff;
		/* Fall through.  */

	      case 1:
		areaCurrentSymbol->area->image[i + 0] = dataValue & 0xff;
		break;

	      default:
		assert (0);
		break;
	    }
	}
    }
  else if (dataValue)
    Error (ErrorError, "Trying to define a non-zero value in an uninitialised area");

  /* Update current AREA index when necessary.  */
  if (newOffset > areaCurrentSymbol->area->curIdx)
    areaCurrentSymbol->area->curIdx = newOffset;
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
  Put_DataWithOffset (areaCurrentSymbol->area->curIdx, size, data, 1);
}


/**
 * Converts a floating-point value from single-precision to half-precision.
 *
 * There are two incompatible representations for half-precision floating-point
 * values.
 * When ieee is true : IEEE 754-2008 format, supports normalized values in the
 * range from 2^-14 to 65504 (i.e. (1 + &3FF / 2^-10) * 2^(30-15)
 * = 2^15 + &3FF*32 = 65504).
 * When ieee is false : ARM alternative format.  Does not support infinities
 * nor NaNs.  The range of exponents is extended so it supports normalized
 * values in the range from 2^-14 to 131008 (i.e. (1 + &3FF / 2^-10) * 2^(31-15)
 * = 2^16 + &3FF*64 = 131008).
 *
 *    b15 = sign
 * b10-14 = exponent (bias 15)
 *  b0-13 = mantissa
 */
static uint16_t
FloatToHalf (float flt, bool ieee)
{
  const union ieee754_float iflt = { .f = flt };
  const unsigned neg = iflt.ieee.negative << 15; /* 1 bit.  */
  int exp = iflt.ieee.exponent; /* 8 bits.  */
  unsigned mantissa = iflt.ieee.mantissa; /* 23 bits.  */

  /* +/-INF, qNaN or sNaN ? */
  if (exp == 0xFF)
    {
      if (!ieee)
	{
	  /* Impossible to represent, go for 0. when qNan/sNan, and for
	     +/-131008 when +/-INF.  */
	  Error (ErrorWarning, "ARM alternative half-precision format can not represent INF or NaN");
	  return mantissa ? 0 : neg | 0x7FFF; 
	}
      return neg | 0x7C00 | (mantissa >> (23 - 10));
    }

  /* +/- 0 ? */
  if (exp == 0 && mantissa == 0)
    return neg;

  /* Normalized or subnormal value.  */
  mantissa |= 1<<23; /* Add the implicit significant bit back.  */
  exp -= 127; /* Undo bias.  */
  /* value = mantissa * 2^-23 * 2^exp  */

  /* Do even-odd rounding.  */
  unsigned hp_lsb;
  if (exp < -14)
    {
      /* Half-precision result is going to be denormalised (or due to rounding
         it can become normalized to 2^-14).  */
      hp_lsb = 1 << (-exp - 1);
    }
  else
    {
      /* Half-precision result is going to be guaranteed normalized.  */
      hp_lsb = 1 << (23 - 10);
    }
  unsigned add = hp_lsb >> 1; /* Value to add before truncation.  */
  /* Test for the case where add-half-then-truncate differs from the even-odd
     rounding we want to do.  */
  if ((mantissa & (hp_lsb - 1)) == add)
    add = mantissa & hp_lsb; /* Even-odd rounding, add what hp_lsb is.  */
  mantissa += add;
  if (mantissa >= (1 << 24))
    {
      mantissa >>= 1;
      ++exp;
    }

  /* Test on overflow.  */
  if (ieee)
    {
      if (exp > 15)
	return neg | 0x7C00; /* sNaN.  */
    }
  else
    {
      if (exp > 16)
	return neg | 0x7FFF; /* Generate maximum value possible.  */
    }

  /* Test on underflow.  */
  if (exp < -14 - 10)
    return neg;

  /* Test on subnormal value.  */
  if (exp < -14)
    {
      mantissa >>= -14 - exp;
      exp = -14;
    }

  /* -1 because mantissa is 1<<10 too big.  */
  return neg | (((exp + 15 - 1) << 10) + (mantissa >> 13));
}


/**
 * Append single or double float at the end of current area.
 * \entry size Size of the float data to be written, should be 4 (single float) or 8 (double float).
 * \entry data Float value to be written.
 */
void
Put_FloatDataWithOffset (uint32_t offset, unsigned size, ARMFloat data, bool alignBefore)
{
  uint64_t toWrite;
  switch (size)
    {
      case 2:
	{
	  if (alignBefore)
	    offset = Area_AlignTo (offset, 2, "half-precision floating-point value");
          toWrite = FloatToHalf ((float)data, true); /* FIXME: ieee vs alternative option needed ? */
	  break;
	}

      case 4:
	{
	  if (alignBefore)
	    offset = Area_AlignTo (offset, 4, "single-precision floating-point value");

	  const union ieee754_float flt = { .f = (float)data };
	  /* float : ARM/FPA and ARM/VFP.  */
	  const union arm_float armflt =
	    {
	      .flt =
		{
		  .mantissa = flt.ieee.mantissa,
		  .exponent = flt.ieee.exponent,
		  .negative = flt.ieee.negative
		}
	    };
	  assert (sizeof (armflt) == 4);
	  toWrite = armflt.i;
	  break;
	}

      case 8:
	{
	  if (alignBefore)
	    offset = Area_AlignTo (offset, 4 /* yes, 4, not 8 */, "double-precision floating-point value");

	  const union ieee754_double dbl = { .d = data };
	  if ((areaCurrentSymbol->area->type & AREA_VFP) == 0)
	    {
	      /* double : ARM/FPA.  */
	      const union arm_double_fpa armdbl_fpa =
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
	      toWrite = armdbl_fpa.i;
	    }
	  else
	    {
	      /* double : ARM/VFP.  */
	      union arm_double_vfp armdbl_vfp =
		{
		  .dbl =
		    {
		      .mantissa1 = dbl.ieee.mantissa1,
		      .mantissa0 = dbl.ieee.mantissa0,
		      .exponent = dbl.ieee.exponent,
		      .negative = dbl.ieee.negative,
		    }
		};
	      assert (sizeof (armdbl_vfp) == 8);
	      toWrite = armdbl_vfp.i;
	    }
	  break;
	}

      default:
	assert (0);
	break;
    }

  Put_DataWithOffset (offset, size, toWrite, 1);
}


void
Put_InsWithOffset (uint32_t offset, unsigned size, ARMWord ins)
{
  offset = Area_AlignTo (offset, State_GetInstrType () == eInstrType_ARM ? 4 : 2, "instruction");

  if (State_GetInstrType () != eInstrType_ARM && size == 4)
    {
      Put_DataWithOffset (offset + 0, 2, ins >> 16, 1);
      Put_DataWithOffset (offset + 2, 2, ins & 0xFFFF, 1);
    }
  else
    Put_DataWithOffset (offset, size, ins, 1);
}


/**
 * Append ARM/Thumb instruction at the end of current area.
 * \entry ins ARM/Thumb instruction value to be written.
 */
void
Put_Ins (unsigned size, ARMWord ins)
{
  Put_InsWithOffset (areaCurrentSymbol->area->curIdx, size, ins);
}


void
Put_Ins_MOVW_MOVT (uint32_t cc, uint32_t destReg, uint32_t value, bool isMOVT)
{
  if (State_GetInstrType () == eInstrType_ARM)
    Put_Ins (4, cc | (isMOVT ? 0x03400000 : 0x03000000) | ((value & 0xF000)<<4) | (destReg<<12) | (value & 0x0FFF));
  else
    {
      Put_Ins (2, (isMOVT ? 0xF2C0 : 0xF240) | ((value & 0x0800) >> 1) | ((value & 0xF000) >> 12));
      Put_Ins (2, ((value & 0x0700) << 4) | (destReg<<8) | (value & 0x00FF));
    }
}


ARMWord
Put_GetHalfWord (uint32_t offset)
{
  assert (offset + 2 <= areaCurrentSymbol->area->imagesize);
  const uint8_t *p = &areaCurrentSymbol->area->image[offset];
  return p[0] + (p[1]<<8);
}


ARMWord
Put_GetWord (uint32_t offset)
{
  assert (offset + 4 <= areaCurrentSymbol->area->imagesize);
  const uint8_t *p = &areaCurrentSymbol->area->image[offset];
  return p[0] + (p[1]<<8) + (p[2]<<16) + (p[3]<<24);
}
