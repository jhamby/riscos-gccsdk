/*
** Drlink AOF linker
**
** Copyright © David Daniels 1993, 1994, 1995, 1996, 1997, 1998.
** All rights reserved.
**
** This file contains all the definitions concerned with symbols
*/

#ifndef symbolhdr_h
#define symbolhdr_h

#include "areahdr.h"

/* OBJ_SYMT symbol table attribute bits */

#define SYM_LOCAL   0x01		/* Symbol defined here, only accessible here */
#define SYM_EXTERN  0x02		/* Symbol defined externally */
#define SYM_GLOBAL  0x03		/* Symbol defined here, accessibly anywhere */
#define SYM_ABSVAL  0x04		/* Symbol has an absolute value i.e. a constant */
#define SYM_IGNCASE 0x08		/* Ignore case when matching symbol */
#define SYM_WEAKREF 0x10		/* Symbol reference may remain unresolved */
#define SYM_STRONG  0x20		/* Symbol definition is 'strong' */
#define SYM_COMMON  0x40		/* Symbol is a common area reference */

/* Following are the extra AOF version 3 symbol attributes */

#define SYM_CADATUM 0x100		/* Code area data attribute */
#define SYM_FPARGS  0x200		/* FP args in FP registers */
#define SYM_LEAF    0x800		/* Simple leaf function */

/* Special masks and attributes */

#define SYM_SCOPE   0x03		/* Mask to say what kind of symbol this is */
#define SYM_DEFN    0x01		/* Mask to check if this is a symbol definition */
#define SYM_STRONGDEF (SYM_DEFN | SYM_STRONG)	/* Definition of strong symbol */
#define SYM_LINKDEF 0x80000000		/* Marks symbol as linker defined */

#define SYM_ATMASK 0x067		/* Mask to extract interesting symbol attributes */
#define SYM_A3ATTR 0x700		/* Mask to extract unsupported AOF 3 symbol attributes */

/* Relocation type bits */

#define FTMASK   0x03			/* 'Field type' mask */
#define REL_BYTE 0x00			/* Byte relocation */
#define REL_HALF 0x01			/* Half-word relocation */
#define REL_WORD 0x02			/* Word relocation */
#define REL_SEQ  0x03			/* Instruction sequence */
#define REL_ABS  0x00			/* Absolute (additive) relocation */
#define REL_PC   0x04			/* PC-relative relocation */
#define REL_INT  0x00			/* Internal relocation */
#define REL_SYM  0x08			/* Symbol relocation */

/* AOF version 3 relocation bits */

#define REL_BASED  0x10			/* Based relocation */
#define REL_IIMASK 0x60			/* 'II' field mask for instruction sequence relocations */
#define REL_A3ATTR 0x70			/* Mask for AOF 3 relocations */

#define REL_IISHIFT 5			/* Register shift to move 'II' count to bottom of register */

/* Others */

#define REL_AOFMASK 0x73		/* Mask used when creating part-linked AOF relocations */
#define REL_TYPE2 0x80000000		/* Type 2 relocation flag */

typedef struct symtentry {		/* Entry in OBJ_SYMT chunk */
  char *symtname;			/* Offset of symbol's name in OBJ_STRT chunk */
  unsigned int symtattr;		/* Symbol attributes */
  unsigned int symtvalue;		/* Symbol's value */
  union {
    unsigned int areaname;		/* Offset in OBJ_STRT chunk of name of symbol's area */
    struct arealist *areaptr;		/* Address of symbol's area's arealist entry */
    struct symtentry *symdefptr;	/* Pointer to symbol definition's SYMT entry */
  } symtarea;
} symtentry;

typedef struct symbol {			/* Symbol table entry */
  int symhash;				/* Hash value of symbol's name */
  struct symtentry *symtptr;		/* Pointer to entry in OBJ_SYMT chunk */
  struct symtentry *symnormal;		/* Pointer to 'normal' version of symbol when a strong def'n exists*/
  struct symbol *symflink;		/* Pointer to next symbol */
} symbol;

typedef symbol *symtable[MAXLOCALS];	/* Local symbol hash table */

typedef struct relocation {		/* Relocation info layout in OBJ_AREA chunk */
  unsigned int reloffset;		/* Offset in area of relocation */
  unsigned int reltypesym;		/* Relocation type and symbol index in OBJ_SYMT area */
} relocation;

typedef struct libentry {		/* Library symbol table entry */
  int libhash;				/* Hash value of library entry name */
  char *libname;			/* Pointer to library entry name */
  char *libmember;			/* Pointer to name of entry's library member */
  unsigned int liboffset;		/* Offset in library of entry's LIB_DATA chunk */
  unsigned int libsize;			/* Size of LIB_DATA chunk */
  struct libentry *libflink;		/* Pointer to next library entry */
} libentry;

typedef libentry *libtable[MAXENTRIES];	/* Library symbol hash table */

typedef struct libheader {		/* Describes a library */
  char *libname;			/* Name of library */
  unsigned int *libase;			/* Address of library in memory */
  unsigned int libextent;		/* Size of library */
  bool libgotsyms;			/* TRUE if library symbol table has been read */
  bool libvalid;			/* TRUE if library's chunk header has already been validated */
  libtable librarysyms;			/* Library's symbol table */
  struct libheader *libflink;		/* Pointer to next library in list */
} libheader;

typedef unsigned int indextable[1];

extern symbol *globalsyms[MAXGLOBALS];	/* Global symbol table */

extern symbol
  *image_robase,			/* Symbol table entries of pre-defined symbols */
  *image_rwbase,
  *image_zibase,
  *image_rolimit,
  *image_rwlimit,
  *image_zilimit,
  *image_codebase,			/* Old symbols used by Fortran 77 */
  *image_codelimit,
  *image_database,
  *image_datalimit,
  *reloc_code;

extern libtable *current_libsyms;	/* Pointer to current library symbol table */
extern libheader *current_lib;		/* Pointer to libheader entry for library being searched */

extern symtentry
  *current_symtbase,			/* Start of OBJ_SYMT chunk in current file containing references */
  *current_symtend;			/* End of OBJ_SYMT chunk in current file */

extern unsigned int
  totalsymbols,				/* Total number of symbols defined in program */
  numwanted,				/* Number of symbols currently wanted */
  numfound,				/* Number of symbols found this library pass */
  lldsize;				/* Space required for names in low-level debug table */

#endif
