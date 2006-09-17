/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas R�emo
 * Copyright (c) 2003-2006 GCCSDK Developers
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
 * reloc.h
 */

#ifndef reloc_header_included
#define reloc_header_included

#include "symbol.h"
#include "value.h"

typedef enum
{
  RelocShiftImm,		/* An immediate shift constant 0-32 may need patching of shiftop */
  RelocImm8s4,			/* An immediate constant 8 bits rotated by 4 bits */
  RelocImmFloat,		/* An immediate constant float (only 0.0 1.0 2.0, 3.0, 4.0, 5.0, 10.0 or 0.5) */
  RelocBranch,			/* Branch immediate */
  RelocBranchT,			/* Thumb branch immediate */
  RelocSwi,			/* Swi immediate */
  RelocCpuOffset,		/* An offset in ldr/str */
  RelocCopOffset,		/* An offset in ldf/stf */
  RelocAdr,			/* Used in pseudoinstruction adr */
  RelocAdrl,			/* Used in pseudoinstruction adrl */
  RelocImmN,			/* extra = 4: A word constant (linker can handle this) */
  /* extra = 2: A halfword constant, also used in ldm/stm with immediate mask */
  /* extra = 1: A byte constant */
  /* all others are illegal */
  RelocFloat,			/* extra = 8: A double constant */
  /* extra = 4: A float constant */
  /* all others are illegal */
  RelocNone			/* No relocation necessary */
}
RelocTag;

typedef struct RELOC
{
  struct RELOC *more;
  RelocTag Tag;
  long int lineno;		/* For error messages */
  const char *file;		/* ditto */
  int offset;			/* Offset in area */
  WORD extra;			/* e.g. shiftop in ShiftImm, size in RelocImmN, e.t.c. */
  Value value;
}
Reloc;

int relocFix (Symbol * area);
void relocOutput (FILE * outfile, Symbol * area);
void relocElfOutput (FILE * outfile, Symbol * area);

void relocShiftImm (WORD shiftop, Value shift);
void relocImm8s4 (WORD ir, Value im8s4);
void relocImmFloat (WORD ir, Value value);
void relocBranch (Value offset);
void relocBranchT (Value offset);
void relocSwi (Value code);
void relocCopOffset (WORD ir, Value offset);
void relocCpuOffset (WORD ir, Value offset);
void relocAdr (WORD ir, Value addr);
void relocAdrl (WORD ir, Value addr);
void relocMask (Value mask);
void relocInt (int size, Value value);
void relocFloat (int size, Value value);
void relocAdd (Reloc * newReloc);
#endif
