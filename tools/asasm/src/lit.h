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

#ifndef lit_header_included
#define lit_header_included

#include <stdbool.h>

#include "state.h"
#include "value.h"

typedef enum
{
  eLitIntUByte,		/* Unsigned byte loading.  */
  eLitIntSByte,		/* Signed byte loading.  */
  eLitIntUHalfWord,	/* Unsigned half word loading.  */
  eLitIntSHalfWord,	/* Signed half word loading.  */
  eLitIntWord,
  /* FIXME: eLitIntDWord, */	/* Double word loading.  */
  eLitFloat,
  eLitDouble
} Lit_eSize;

typedef enum
{
  /* Not supported cases for literals:
       - Thumb LDRB.
       - LDR{B}T  */
  eLitAddr2,		/**< One variant of Addressing Mode 2 : [PC, #+/-<offset_12>]
    To be used for :
      - ARM LDR{B}.
      - Thumb2 LDR{B},
      - Thumb2 LDR{H|SH|SB} (*not* LDRD)
    Encoding:
      - ARM : bits 11-0, bit 23 is up bit
      - Thumb2 : bits 11-0@1, bit 7@0 is up bit  */
  eLitAddr3,		/**< One variant of Addressing Mode 3 : [PC, #+/-<offset_8>]
    To be used for :
      - ARM LDR{H|SH|SB|D}.
    Encoding:
      - ARM : bits 3-0@1 is lower 4 bits, bits 11-8 is higher 4 bits, bit 23 is up bit.  */
  eLitAddr5,		/**< One variant of Addressing Mode 5 : [PC, #+/-<offset_8>*4]
    To be used for :
      - ARM LDC{2}
      - Thumb2 LDC{2}
      - Thumb2 LDRD.
    Encoding:
      - ARM : bits 7-0, bit 23 is up bit
      - Thumb2 : bits 7-0@1, bit 7@0 is up bit  */
  eLitFormat3,     /**< One variant of Thumb Addressing Mode : [PC, #<offset_8>*4]
    To be used for ;
      - Thumb LDR
    Encoding:
      - Thumb : bits 7-0.  */
} Lit_eAddrType;

void Lit_RemoveLiterals (Symbol *areaSymbolP);
Value Lit_RegisterInt (const Value *valueP, Lit_eSize size,
                       Lit_eAddrType addrType, InstrType_e instrType);
Value Lit_RegisterFloat (const Value *valueP, Lit_eSize size,
                         Lit_eAddrType addrType, InstrType_e instrType);
void Lit_DumpPool (void);

bool c_ltorg (void);

#endif
