/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2002-2012 GCCSDK Developers
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
 * decode.h
 */

#ifndef decode_header_included
#define decode_header_included

#include "lex.h"

/* Return type used to decide if and which type of $d, $t and $a mapping
   symbols needs to be defined.  */
typedef enum
{
  eRslt_NotRecognized = 0, /* In a partial match callback, this is returned
    when the partial match code fails to match the rest of the
    directive/mnemonic.  */
  eRslt_ARM   = 1, /* ARM instruction. Define $a mapping symbol.  */
  eRslt_Data  = 2, /* Data. Define $d mapping symbol. */
  eRslt_Thumb = 3, /* Thumb, Thumb-2, ThumbEE instruction. Define $t mapping
    symbol */
  eRslt_None  = 4  /* No mapping symbol define to be done.  */
} Rslt_e;

void decode (const Lex *label);
void decode_finalcheck (void);

#endif
