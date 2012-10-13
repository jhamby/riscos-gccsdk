/*
 * defines types used in decaof
 *
 * Copyright (c) 1992 Andy Duplain, andy.duplain@dsl.pipex.com
 * Copyright (c) 2005-2012 GCCSDK Developers
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#ifndef DECAOF_HEADER_INCLUDED
#define DECAOF_HEADER_INCLUDED

#include <stdint.h>

/*
 * "Word" must be a 4-byte type.
 * "Halfword" must be a 2-byte type.
 * "Byte" must be a 1-byte type.
 */
typedef uint32_t Word;
typedef uint16_t Halfword;
typedef uint8_t Byte;

/*
 * stream error enumeration
 */
typedef enum
{
  FNOERR,
  FEND,
  FRWERR
} Ferror;

/*
 * Chunk file / AOF structures
 */
struct chunkhdr
{
  Word chunkfileid;
  Word maxchunks;
  Word numchunks;
};

struct chunkent
{
  char chunkid[8];
  Word offset;
  Word size;
};

struct aofhdr
{				/* fixed part of AOF header only */
  Word filetype;
  Word version;
  Word numareas;
  Word numsyms;
  Word entryarea;
  Word entryoffset;
};

#ifdef WORDS_BIGENDIAN
struct areahdr_flags
{
  Halfword zeroes;
  Byte AT;
  Byte AL;
};
#else
struct areahdr_flags
{
  Byte AL;
  Byte AT;
  Halfword zeroes;
};
#endif

struct areahdr
{
  Word name;
  Word flags;
  Word size;
  Word numrelocs;
  Word baseaddr;
};

/* Area attributes.  */
/* Lowest 8 bits encode the alignment of the start of the area as a power
   of 2 and has a value between 2 and 32.  */
#define AREA_ALIGN_MASK		0x000000FF
#define AREA_ABS		0x00000100
#define AREA_CODE		0x00000200
#define AREA_COMMONDEF		0x00000400 /* Common block definition */
#define AREA_COMMONREF		0x00000800 /* Common block reference */
#define AREA_UDATA		0x00001000 /* Uninitialised (0-initialised) */
#define AREA_READONLY		0x00002000
#define AREA_PIC		0x00004000
#define AREA_DEBUG		0x00008000
#define AREA_32BITAPCS		0x00010000 /* Code area only */
#define AREA_REENTRANT		0x00020000 /* Code area only */
#define AREA_EXTFPSET		0x00040000 /* Code area only */
#define AREA_NOSWSTACKCHECK	0x00080000 /* Code area only */
#define AREA_THUMB		0x00100000 /* Code area only, same value as AREA_BASED.  */
#define AREA_HALFWORD		0x00200000 /* Code area only, same value as AREA_STUBDATA.  */
#define AREA_INTERWORK		0x00400000 /* Code area only */
#define AREA_BASED		0x00100000 /* Data area only, same value as AREA_THUMB.  */
#define AREA_STUBDATA		0x00200000 /* Data area only, same value as AREA_HALFWORD.  */
#define AREA_RESERVED23		0x00800000
#define AREA_MASKBASEREG	0x0F000000 /* Base reg, data area only */
#define AREA_RESERVED28		0x10000000
#define AREA_RESERVED29		0x20000000
#define AREA_RESERVED30		0x40000000
#define AREA_VFP		0x80000000

/* New since DDE Rel 21 

   Area attribute bit 31 is valid for both code and data areas, and when
   set, indicates:
   * double-precision floating point data in the area is stored using VFP
     rules (endianness matches the rest of the file) rather than FPA rules
     (most-significant word always stored first).
   * for code areas in little-endian mode, that when functions in the area
     receive double-precision arguments and/or return a double-precision
     value in integer registers or on the stack, that the lower register
     number or lower stack address holds the least significant word
     (little-endian VFP compatibilty mode). Otherwise (big-endian mode or
     FPA compatibility mode), the lower register or lower stack address
     holds the most-significant word.
   An area with this attribute cannot be linked with an area which lacks
   the attribute.  */

#define AREA_INT_DATAMASK	0x8F30FFFF /* Mask for DATA areas.  */
#define AREA_INT_CODEMASK	0xC7FFFFFF /* Mask for CODE areas.  */

/* Symbol attributes.  */
#define SYMBOL_LOCAL     0x0001	/* Defined with local scope */
#define SYMBOL_REFERENCE 0x0002 /* E.g. IMPORT symbol, or undefined EXPORT symbol.  */
#define SYMBOL_GLOBAL    0x0003	/* Defined with global scope.  E.g. defined EXPORT symbol.  */

#define SYMBOL_KIND(x)   ((x) & 0x0003) /* Returns: 0 (area symbol (SYMBOL_AREA is set) or symbols which are not marked as imported nor exported), SYMBOL_LOCAL, SYMBOL_REFERENCE or SYMBOL_GLOBAL.  */

#define SYMBOL_DEFINED   0x0001 /* This is a mask.  */
#define SYMBOL_EXPORT    0x0002 /* This is a mask.  */

#define SYMBOL_ABSOLUTE  0x0004	/* This is an absolute value (e.g. constant) and only valid when SYMBOL_DEFINED is set.  Otherwise its value is relative to the base AREA where it is defined. */
#define SYMBOL_NOCASE    0x0008	/* Only if SYMBOL_REFERENCE, case insensitive */
#define SYMBOL_WEAK      0x0010	/* Only if SYMBOL_REFERENCE, must not be resolved (AOF).  For ELF, can be on SYMBOL_LOCAL/SYMBOL_GLOBAL.  */
#define SYMBOL_STRONG    0x0020	/* Only if SYMBOL_GLOBAL.  This attribute 
  only has meaning if there is a non-strong, external definition of the same 
  symbol in another object file. In this case, references to the symbol from 
  outside of the file containing the strong definition, resolve to the strong 
  definition, while those within the file containing the strong definition 
  resolve to the non-strong definition.  */
#define SYMBOL_COMMON    0x0040 /* Only if SYMBOL_REFERENCE.  If set, the
  symbol is a reference to a common area with the symbol's name. The length of
  the common area is given by the symbol's <Value> field (see above).  */

#define SYMBOL_DATUM     0x0100	/* Set for data symbol defined in code area.  */
#define SYMBOL_FPREGARGS 0x0200	/* FP args in FP regs attribute, for code symbols only.  */
#define SYMBOL_USESSB    0x0400
#define SYMBOL_LEAF      0x0800	/* Leaf function, for code symbol only.  */
#define SYMBOL_THUMB     0x1000	/* Identifies Thumb code, instead of ARM code. For code symbol only.  */

/* New since DDE Rel 21:

   Symbol attribute bit 13 is meaningful only for function entry point
   symbols in code areas (or for absolute function entry point symbols).
   It declares that the function not only receives its floating point
   arguments in integer registers, but also returns any floating point
   return value in integer registers. A symbolic reference with this
   attribute cannot be matched by the linker to a symbol definition which
   lacks the attribute.  */
#define SYMBOL_SOFTFP    0x2000

#define SYMBOL_SUPPORTED_AOF_BITS 0x3F7F /** Mask for the symbol bits which are AOF defined symbols bits and can be passed on in the AOF output file.  */

struct reloc
{
  Word offset;
  Word flags;			/* varies depending on type */
};

struct symbol
{
  Word name;
  Word flags;
  Word value;
  Word areaname;
};


#endif /* DECAOF_HEADER_INCLUDED */
