/*
 * AsAsm an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2003-2014 GCCSDK Developers
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

#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "libelf.h"

#include "aoffile.h"
#include "main.h"
#include "symbol.h"
#include "value.h"

typedef struct
{
  /* AsAsm internal relocation data.  */
  size_t numRelocsInt;
  size_t maxNumRelocsInt;
  struct RelocInt
    {
      uint32_t how; /* AOF: HOW2_* bits.  ELF: R_ARM_* value.  */
      uint32_t offset; /* Area offset.  */
      Symbol *symP; /* For ELF, this can be NULL (= relocation against
        anonymous symbol, i.e. symbol 0 of the symbol table).  */
    } *relocIntP;
  uint32_t offsetForExplicitReloc; /* When non-UINT32_MAX, RELOC can be used
    to add an explicit relocation (possibly replacing existing relocations).  */

  /* Raw AOF/ELF reloc structures.  */
  union
    {
      void *rawP;
      AofReloc *aofP;
      Elf32_Rel *elfP;
    } raw; 
} Reloc_State_t;

void Reloc_InitializeState (Reloc_State_t *stateP);
void Reloc_FinalizeState (Reloc_State_t *stateP);

typedef struct
{
  Value relocSymbol; /* When ValueSymbol, its offset is guaranteed to be zero.
    Is ValueIllegal when no reloc should be outputed.  */
  Value addend; /* Is ValueIllegal when Symbol_GetLabelAddend failed.
    ValueInt or ValueAddr otherwise.  */
} RelocAndAddend_t;
RelocAndAddend_t Reloc_SplitRelocAndAddend (const Value *valP,
					    const Symbol *pcRelSymP,
					    uint32_t instrOffset,
					    bool intAddendOk);

/**
 * Enable the use of RELOC for given offset.
 */
static inline void
Reloc_EnableExplicitReloc (Reloc_State_t *stateP, uint32_t offset)
{
  stateP->offsetForExplicitReloc = offset;
}

/**
 * Disable the use of RELOC (until Reloc_EnableExplicitReloc is called).
 */
static inline void
Reloc_DisableExplicitReloc (Reloc_State_t *stateP)
{
  stateP->offsetForExplicitReloc = UINT32_MAX;
}

void Reloc_CreateInternal (uint32_t how, uint32_t offset, const Value *value,
                           bool replace);

/**
 * Create AOF relocation.
 */
static inline void
Reloc_CreateAOF (uint32_t how, uint32_t offset, const Value *value)
{
  if (option_aof)
    Reloc_CreateInternal (how, offset, value, false);
}

/**
 * Create ELF relocation.
 */
static inline void
Reloc_CreateELF (uint32_t how, uint32_t offset, const Value *value)
{
  if (!option_aof)
    Reloc_CreateInternal (how, offset, value, false);
}

/**
 * \return Number of relocactions associated with given area's Reloc_State_t
 * object.
 */
static inline size_t
Reloc_GetNumRelocs (const Reloc_State_t *stateP)
{
  return stateP->numRelocsInt;
}

/**
 * \return Size of relocaction AOF/ELF data associated with given area's
 * Reloc_State_t object.
 */
static inline size_t
Reloc_GetRawRelocSize (const Reloc_State_t *stateP)
{
  return stateP->numRelocsInt * (option_aof ? sizeof (AofReloc) : sizeof (Elf32_Rel));
}

void *Reloc_GetRawRelocData (Reloc_State_t *stateP);

bool c_reloc (void);

#endif
