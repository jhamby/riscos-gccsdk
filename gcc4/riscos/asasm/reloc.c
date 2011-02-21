/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2011 GCCSDK Developers
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
#include "fix.h"
#include "get.h"
#include "global.h"
#include "input.h"
#include "lex.h"
#include "main.h"
#include "option.h"
#include "output.h"
#include "reloc.h"
#include "symbol.h"

static RelocQueue *
Reloc_CreateQueueObj (void)
{
  RelocQueue *rel;
  if ((rel = malloc (sizeof (RelocQueue))) == NULL)
    errorOutOfMem ();

  rel->next = areaCurrentSymbol->area.info->relocQueue;
  rel->file = FS_GetCurFileName ();
  rel->lineno = FS_GetCurLineNumber ();
  /* rel->privData = */
  /* rel->offset = */
  rel->expr.Tag = ValueIllegal;
  /* rel->legal = */
  /* rel->callback = */
  
  areaCurrentSymbol->area.info->relocQueue = rel;

  return rel;
}


/**
 * Builds & evaluates expression.
 * Update instruction/data using callback at given offset.
 * \param callback Will *always* be called twice.  First time with 'final'
 * being false, second time with 'final' true.
 * \param legal Valid Value Tags.
 * \return false for success, true for failure.
 */
bool
Reloc_QueueExprUpdate (RelocUpdater callback, ARMWord offset, ValueTag legal,
		       void *privData, size_t sizePrivData)
{
  Value value; /* We have ownership of this.  */
  if (Code_HasUndefinedSymbols ())
    value = Code_TakeSnapShot ();
  else
    {
      /* Evaluate expression.  */
      const Value *result = codeEval (legal, legal & ValueAddr ? &offset : NULL);
      if (result->Tag == ValueIllegal)
	return true; /* This is bad, we even didn't get a ValueCode.  */
      if (result->Tag == ValueCode)
	{
	  value.Tag = ValueIllegal;
	  Value_Assign (&value, result);
	}
      else
	{
	  /* Make ValueCode from it.  */
	  const Code code =
	    {
	      .Tag = CodeValue,
	      .Data.value = *result
	    };
	  value = Value_Code (1, &code);
	}
    }
  assert (value.Tag == ValueCode);
  if (!callback (FS_GetCurFileName (), FS_GetCurLineNumber (), offset, &value,
		 privData, false))
    {
      valueFree (&value);
      return false; /* Instruction/data update succeeded.  */
    }
  
  /* We have something which the callback didn't like that much.  We'll give
     it a 2nd try after all labels are defined.  Either the missing labels are
     defined by then and we can update our area content, either we have
     to emit a relocation.  */
  RelocQueue *rel = Reloc_CreateQueueObj ();
  if (privData == NULL || sizePrivData == 0)
    rel->privData = NULL;
  else
    {
      if ((rel->privData = malloc (sizePrivData)) == NULL)
	errorOutOfMem ();
      memcpy (rel->privData, privData, sizePrivData);
    }
  rel->offset = offset;
  rel->expr = value;
  rel->legal = legal;
  rel->callback = callback;
  return false;
}


Reloc *
Reloc_Create (uint32_t how, uint32_t offset, const Value *value)
{
  /* Check if we can create a relocation for given value.  */
  if (value->Tag != ValueSymbol || value->Data.Symbol.factor <= 0)
    return NULL;

  Reloc *newReloc;
  if ((newReloc = malloc (sizeof (Reloc))) == NULL)
    errorOutOfMem ();
  newReloc->next = areaCurrentSymbol->area.info->relocs;
  newReloc->reloc.Offset = offset;
  newReloc->reloc.How = how;
  newReloc->value.Tag = ValueIllegal;
  Value_Assign (&newReloc->value, value);

  areaCurrentSymbol->area.info->relocs = newReloc;

  /* Mark we want this symbol in our output.  */
  if ((newReloc->value.Data.Symbol.symbol->type & SYMBOL_AREA) == 0)
    newReloc->value.Data.Symbol.symbol->used = 0;
  
  return newReloc;
}


/**
 * \return The number of relocations associated with given area.
 */
int
relocFix (const Symbol *area)
{
  areaCurrentSymbol = area;
  for (RelocQueue *relQueue = areaCurrentSymbol->area.info->relocQueue;
       relQueue != NULL;
       relQueue = relQueue->next)
    {
      /* Re-evaluate value.  By now we can get a different result which we can
         use.  */
      codeInit ();
      codeValue (&relQueue->expr, true);
      const Value *value = codeEval (relQueue->legal | ValueCode,
				     relQueue->legal & ValueAddr ? &relQueue->offset : NULL);
      if (value->Tag == ValueIllegal)
	errorLine (relQueue->file, relQueue->lineno, ErrorError, "Unable to express relocation");
      else
	{
	  Code code =
	    {
	      .Tag = CodeValue,
	      .Data.value = *value
	    };
	  const Value codeAsValue =
	    {
	      .Tag = ValueCode,
	      .Data.Code = { .len = 1, .c = &code }
	    };
	  if (value->Tag != ValueCode)
	    value = &codeAsValue;
	  if (relQueue->callback (relQueue->file, relQueue->lineno,
				  relQueue->offset, value, relQueue->privData, true))
	    errorLine (relQueue->file, relQueue->lineno, ErrorError, "Unable to express relocation");
	}
    }
  areaCurrentSymbol = NULL;

  /* Calculate the number of relocations, i.e. per Reloc object, count all
     ValueSymbols.  */
  int norelocs = 0;
  for (const Reloc *relocs = area->area.info->relocs;
       relocs != NULL;
       relocs = relocs->next)
    {
      if (relocs->value.Tag == ValueSymbol)
	norelocs += relocs->value.Data.Symbol.factor;
      else
	{
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
relocAOFOutput (FILE *outfile, const Symbol *area)
{
  for (const Reloc *relocs = area->area.info->relocs; relocs != NULL; relocs = relocs->next)
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
relocELFOutput (FILE *outfile, const Symbol *area)
{
  for (const Reloc *relocs = area->area.info->relocs; relocs != NULL; relocs = relocs->next)
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
	  int symbol = value->Data.Symbol.symbol->used + 1;
	  int type;
	  if (relocs->reloc.How & HOW3_RELATIVE)
	    type = R_ARM_PC24;
	  else
	    type = R_ARM_ABS32;
	  areloc.r_info = armword (ELF32_R_INFO (symbol, type));
	  areloc.r_info = armword (areloc.r_info);
	  int loop = value->Data.Symbol.factor;
	  assert (loop > 0 && "Reloc_Create() check on this got ignored");
	  while (loop--)
	    fwrite (&areloc, 1, sizeof (Elf32_Rel), outfile);
	}
      else
	{
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
		  int symbol = value->Data.Symbol.symbol->used + 1;
		  int type;
		  if (relocs->reloc.How & HOW3_RELATIVE)
		    type = R_ARM_PC24;
		  else
		    type = R_ARM_ABS32;
		  areloc.r_info = armword (ELF32_R_INFO (symbol, type));
		  areloc.r_info = armword (areloc.r_info);
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
