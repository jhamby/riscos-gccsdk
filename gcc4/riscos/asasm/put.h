/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2004-2010 GCCSDK Developers
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
 * put.h
 */

#ifndef put_header_included
#define put_header_included

#include "global.h"

void putData (size_t size, ARMWord data);
void Put_DataWithOffset (size_t offset, size_t size, ARMWord data);
void putDataFloat (size_t size, ARMFloat data);
void putIns (ARMWord ins);
void Put_InsWithOffset (size_t offset, ARMWord data);

ARMWord GetWord (size_t offset);
void PutWord (size_t offset, ARMWord val);

#endif
