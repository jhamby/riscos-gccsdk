/*
 * AsAsm an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2003-2013 GCCSDK Developers
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

#ifndef reloc_header_included
#define reloc_header_included

#include "config.h"

#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "libelf.h"

#include "aoffile.h"
#include "symbol.h"
#include "value.h"

typedef struct RELOC
{
  struct RELOC *next;
  AofReloc reloc;
  Value value;		/**< ValueSymbol.   */
} Reloc;

void Reloc_RemoveRelocs (Symbol *areaSymbolP);

Reloc *Reloc_Create (uint32_t how, uint32_t offset, const Value *value);

bool Reloc_PrepareRelocOutPart1 (const Symbol *area);
void Reloc_PrepareRelocOutPart2 (const Symbol *area);

typedef struct RelocOut
{
  unsigned num; /** Number of relocations to output.  */
  union
    {
      void *rawP;
      AofReloc *aofP;
      Elf32_Rel *elfP;
    } relocs; /* Only accessable after Reloc_PrepareRelocOutPart2().  */ 
  uint32_t size; /** Reloc data size in bytes.  */
} RelocOut;

#endif
