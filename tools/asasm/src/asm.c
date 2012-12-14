/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2012 GCCSDK Developers
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
 * asm.c
 */

#include "config.h"

#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif

#include "area.h"
#include "asm.h"
#include "code.h"
#include "decode.h"
#include "error.h"
#include "expr.h"
#include "filestack.h"
#include "input.h"
#include "lex.h"
#include "local.h"
#include "main.h"

#ifdef DEBUG
//#  define DEBUG_ASM
#endif

/**
 * Read one line input.
 * \return true for success, false otherwise.
 */
static bool
ASM_NextLine (void)
{
  while (gCurPObjP != NULL)
    {
      const char *curFileName = FS_GetCurFileName ();
      unsigned curLineNum = FS_GetCurLineNumber ();

      if (Input_NextLine (eVarSubst))
	return true;

      /* Failed to read a line, this might be we're EOD for the current
         parsable object.  Go up one.  */
      if (gCurPObjP->type == POType_eFile)
	errorLine (curFileName, curLineNum, ErrorWarning, "No END found");
      FS_PopPObject (false);
    }
  return false;
}

static void
ASM_DoPass (const char *asmFile)
{
  assert (gCurPObjP == NULL);
  FS_PushFilePObject (asmFile);

  // FIXME: this will be wrong when we're skipping if/while contents.
  if (setjmp (asmContinue) == 0)
    {
      asmContinueValid = true;

      while (ASM_NextLine ())
	{
	  /* Ignore blank lines and comments.  */
	  if (Input_IsEolOrCommentStart ())
	    continue;

#ifdef DEBUG_ASM
	  const char *fileName = FS_GetCurFileName ();
	  size_t len = strlen (fileName);
	  if (len > 12)
	    fileName += len - 12;
	  printf("%.*s : %u : 0x%x : <%s>\n", (int)len, fileName, FS_GetCurLineNumber (), areaCurrentSymbol->area->curIdx, inputLine ());
#endif
	  /* Read label (in case there is one).  */
	  Lex label;
	  if (!isspace ((unsigned char)inputLook ()))
	    label = Lex_GetDefiningLabel ();
	  else
	    label.tag = LexNone;
	  skipblanks ();

#ifdef DEBUG_ASM
	  lexPrint (&label);
	  printf ("\n");
#endif

	  decode (&label);
	}
    }
  else
    {
      while (gCurPObjP != NULL)
	FS_PopPObject (true);
    }
  
  asmContinueValid = false;
}

/**
 * Parse the input file, and perform the assembly.
 * \param asmFile Filename to assemble.
 */
void
ASM_Assemble (const char *asmFile, bool doOnePassOnly)
{
  Phase_PrepareFor (ePassOne);
  ASM_DoPass (asmFile);

  /* Don't do a next pass if we already have errors now.  */
  if (!doOnePassOnly && returnExitStatus () == EXIT_SUCCESS)
    {
      Phase_PrepareFor (ePassTwo);
      ASM_DoPass (asmFile);
    }
}


/**
 * Defines a label being at offset of the current AREA.
 * \param isMapping true when it is an area mapping symbol (i.e. $a, $d, $t)
 * This will ensure we make the mapping symbol relative to its area
 * (even for ABS areas).
 * \return NULL when give Lex object can not be a label or has been defined
 * as label before.
 */
Symbol *
ASM_DefineLabel (const Lex *label, uint32_t offset, bool isMapping)
{
  if (label->tag == LexNone)
    return NULL;
  assert (label->tag == LexId || label->tag == LexLocalLabel);

  /* Turn LexLocalLabel into LexId (assuming no errors).  */
  Lex localLabel;
  if (label->tag == LexLocalLabel)
    {
      localLabel = Lex_DefineLocalLabel (label);
      if (localLabel.tag == LexNone)
	return NULL;
      assert (localLabel.tag == LexId);
      label = &localLabel;
    }

  /* Define label as "ValueSymbol(current AREA) + <given area offset>".  */
  const Value value = Value_Symbol (areaCurrentSymbol, 1, offset);
  unsigned symbolType = !isMapping && (areaCurrentSymbol->area->type & AREA_ABS) != 0 ? SYMBOL_ABSOLUTE : 0;

  /* All Data symbols in code areas need SYMBOL_DATUM bit set.
     Note Area_GetCurrentEntryType() can return eInvalid when we're going
     to define a label at the start of an area which haven't processed
     yet any data or ARM/Thumb instruction.  */
  if ((areaCurrentSymbol->area->type & AREA_CODE) != 0
      && Area_GetCurrentEntryType () == eData)
    symbolType |= SYMBOL_DATUM;

  Symbol *symbol = Symbol_Get (label);
  if (Symbol_Define (symbol, symbolType, &value))
    return NULL;

  return symbol;
}
