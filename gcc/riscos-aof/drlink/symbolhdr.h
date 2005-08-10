/*
** Drlink AOF linker
**
** Copyright (c) 1993, 1994, 1995, 1996, 1997, 1998  David Daniels
** Copyright (c) 2001, 2002, 2003, 2004, 2005  GCCSDK Developers
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
**
** This file contains all the definitions concerned with symbols
*/

#ifndef symbolhdr_h
#define symbolhdr_h

#include "areahdr.h"

/* OBJ_SYMT symbol table attribute bits */

#define SYM_LOCAL   0x01	/* Symbol defined here, only accessible here */
#define SYM_EXTERN  0x02	/* Symbol defined externally */
#define SYM_GLOBAL  0x03	/* Symbol defined here, accessibly anywhere */
#define SYM_ABSVAL  0x04	/* Symbol has an absolute value i.e. a constant (only for SYM_LOCAL or SYM_GLOBAL) */
#define SYM_IGNCASE 0x08	/* Ignore case when matching symbol (only for SYM_EXTERN) */
#define SYM_WEAKREF 0x10	/* Symbol reference may remain unresolved (only for SYM_EXTERN) */
#define SYM_STRONG  0x20	/* Symbol definition is 'strong' (only for SYM_GLOBAL) */
#define SYM_COMMON  0x40	/* Symbol is a common area reference (only for SYM_EXTERN) */
/* Bit 7 (value 1<<7) is reserved and always set to 0. */

/* Following are the extra AOF version 3 symbol attributes */

#define SYM_CADATUM 0x100	/* Code area data attribute */
#define SYM_FPARGS  0x200	/* FP args in FP registers */
/* Bit 10 (value 1<<10) is reserved and always set to 0. */
#define SYM_LEAF    0x800	/* Simple leaf function */
#define SYM_THUMB   0x1000	/* Identifies Thumb code, instead of ARM code */
/* Bits 13-31 are reserved and are always set to 0. */

/* Special masks and attributes */

#define SYM_SCOPE   0x03	/* Mask to say what kind of symbol this is */
#define SYM_DEFN    0x01	/* Mask to check if this is a symbol definition */
#define SYM_STRONGDEF (SYM_DEFN | SYM_STRONG)	/* Definition of strong symbol */
#define SYM_LINKDEF 0x80000000	/* Marks symbol as linker defined */

#define SYM_ATMASK 0x067	/* Mask to extract interesting symbol attributes */
#define SYM_UNSUPATTR 0xFFFFF680	/* Mask containing all unsupported attributes. */

/* Relocation type bits */

#define FTMASK   0x03		/* 'Field type' mask */
#define REL_BYTE 0x00		/* Byte relocation */
#define REL_HALF 0x01		/* Half-word relocation */
#define REL_WORD 0x02		/* Word relocation */
#define REL_SEQ  0x03		/* Instruction sequence */
#define REL_ABS  0x00		/* Absolute (additive) relocation */
#define REL_PC   0x04		/* PC-relative relocation */
#define REL_INT  0x00		/* Internal relocation */
#define REL_SYM  0x08		/* Symbol relocation */

/* AOF version 3 relocation bits */

#define REL_BASED  0x10		/* Based relocation */
#define REL_IIMASK 0x60		/* 'II' field mask for instruction sequence relocations */
#define REL_A3ATTR 0x70		/* Mask for AOF 3 relocations */

#define REL_IISHIFT 5		/* Register shift to move 'II' count to bottom of register */

/* Others */

#define REL_AOFMASK 0x73	/* Mask used when creating part-linked AOF relocations */
#define REL_TYPE2 0x80000000	/* Type 2 relocation flag */

/* Entry in OBJ_SYMT chunk */
typedef struct symtentry
{
  const char *symtname;		/* Offset of symbol's name in OBJ_STRT chunk */
  unsigned int symtattr;	/* Symbol attributes */
  unsigned int symtvalue;	/* Symbol's value */
  union
  {
    unsigned int areaname;	/* Offset in OBJ_STRT chunk of name of symbol's area */
    struct arealist *areaptr;	/* Address of symbol's area's arealist entry */
    struct symtentry *symdefptr;	/* Pointer to symbol definition's SYMT entry */
  } symtarea;
} symtentry;

/* Symbol table entry */
typedef struct symbol
{
  struct symbol *left;		/* Left in binary tree */
  struct symbol *right;		/* Right in binary tree */
  int symhash;			/* Hash value of symbol's name */
  struct symtentry *symtptr;	/* Pointer to entry in OBJ_SYMT chunk */
  struct symtentry *symnormal;	/* Pointer to 'normal' version of symbol when a strong def'n exists */
} symbol;

typedef symbol *symtable[MAXLOCALS];	/* Local symbol hash table */

/* Relocation info layout in OBJ_AREA chunk */
typedef struct relocation
{
  unsigned int reloffset;	/* Offset in area of relocation */
  unsigned int reltypesym;	/* Relocation type and symbol index in OBJ_SYMT area */
} relocation;

typedef unsigned int indextable[1];

extern symbol *globalsyms[MAXGLOBALS];	/* Global symbol table */

extern symbol *image_robase,	/* Symbol table entries of pre-defined symbols */
 *image_rwbase, *image_zibase, *image_rolimit, *image_rwlimit, *image_zilimit, *image_codebase,	/* Old symbols used by Fortran 77 */
 *image_codelimit, *image_database, *image_datalimit, *reloc_code;

extern symtentry *current_symtbase,	/* Start of OBJ_SYMT chunk in current file containing references */
 *current_symtend;			/* End of OBJ_SYMT chunk in current file */

extern symtentry *symtbase;	/* Address of current OBJ_SYMT chunk */

extern unsigned int totalsymbols,	/* Total number of symbols defined in program */
  numwanted,				/* Number of symbols currently wanted */
  numfound,				/* Number of symbols found this library pass */
  lldsize;				/* Space required for names in low-level debug table */

#endif
