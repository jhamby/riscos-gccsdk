/*
 * AS an assembler for ARM
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * reloc.c
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

#include "aoffile.h"
#include "area.h"
#include "code.h"
#include "elf.h"
#include "error.h"
#include "expr.h"
#include "eval.h"
#include "filestack.h"
#include "global.h"
#include "main.h"
#include "output.h"
#include "phase.h"
#include "reloc.h"
#include "symbol.h"

Reloc *
Reloc_Create (uint32_t how, uint32_t offset, const Value *value)
{
  /* Check if we can create a relocation for given value.  */
  if (value->Tag != ValueSymbol || value->Data.Symbol.factor <= 0)
    return NULL;

  Reloc *newReloc;
  if ((newReloc = malloc (sizeof (Reloc))) == NULL)
    Error_OutOfMem ();
  newReloc->next = areaCurrentSymbol->area->relocs;
  newReloc->reloc.Offset = offset;
  newReloc->reloc.How = how;
  newReloc->value.Tag = ValueIllegal;
  Value_Assign (&newReloc->value, value);

  areaCurrentSymbol->area->relocs = newReloc;

  /* Mark we want this symbol in our output.  */
  if ((newReloc->value.Data.Symbol.symbol->type & SYMBOL_AREA) == 0)
    newReloc->value.Data.Symbol.symbol->used = 0;

  return newReloc;
}


/**
 * \return The number of relocations associated with given area.
 */
unsigned
Reloc_GetNumberRelocs (const Symbol *area)
{
  /* Calculate the number of relocations, i.e. per Reloc object, count all
     ValueSymbols.  */
  unsigned norelocs = 0;
  for (const Reloc *relocs = area->area->relocs;
       relocs != NULL;
       relocs = relocs->next)
    {
      if (relocs->value.Tag == ValueSymbol)
	norelocs += relocs->value.Data.Symbol.factor;
      else
	{
assert (0); /* FIXME: this code can be removed.  */
	  assert (relocs->value.Tag == ValueCode);
	  size_t len = relocs->value.Data.Code.len;
	  const Code *code = relocs->value.Data.Code.c;
	  for (size_t i = 0; i != len; ++i)
	    {
	      if (code->Tag == CodeValue
	          && code->Data.value.Tag == ValueSymbol)
		norelocs += code->Data.value.Data.Symbol.factor;
	    }
	}
    }

  return norelocs;
}


void
Reloc_AOFOutput (FILE *outfile, const Symbol *area)
{
  for (const Reloc *relocs = area->area->relocs; relocs != NULL; relocs = relocs->next)
    {
      AofReloc areloc =
	{
	  .Offset = armword (relocs->reloc.Offset),
	  .How = armword (relocs->reloc.How)
	};
      if (relocs->value.Tag == ValueSymbol)
	{
	  const Value *value = &relocs->value;

	  assert (value->Data.Symbol.symbol->used >= 0);
	  areloc.How |= value->Data.Symbol.symbol->used;
	  if (!(value->Data.Symbol.symbol->type & SYMBOL_AREA))
	    areloc.How |= HOW2_SYMBOL;
	  areloc.How = armword (areloc.How);
	  int loop = value->Data.Symbol.factor;
	  assert (loop > 0 && "Reloc_Create() check on this got ignored");
	  while (loop--)
	    fwrite (&areloc, 1, sizeof (AofReloc), outfile);
	}
      else
	{
assert (0); /* FIXME: this code can be removed.  */
	  assert (relocs->value.Tag == ValueCode);
	  size_t len = relocs->value.Data.Code.len;
	  const Code *code = relocs->value.Data.Code.c;
	  for (size_t i = 0; i != len; ++i)
	    {
	      if (code->Tag == CodeValue
	          && code->Data.value.Tag == ValueSymbol)
		{
		  const Value *value = &code->Data.value;

		  assert (value->Data.Symbol.symbol->used >= 0);
		  areloc.How |= value->Data.Symbol.symbol->used;
		  if (!(value->Data.Symbol.symbol->type & SYMBOL_AREA))
		    areloc.How |= HOW2_SYMBOL;
		  areloc.How = armword (areloc.How);
		  int loop = value->Data.Symbol.factor;
		  assert (loop > 0 && "Reloc_Create() check on this got ignored");
		  while (loop--)
		    fwrite (&areloc, 1, sizeof (AofReloc), outfile);
		}
	    }
	}
    }
}


#ifndef NO_ELF_SUPPORT
void
Reloc_ELFOutput (FILE *outfile, const Symbol *area)
{
  for (const Reloc *relocs = area->area->relocs; relocs != NULL; relocs = relocs->next)
    {
      Elf32_Rel areloc =
	{
	  .r_offset = armword (relocs->reloc.Offset),
	  .r_info = 0
	};
      if (relocs->value.Tag == ValueSymbol)
	{
	  const Value *value = &relocs->value;

	  assert (value->Data.Symbol.symbol->used >= 0);
	  int symbol = value->Data.Symbol.symbol->used;
	  int type;
	  if (relocs->reloc.How & HOW2_RELATIVE)
	    type = R_ARM_PC24; /* FIXME: for EABI (when ELF_EABI defined), this should be R_ARM_CALL or R_ARM_JUMP24.  */
	  else
	    type = R_ARM_ABS32;
	  areloc.r_info = armword (ELF32_R_INFO (symbol, type));
	  int loop = value->Data.Symbol.factor;
	  assert (loop > 0 && "Reloc_Create() check on this got ignored");
	  while (loop--)
	    fwrite (&areloc, 1, sizeof (Elf32_Rel), outfile);
	}
      else
	{
assert (0); /* FIXME: this code can be removed.  */
	  assert (relocs->value.Tag == ValueCode);
	  size_t len = relocs->value.Data.Code.len;
	  const Code *code = relocs->value.Data.Code.c;
	  for (size_t i = 0; i != len; ++i)
	    {
	      if (code->Tag == CodeValue
	          && code->Data.value.Tag == ValueSymbol)
		{
		  const Value *value = &code->Data.value;

		  assert (value->Data.Symbol.symbol->used >= 0);
		  int symbol = value->Data.Symbol.symbol->used;
		  int type;
		  if (relocs->reloc.How & HOW2_RELATIVE)
		    type = R_ARM_PC24; /* FIXME: for EABI (when ELF_EABI defined), this should be R_ARM_CALL or R_ARM_JUMP24.  */
		  else
		    type = R_ARM_ABS32;
		  areloc.r_info = armword (ELF32_R_INFO (symbol, type));
		  int loop = value->Data.Symbol.factor;
		  assert (loop > 0 && "Reloc_Create() check on this got ignored");
		  while (loop--)
		    fwrite (&areloc, 1, sizeof (Elf32_Rel), outfile);
		}
	    }
	}
    }
}
#endif
