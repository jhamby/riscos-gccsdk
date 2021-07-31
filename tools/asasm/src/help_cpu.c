/*
 * AsAsm an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2001-2013 GCCSDK Developers
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

#include "help_cpu.h"

/**
 * Encodes given value as immediate constant for ARM instruction.
 * \param value Value to encode
 * \return When non-UINT32_MAX, the encoded immediate constant.  When
 * UINT32_MAX given value can not be encoded.
 */
uint32_t
HelpCPU_Imm8s4 (uint32_t value)
{
  static const uint32_t mask[] =
    {
      0x000000ff, 0xc000003f, 0xf000000f, 0xfc000003,
      0xff000000, 0x3fc00000, 0x0ff00000, 0x03fc0000,
      0x00ff0000, 0x003fc000, 0x000ff000, 0x0003fc00,
      0x0000ff00, 0x00003fc0, 0x00000ff0, 0x000003fc
    };

  for (unsigned i = 0; i != 16; i++)
    {
      /* An UAL assembler must select the encoding with the lowest unsigned
	 value of the rotation field.  */
      if ((value & mask[i]) == value)
	return ((value >> ((16 - i) * 2)) | value << (i * 2)) | i << 8;
    }
  return UINT32_MAX;
}

static unsigned int
ShiftL (unsigned int val, unsigned int shiftPos)
{
  assert (shiftPos < 32);
  return (val << shiftPos) | val >> (32 - shiftPos);
}

/**
 * \return Returns the minimal number of Imm8s4 constants needed to represent
 * the given constant.
 */
unsigned
HelpCPU_SplitByImm8s4 (uint32_t cnst, uint32_t c[4])
{
  /* Check the odd case.  */
  if (cnst == 0)
    {
      c[0] = 0;
      return 1;
    }

  /* Find the first byte with one or more bits set at its bottom 2 bit
     positions.  */
  unsigned shift;
  for (shift = 0; (cnst & (0x3<<shift)) == 0; shift += 2)
    /* */;

  /* Find the last shift position we never should consider.  */
  unsigned shiftEnd;
  for (shiftEnd = 32; (cnst & (0x3<<(shiftEnd - 2))) == 0; shiftEnd -= 2)
    /* */;
  /* So all shifts used in a solution are between 'shift' (incl) - 'shiftEnd'
     (excl).  */
  
  /* First combination is: 0xFF<<(shift + 0), 0xFF<<(shift + 2),
     0xFF<<(shift + 4) and 0xFF<<(shift + 6).  */
  unsigned instrNeeded = 5;
  for (unsigned shift0 = shift; shift0 != shift + 8; shift0 += 2)
    {
      /* First mask: */
      unsigned int m0 = ShiftL (0xFF, shift0);
      unsigned int r0 = cnst & ~m0;
      if (r0 == 0)
	{
	  c[0] = cnst & m0;
	  return 1; /* Best solution possible, bail out.  */
	}
      /* Figure out how many bytes we minimally need to mask out the
         remainer r0.  */

      /* Second mask: */
      unsigned shift1;
      for (shift1 = shift0 + 8;
	   shift1 < shiftEnd && (r0 & (0x3<<shift1)) == 0;
	   shift1 += 2)
	/* */;
      if (shift1 >= shiftEnd)
	continue;
      unsigned int m1 = ShiftL (0xFF, shift1);
      unsigned int r1 = r0 & ~m1;
      if (r1 == 0 /* && instrNeeded > 2 */)
        {
	  c[0] = cnst & m0;
          c[1] = r0 & m1;
          return 2; /* We know 1 instr was not enough, so this is best solution.  */
       }

      /* Third mask: */
      unsigned shift2;
      for (shift2 = shift1 + 8;
	   shift2 < shiftEnd && (r1 & (0x3<<shift2)) == 0;
	   shift2 += 2)
	/* */;
      if (shift2 >= shiftEnd)
	continue;
      unsigned int m2 = ShiftL (0xFF, shift2);
      unsigned int r2 = r1 & ~m2;
      if (r2 == 0 && instrNeeded > 3)
	{
	  c[0] = cnst & m0;
          c[1] = r0 & m1;
	  c[2] = r1 & m2;
	  instrNeeded = 3;
	  continue;
	}

      /* Fourth mask (the remaining bits): */
      if (instrNeeded > 4)
	{
	  /* Worst solution.  */
	  c[0] = cnst & m0;
          c[1] = r0 & m1;
	  c[2] = r1 & m2;
	  c[3] = r2;
	  instrNeeded = 4;
	}
    }
  assert (instrNeeded >= 1 && instrNeeded <= 4);

  return instrNeeded;
}
