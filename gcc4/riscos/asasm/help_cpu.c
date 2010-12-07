/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2001-2010 GCCSDK Developers
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
 * help_cpu.c
 */

#include <assert.h>
#include <stdbool.h>

#include "error.h"
#include "help_cpu.h"

static const ARMWord mask[] =
{
  0x000000ff, 0xc000003f, 0xf000000f, 0xfc000003,
  0xff000000, 0x3fc00000, 0x0ff00000, 0x03fc0000,
  0x00ff0000, 0x003fc000, 0x000ff000, 0x0003fc00,
  0x0000ff00, 0x00003fc0, 0x00000ff0, 0x000003fc
};

int
help_cpuImm8s4 (int value)
{
  for (int i = 0; i < 16; i++)
    {
      if (((ARMWord) value & mask[i]) == (ARMWord) value)	/* hittat mask */
	return ((((ARMWord) value >> ((16 - i) * 2)) | value << (i * 2)) & 0xff) | i << 8;
    }
  return -1;
}

static inline unsigned int
ShiftL (unsigned int val, unsigned int shiftPos)
{
  assert (shiftPos < 32);
  return (val << shiftPos) | val >> (32 - shiftPos);
}

/**
 * \return Returns the minimal number of Imm8s4 constants needed to represent
 * the given constant.
 */
int
Help_SplitByImm8s4 (unsigned int cnst, unsigned int *c0, unsigned int *c1,
		    unsigned int *c2, unsigned int *c3)
{
  /* Check the odd case.  */
  if (cnst == 0)
    {
      *c0 = 0;
      return 1;
    }

  /* Find the first byte with one or more bits set at its bottom 2 bit
     positions.  */
  int shift;
  for (shift = 0; (cnst & (0x3<<shift)) == 0; shift += 2)
    /* */;

  /* Find the last shift position we never should consider.  */
  int shiftEnd;
  for (shiftEnd = 32; (cnst & (0x3<<(shiftEnd - 2))) == 0; shiftEnd -= 2)
    /* */;
  /* So all shifts used in a solution are between 'shift' (incl) - 'shiftEnd'
     (excl).  */
  
  /* First combination is: 0xFF<<(shift + 0), 0xFF<<(shift + 2),
     0xFF<<(shift + 4) and 0xFF<<(shift + 6).  */
  int instrNeeded = 5;
  for (int shift0 = shift; shift0 != shift + 8; shift0 += 2)
    {
      /* First mask: */
      unsigned int m0 = ShiftL (0xFF, shift0);
      unsigned int r0 = cnst & ~m0;
      if (r0 == 0)
	{
	  *c0 = cnst & m0;
	  return 1; /* Best solution possible, bail out.  */
	}
      /* Figure out how many bytes we minimally need to mask out the
         remainer r0.  */

      /* Second mask: */
      int shift1;
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
	  *c0 = cnst & m0;
          *c1 = r0 & m1;
          return 2; /* We know 1 instr was not enough, so this is best solution.  */
       }

      /* Third mask: */
      int shift2;
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
	  *c0 = cnst & m0;
          *c1 = r0 & m1;
	  *c2 = r1 & m2;
	  instrNeeded = 3;
	  continue;
	}

      /* Fourth mask (the remaining bits): */
      if (instrNeeded > 4)
	{
	  /* Worst solution.  */
	  *c0 = cnst & m0;
          *c1 = r0 & m1;
	  *c2 = r1 & m2;
	  *c3 = r2;
	  instrNeeded = 4;
	}
    }
  return instrNeeded;
}
