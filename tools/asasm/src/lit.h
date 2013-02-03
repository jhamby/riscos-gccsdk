/*
 * AS an assembler for ARM
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * lit.h
 */

#ifndef lit_header_included
#define lit_header_included

#include <stdbool.h>

#include "value.h"

typedef enum
{
  eLitIntUByte,		/* Unsigned byte loading.  */
  eLitIntSByte,		/* Signed byte loading.  */
  eLitIntUHalfWord,	/* Unsigned half word loading.  */
  eLitIntSHalfWord,	/* Signed half word loading.  */
  eLitIntWord,
  eLitFloat,
  eLitDouble
} Lit_eSize;

Value Lit_RegisterInt (const Value *valueP, Lit_eSize size);
Value Lit_RegisterFloat (const Value *valueP, Lit_eSize size);
void Lit_DumpPool (void);

bool c_ltorg (void);

#endif
