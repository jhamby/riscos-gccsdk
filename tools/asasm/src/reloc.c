/*
 * AsAsm an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2013 GCCSDK Developers
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

#include "config.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif
#include <string.h>

#include "libelf.h"

#include "aoffile.h"
#include "area.h"
#include "error.h"
#include "global.h"
#include "main.h"
#include "phase.h"
#include "reloc.h"
#include "symbol.h"

void
Reloc_InitializeState (Reloc_State_t *stateP)
{
  stateP->numRelocsInt = 0;
  stateP->maxNumRelocsInt = 0;
  stateP->relocIntP = NULL;
  stateP->raw.rawP = NULL;
}


void
Reloc_FinalizeState (Reloc_State_t *stateP)
{
  free (stateP->relocIntP);
  stateP->relocIntP = NULL;
  free (stateP->raw.rawP);
  stateP->raw.rawP = NULL;
}


/**
 * Adds AOF/ELF relocation for the current area.
 * \param how For AOF these are the HOW2_* bits. HOW2_SYMBOL will get assigned
 * automatically. For ELF, this is a R_ARM_* vaule.
 * \param offset Area offset where the relocation needs to happen.
 * \param value A ValueSymbol value.  Its offset will be ignored and needs to
 * be taken into account by the caller.
 */
void
Reloc_CreateInternal (uint32_t how, uint32_t offset, const Value *value)
{
  assert (value->Tag == ValueSymbol && value->Data.Symbol.factor > 0);
  assert (!Area_IsMappingSymbol (value->Data.Symbol.symbol->str));
  assert (gPhase == ePassTwo);

  assert ((option_aof && (how & HOW2_SIDMASK) == 0) || (!option_aof && how < 256u));
  assert (offset < areaCurrentSymbol->area->maxIdx);

  Reloc_State_t *stateP = &areaCurrentSymbol->area->reloc;
  size_t entriesNeeded = value->Data.Symbol.factor;
  if (stateP->numRelocsInt + entriesNeeded > stateP->maxNumRelocsInt)
    {
      size_t newMaxNumRelocsInt = stateP->maxNumRelocsInt ? 2*stateP->maxNumRelocsInt : 16;
      if (stateP->numRelocsInt + entriesNeeded > newMaxNumRelocsInt)
	newMaxNumRelocsInt = stateP->numRelocsInt + entriesNeeded;
      struct RelocInt *newRelocsIntP;
      if ((newRelocsIntP = realloc (stateP->relocIntP, newMaxNumRelocsInt * sizeof (struct RelocInt))) == NULL)
	Error_OutOfMem ();
      stateP->maxNumRelocsInt = newMaxNumRelocsInt;
      stateP->relocIntP = newRelocsIntP;
    }
  while (entriesNeeded--)
    {
      stateP->relocIntP[stateP->numRelocsInt].how = how;
      stateP->relocIntP[stateP->numRelocsInt].offset = offset;
      stateP->relocIntP[stateP->numRelocsInt].symP = value->Data.Symbol.symbol;
      ++stateP->numRelocsInt;
    }

  /* Mark we want this symbol in our output (only for non-area symbols).  */
  if ((value->Data.Symbol.symbol->type & SYMBOL_AREA) == 0)
    value->Data.Symbol.symbol->used = 0;
}

void *
Reloc_GetRawRelocData (Reloc_State_t *stateP)
{
  if (stateP->numRelocsInt == 0)
    return NULL;
  assert (stateP->raw.rawP == NULL);
  if ((stateP->raw.rawP = malloc (Reloc_GetRawRelocSize (stateP))) == NULL)
    Error_OutOfMem ();
  /* FIXME: need to sort based on reloc offset ? Normally it should not be
     needed.  */
  if (option_aof)
    {
      const struct RelocInt *relSrcP = stateP->relocIntP;
      AofReloc *relDstP = stateP->raw.aofP;
      for (size_t idx = 0; idx != stateP->numRelocsInt; ++idx, ++relSrcP, ++relDstP)
	{
	  relDstP->Offset = armword (relSrcP->offset);
	  const Symbol *symP = relSrcP->symP;
	  assert (symP->used >= 0 && (symP->used & ~HOW2_SIDMASK) == 0);
	  uint32_t how = relSrcP->how | symP->used;
	  if ((symP->type & SYMBOL_AREA) == 0)
	    how |= HOW2_SYMBOL;
	  relDstP->How = armword (how);
	}
    }
  else
    {
      const struct RelocInt *relSrcP = stateP->relocIntP;
      Elf32_Rel *relDstP = stateP->raw.elfP;
      for (size_t idx = 0; idx != stateP->numRelocsInt; ++idx, ++relSrcP, ++relDstP)
	{
	  relDstP->r_offset = relSrcP->offset;
	  const Symbol *symP = relSrcP->symP;
	  assert (symP->used >= 0 && (symP->used & ~0xFFu) == 0);
	  relDstP->r_info = ELF32_R_INFO (symP->used, relSrcP->how);
	}
    }
  return stateP->raw.rawP;
}
