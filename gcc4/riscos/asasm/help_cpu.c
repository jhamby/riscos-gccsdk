/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2001-2008 GCCSDK Developers
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

#include "error.h"
#include "help_cpu.h"

static const WORD mask[] =
{
  0x000000ff, 0xc000003f, 0xf000000f, 0xfc000003,
  0xff000000, 0x3fc00000, 0x0ff00000, 0x03fc0000,
  0x00ff0000, 0x003fc000, 0x000ff000, 0x0003fc00,
  0x0000ff00, 0x00003fc0, 0x00000ff0, 0x000003fc
};

int
help_cpuImm8s4 (int value)
{
  int i;
  for (i = 0; i < 16; i++)
    {
      if (((WORD) value & mask[i]) == (WORD) value)	/* hittat mask */
	return ((((WORD) value >> ((16 - i) * 2)) | value << (i * 2)) & 0xff) | i << 8;
    }
  return -1;
}
