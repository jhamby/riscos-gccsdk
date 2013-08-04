/*
 * AsAsm an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2004-2013 GCCSDK Developers
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

#ifndef fix_header_included
#define fix_header_included

#include <stdbool.h>

#include "global.h"

ARMWord Fix_Imm8s4 (ARMWord ir, int im);

ARMWord Fix_ShiftImm (ARMWord shiftop, int shift);
ARMWord Fix_Int (unsigned dataSize, unsigned dataValue);
ARMWord Fix_CopOffset (ARMWord ir, int offset);
ARMWord Fix_CPUOffset (ARMWord ir, int offset);

bool Fix_CheckForOverflow (unsigned dataSize, uint32_t dataValue);

#endif
