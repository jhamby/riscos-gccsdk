/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2003-2010 GCCSDK Developers
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

#include "aoffile.h"
#include "symbol.h"
#include "value.h"

typedef struct RELOC
{
  struct RELOC *next;
  AofReloc reloc;
  Value value;		/**< ValueSymbol or ValueCode.   */
} Reloc;

int relocFix (const Symbol *area);
void relocAOFOutput (FILE *outfile, const Symbol *area);
#ifndef NO_ELF_SUPPORT
void relocELFOutput (FILE *outfile, const Symbol *area);
#endif

Reloc *Reloc_Create (uint32_t how, uint32_t offset, const Value *value);

#endif
