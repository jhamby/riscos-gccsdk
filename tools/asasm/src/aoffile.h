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

#ifndef aoffile_header_included
#define aoffile_header_included

#include <stdint.h>

typedef struct
{
  uint32_t Name;		/* Offset into string table */
  uint32_t Type;
  uint32_t Size;		/* Size of area, must be divisible by four */
  uint32_t noRelocations;	/* Size of relocation table */
  uint32_t BaseAddr;		/* Base address or 0.  */
} AofEntry;

#define AofHeaderID 0xc5e2d080

typedef struct
{
  uint32_t Type;	/* 0xc5e2d080 if relocatable object format       */
  uint32_t Version;	/* 1.xx -> 150  2.xx -> 200                      */
  uint32_t noAreas;	/* size of Area[]                                */
  uint32_t noSymbols;	/* size of Symbol Table if such exist            */
  uint32_t EntryArea;	/* Where to start execution, 0 no entry          */
  uint32_t EntryOffset;	/* otherwise start at Area[EntryArea]+AreaOffset */
} AofHeader;

/* Type 1 relocation directive.  */
#define HOW1_INIT     0x00000000u /* Top 12 bits are 0.  */
#define HOW1_BYTE     0x00000000u /* Field to be relocated is a byte.  */
#define HOW1_HALF     0x00010000u /* Field to be relocated is a half word.  */
#define HOW1_WORD     0x00020000u /* Field to be relocated is a word.  */
#define HOW1_RELATIVE 0x00040000u
#define HOW1_SYMBOL   0x00080000u /* Only used if not HOW1_RELATIVE */
#define HOW1_SIDMASK  0x0000ffffu /* Only used if HOW1_SYMBOL */

/* Type 2 relocation directive.  */
#define HOW2_INIT         0x80000000u
#define HOW2_BYTE         0x00000000u /* Field to be relocated is a byte.  */
#define HOW2_HALF         0x01000000u /* Field to be relocated is a half word.  */
#define HOW2_WORD         0x02000000u /* Field to be relocated is a word.  */
#define HOW2_INSTR_UNLIM  (0x03000000u | 0x00000000u) /* Field to be relocated is an unlimited instruction sequence.  */
#define HOW2_INSTR_MAX1   (0x03000000u | 0x20000000u) /* Field to be relocated is one instruction.  */
#define HOW2_INSTR_MAX2   (0x03000000u | 0x40000000u) /* Field to be relocated is an at most 2 instruction sequence.  */
#define HOW2_INSTR_MAX3   (0x03000000u | 0x60000000u) /* Field to be relocated is an at most 3 instruction sequence.  */
#define HOW2_RELATIVE     0x04000000u /* PC-relative.  */
#define HOW2_SYMBOL       0x08000000 /* The 'A' bit. Determines SID type. */
#define HOW2_BASED        0x10000000 /* Based area.  */
#define HOW2_SIDMASK      0x00ffffff /* Symbol offset if HOW2_SYMBOL, otherwise area number */

typedef struct
{
  uint32_t Offset;	/* Offset in area of the field to be relocated */
  uint32_t How;		/* How relocation is done */
} AofReloc;

#define TYPE_LOCAL     0x01 /* Defined with local scope */
#define TYPE_REFERENCE 0x02
#define TYPE_GLOBAL    0x03 /* Defined with global scope */
#define TYPE_KIND      0x03
#define TYPE_DEFINE    0x01
#define TYPE_EXPORT    0x02
#define TYPE_ABSOLUTE  0x04 /* This is a constant, (not valid if TYPE_REFERENCE) */
#define TYPE_NOCASE    0x08 /* Only if TYPE_REFERENCE, case insensitive */
#define TYPE_WEAK      0x10 /* Only if TYPE_REFERENCE, must not be resolved */
#define TYPE_STRONG    0x20 /* Complicated ??? */
#define TYPE_COMMON    0x40

typedef struct
{
  uint32_t Name;	/* Offset in string Table */
  uint32_t Type;
  uint32_t Value;	/* Value if constant, Size if common, otherwise Offset */
  uint32_t AreaName;
} AofSymbol;

#endif
