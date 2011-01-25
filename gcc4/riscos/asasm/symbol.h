/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2001-2011 GCCSDK Developers
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
 * symbol.h
 */

#ifndef symbol_header_included
#define symbol_header_included

#include <stdio.h>

#include "lex.h"
#include "value.h"

#define SYMBOL_LOCAL     0x0001	/* Defined with local scope */
#define SYMBOL_REFERENCE 0x0002 /* E.g. IMPORT symbol, or unused EXPORT symbol.  */
#define SYMBOL_GLOBAL    0x0003	/* Defined with global scope.  E.g. used EXPORT symbol.  */

#define SYMBOL_KIND(x)   ((x) & 0x0003) /* Returns: 0 (area symbol, symbols which are only marked as imported or exported), SYMBOL_LOCAL, SYMBOL_REFERENCE or SYMBOL_GLOBAL.  */

#define SYMBOL_DEFINED   0x0001 /* This is a mask.  */
#define SYMBOL_EXPORT    0x0002 /* This is a mask.  */

#define SYMBOL_ABSOLUTE  0x0004	/* This is an absolute value (e.g. constant) and only valid when SYMBOL_DEFINED is set.  Otherwise its value is relative to the base AREA where it is defined. */
#define SYMBOL_NOCASE    0x0008	/* Only if SYMBOL_REFERENCE, case insensitive */
#define SYMBOL_WEAK      0x0010	/* Only if SYMBOL_REFERENCE, must not be resolved */
#define SYMBOL_STRONG    0x0020	/* Complicated ??? */
#define SYMBOL_COMMON    0x0040

#define SYMBOL_DATUM     0x0100	/* ???, for code symbols only */
#define SYMBOL_FPREGARGS 0x0200 /* FP args in FP regs attribute, for code symbols only */
#define SYMBOL_LEAF      0x0800	/* Leaf function, for code symbol only */
#define SYMBOL_THUMB     0x1000	/* Identifies Thumb code, instead of ARM code */

#define SYMBOL_SUPPORTEDBITS 0x1B7F /** Mask for the symbol bits which are AOF defined symbols bits and can be passed on in the AOF output file.  */

/* The following are 'asasm' internal SYMBOL attribute values only and should
   not be used in the AOF output file.  */

#define SYMBOL_MACRO_LOCAL	0x00010000 /** Set when symbol is only known as
  a local macro variable.  */

#define SYMBOL_KEEP		0x01000000
#define SYMBOL_AREA		0x02000000 /* Symbol is actually an area name. When set, SYMBOL_DECLARED is set as well, but not SYMBOL_DEFINED.  */
#define SYMBOL_NOTRESOLVED	0x04000000

#define SYMBOL_CPUREG		0x10000000
#define SYMBOL_FPUREG		0x20000000
#define SYMBOL_COPREG		0x30000000
#define SYMBOL_COPNUM		0x40000000
#define SYMBOL_GETREGTYPE(x)	((x) & 0x70000000)

#define SYMBOL_DECLARED		0x80000000

#define SYMBOL_TABLESIZE 1024


typedef struct Symbol
{
  struct Symbol *next;  /** Linked symbols all having the same hash value.  */

  unsigned int type; /** Cfr. SYMBOL_* bits.  */
  Value value; /** For AREA symbol, this is its current content size.  */
  size_t codeSize; /** Size of the code associated with this symbol label (for AREA symbol, this is unused).  */
  union
    {
      struct Symbol *rel; /* FIXME: we need to support this better, i.e. this symbol (with ValueInt, ValueAddr) is relative to 'ptr' symbol, i.e. only relevant when SYMBOL_ABSOLUTE is *not* defined.  */
      struct AREA *info; /** When SYMBOL_AREA is set.  */
    } area;

  /* For output: */
  unsigned int offset;	/** Offset in stringtable.  For AOF output you need to add an extra 4, for ELF output you need to add an extra 1.  */
  int used;		/** -1 when not used; for area symbols: at outputAof stage, this will be the area number counted from 0, at outputElf stage, this is the section number; for other symbols, 0 when symbol is needed in output. This will become the symbol index at symbolFix stage.  */

  /* Symbol name: */
  size_t len;		/** length of str[] without its NUL terminator.  */
  char str[1];		/** symbol name as NUL terminated string */
} Symbol;

void symbolInit (void);
Symbol *symbolAdd (const Lex *l);
Symbol *symbolGet (const Lex *l);
Symbol *symbolFind (const Lex *l);
void symbolRemove (const Lex *l);

int symbolFix (int *stringSizeNeeded);
void symbolStringOutput (FILE *outfile);
void symbolSymbolAOFOutput (FILE *outfile);
#ifndef NO_ELF_SUPPORT
void symbolSymbolELFOutput (FILE *outfile);
#endif

bool c_export (void);
bool c_import (void);
bool c_keep (void);
bool c_strong (void);
bool c_exportas (void);

#ifdef DEBUG
void symbolPrint (const Symbol *sym);
void symbolPrintAll (void);
#endif

#endif
