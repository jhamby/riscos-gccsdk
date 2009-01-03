/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2001-2006 GCCSDK Developers
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
 * aoffile.h
 * Types that exist in an AOF file.
 */

#ifndef aoffile_header_included
#define aoffile_header_included

typedef struct
{
  unsigned int Name;		/* Offset into string table */
  unsigned int Type;
  unsigned int Size;		/* Size of area, must be divisible by four */
  unsigned int noRelocations;	/* Size of relocation table */
  unsigned int Unused;		/* Unused, must be zero */
}
AofEntry;

#define AofHeaderID 0xc5e2d080

typedef struct
{
  unsigned int Type;		/* 0xc5e2d080 if relocatable object format       */
  unsigned int Version;		/* 1.xx -> 150  2.xx -> 200                      */
  unsigned int noAreas;		/* size of Area[]                                */
  unsigned int noSymbols;		/* size of Symbol Table if such exist            */
  unsigned int EntryArea;		/* Where to start execution, 0 no entry          */
  unsigned int EntryOffset;		/* otherwise start at Area[EntryArea]+AreaOffset */
}
AofHeader;

#define aofHeaderSize(max) \
        (sizeof (AofHeader) + \
        (max - 1) * sizeof(AofEntry))

/* Type 1 relocation directive.  */
#define HOW1_INIT     0x00000000
#define HOW1_BYTE     0x00000000
#define HOW1_HALF     0x00010000
#define HOW1_WORD     0x00020000
#define HOW1_SIZE     0x00030000

#define HOW1_RELATIVE 0x00040000
/* Only used if not HOW1_RELATIVE */
#define HOW1_SYMBOL   0x00080000
/* Only used if HOW1_SYMBOL */
#define HOW1_SIDMASK  0x0000ffff

/* Type 2 relocation directive.  */
#define HOW_TYPE2     0x80000000
#define HOW2_INIT     0x80000000
#define HOW2_BYTE     0x00000000
#define HOW2_HALF     0x01000000
#define HOW2_WORD     0x02000000
#define HOW2_SIZE     0x03000000

#define HOW2_RELATIVE 0x04000000
/* Only used if not HOW2_RELATIVE */
#define HOW2_SYMBOL   0x08000000
/* Symbol offset if HOW2_SYMBOL, otherwise area number */
#define HOW2_SIDMASK  0x00ffffff

/* Type 3 relocation directive.  */
#define HOW_TYPE3     (1U << 31)
#define HOW3_INIT     (1U << 31)
/* Field to be relocated is a byte.  */
#define HOW3_BYTE     (0 << 24)
/* Field to be relocated is a half word.  */
#define HOW3_HALF     (1 << 24)
/* Field to be relocated is a word.  */
#define HOW3_WORD     (2 << 24)
/* Field to be relocated is an instruction sequence.  */
#define HOW3_INSTR    (3 << 24)
#define HOW3_SIZE     0x03000000

/* PC-relative.  */
#define HOW3_RELATIVE (1 << 26)
/* Based area.  */
#define HOW3_BASED    (1 << 28)
/* The 'A' bit. Determines SID type.  */
#define HOW3_SYMBOL   (1 << 27)
/* Symbol offset if HOW3_SYMBOL, otherwise area number */
#define HOW3_SIDMASK  0x00ffffff

typedef struct
{
  unsigned int Offset;		/* Offset in area of the field to be relocated */
  unsigned int How;		/* How relocation is done */
}
AofReloc;


/* Defined with local scope */
#define TYPE_LOCAL     0x01
#define TYPE_REFERENCE 0x02
/* Defined with global scope */
#define TYPE_GLOBAL    0x03
#define TYPE_KIND      0x03
#define TYPE_DEFINE    0x01
#define TYPE_EXPORT    0x02

/* This is a constant, (not valid if TYPE_REFERENCE) */
#define TYPE_ABSOLUTE  0x04
/* Only if TYPE_REFERENCE, case insesitive */
#define TYPE_NOCASE    0x08
/* Only if TYPE_REFERENCE, must not be resolved */
#define TYPE_WEAK      0x10
/* Complicated ??? */
#define TYPE_STRONG    0x20
#define TYPE_COMMON    0x40

typedef struct
{
  unsigned int Name;		/* Offset in string Table */
  unsigned int Type;
  unsigned int Value;		/* Value if constant, Size if common, otherwise Offset */
  unsigned int AreaName;
}
AofSymbol;


#endif
