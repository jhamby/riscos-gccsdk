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

#ifndef symbol_header_included
#define symbol_header_included

#include <stdbool.h>

#include "lex.h"
#include "phase.h"
#include "value.h"

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
  the common area is given by the symbol's <Value> field.  */

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

/* The following are 'asasm' internal SYMBOL attribute values only and should
   not be output in the AOF output file.  */

#define SYMBOL_READONLY		0x02000000 /* Imported symbol from a READONLY
  area.  */
#define SYMBOL_MACRO_LOCAL	0x04000000 /** Set when symbol is only known as
  a local macro variable.  */
#define SYMBOL_NO_EXPORT	0x08000000 /** Do not export this symbol
  without any exceptions, even when SYMBOL_KEEP set (offset-to-literal-value
  symbols, local symbols, storage map symbols, bool/string variables,
  register/coprocessor number symbols).  */
#define SYMBOL_CONSTANT		0x10000000 /** Defined by EQU, *, RN, CN, FN, CP, RLIST.  */
#define SYMBOL_RW		0x20000000 /** GBLL, GBLA, GBLS, LBLL, LBLA or LBLS, i.e. a read-write symbol or variable.  */
#define SYMBOL_KEEP		0x40000000 /* Symbol needs to come in output,
  even if it is local.  Not when its value does not allow it.  */
#define SYMBOL_AREA		0x80000000 /* Symbol is an area name.  */

#define SYMBOL_TABLESIZE 1024


typedef struct Symbol
{
  struct Symbol *next;  /** Linked symbols all having the same hash value.  */

  unsigned int type; /** Cfr. SYMBOL_* bits.  */
  Value value; /** Symbol value.  For symbol labels, this is always a ValueSymbol
    with symbol an area symbol.
    For SYMBOL_COMMON symbols, this is the size of the common block it represents.  */
  uint32_t codeSize; /** Size of the code associated with this symbol label
    (for AREA symbol, this is unused).
    For SYMBOL_COMMON symbols, this is also its size.  */
  struct AREA *area; /** Area when SYMBOL_AREA is set, otherwise NULL.  */

  /* First reference, or definition.  */
  const char *fileName;
  unsigned lineNumber;

  unsigned aligned; /** ELF alignment value.  Not supported in AOF.  */
  unsigned visibility:2; /** ELF symbol visibility.  Not supported in AOF.  */
  
  /* For output: */
  uint32_t offset; /** In Symbol_CreateSymbolOut() (called from Output_AOF()
    and Output_ELF()): symbol's offset position in symbol table (for AOF,
    minus 4).
    For ELF, section (area) symbols: is 0.  */
  int used; /** Has several usages:
    At start of Output_AOF()/Output_ELF():
      either -1 (no relocation needed),
      either 0 (relocation needed, see Reloc_CreateInternal()).

    At end of Symbol_CreateSymbolOut(): an index suited for choosen output
      format to refer to this symbol.
      For AOF: For area symbols, this index is the area number.  For non area
	       symbols, this index is an index in the symbol table.
	       From 0 onwards.
      For ELF: Index in the symbol table (area symbols also have an entry there).
	       From 0 onwards.
      Is -1 when the symbol is not to be mentioned in the symbol table.  */

  /* Symbol name: */
  size_t len;		/** length of str[] without its NUL terminator.  */
  char str[];		/** symbol name as NUL terminated string */
} Symbol;

/* Prefix of all internal AsAsm symbols.  */
#define kIntLabelPrefix "$$AsAsm$$Int$$"

void Symbol_PrepareForPhase (Phase_e phase);

Symbol *Symbol_Get (const Lex *l);
Symbol *Symbol_Find (const Lex *l);
void Symbol_Remove (const Lex *l);
void Symbol_RemoveVariables (void);

bool Symbol_Define (Symbol *symbol, unsigned newSymbolType, const Value *newValue);

const char *Symbol_GetDescription (const Symbol *symbol);

typedef struct
{
  unsigned numAllSymbols;
  unsigned numLocalSymbols;
  unsigned strDataSize; /* For AOF : size of OBJ_STRT (incl. its area length); for ELF : size of .strtab section.  */
  void *strDataP; /* For AOF : OBJ_STRT data to write; for ELF : .strtab section data to write.  */
  unsigned symDataSize;
  void *symDataP; /* For AOF : OBJ_SYMT data to write; for ELF : .symtab section data to write.  */
} SymbolOut_t;

SymbolOut_t Symbol_CreateSymbolOut (void);
void Symbol_FreeSymbolOut (SymbolOut_t *symOutP);

void Symbol_MarkToOutput (Symbol *symP);

bool c_common (void);
bool c_export (void);
bool c_import (void);
bool c_keep (void);
bool c_leaf (void);
bool c_strong (void);

#ifdef DEBUG
void Symbol_Print (const Symbol *sym);
void Symbol_PrintAll (void);
#endif

#endif
