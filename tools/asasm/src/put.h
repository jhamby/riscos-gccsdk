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

#ifndef put_header_included
#define put_header_included

#include <stdbool.h>
#include <stdint.h>

#include "global.h"

void Put_Data (unsigned size, ARMWord data);
void Put_AlignDataWithOffset (uint32_t offset, unsigned size, ARMWord data,
			      uint32_t times, bool alignBefore);
void Put_FloatDataWithOffset (uint32_t offset, unsigned size, ARMFloat data, bool alignBefore);
void Put_Ins (unsigned size, ARMWord ins);
void Put_InsWithOffset (uint32_t offset, unsigned size, ARMWord data);

void Put_Ins_MOVW_MOVT (uint32_t cc, uint32_t destReg, uint32_t value, bool isMOVT);

ARMWord Put_GetWord (uint32_t offset);

#endif
