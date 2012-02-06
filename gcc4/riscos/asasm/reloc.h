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

/**
 * Callback for updating instruction/data at given offset in current AREA.
 * Gets called up to two times, the second time only when the first time
 * the return value was true, meaning the valueP could not be used to finalise
 * the mnemonic.
 * The second time calling is happening when the full assembler source got
 * parsed so more ValueSymbol cases seen the first time can be resolved but any
 * remaining ValueSymbol ones need now to be taken into account via a
 * relocation.  When done successful, return false, otherwise return true and
 * this will result in an error.
 * When reporting info/warning/error/abort, always use RelocQueue::file and
 * RelocQueue::lineNum as this can get called after parsing all input files.
 * \param valueP No ownership transfer.
 */
typedef bool (*RelocUpdater)(const char *file, unsigned lineNum, ARMWord offset,
			     const Value *valueP, void *privData, bool final);

typedef struct RelocQueue
{
  struct RelocQueue *next;
  const char *file;	/**< Source filename of instruction/data needing a change.  */
  unsigned lineNum;	/**< Line number of instruction/data needing a change.  */

  void *privData;	/**< Pointer to private data.  */
  
  ARMWord offset;	/**< Area content offset value of start instruction/data needing a change.  */
  Value expr;		/**< Value (Tag: ValueCode) representing the change. */
  ValueTag legal;	/**< Valid value tags we expect after evaluting 'expr'.  */
  RelocUpdater callback;/**< Callback to do for doing the instruction/data change.  */
} RelocQueue;

bool Reloc_QueueExprUpdate (RelocUpdater callback, ARMWord offset, ValueTag legal,
			    void *privData, size_t sizePrivData);

int relocFix (const Symbol *area);
void relocAOFOutput (FILE *outfile, const Symbol *area);
#ifndef NO_ELF_SUPPORT
void relocELFOutput (FILE *outfile, const Symbol *area);
#endif

Reloc *Reloc_Create (uint32_t how, uint32_t offset, const Value *value);

#endif
