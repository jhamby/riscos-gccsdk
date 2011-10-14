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

ASM_Phase_e gASM_Phase = eStartup;

/**
 * Read one line input.
 * \return true for success, false otherwise.
 */
static bool
ASM_NextLine (void)
{
  const char *curFile = FS_GetCurFileName ();
  int curLine = FS_GetCurLineNumber ();
  while (!Input_NextLine (eVarSubst))
    {
      /* Failed to read a line, this might be we're EOD for the current
         parsable object.  Go up one.  */
      if (gCurPObjP->type == POType_eFile)
	errorLine (curFile, curLine, ErrorWarning, "No END found");
      FS_PopPObject (false);
      if (gCurPObjP == NULL)
	return false;
      curFile = FS_GetCurFileName ();
      curLine = FS_GetCurLineNumber ();
    }
  return true;
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

      while (gCurPObjP != NULL && ASM_NextLine ())
	{
	  /* Ignore blank lines and comments.  */
	  if (Input_IsEolOrCommentStart ())
	    continue;

#ifdef DEBUG_ASM
	  const char *fileName = FS_GetCurFileName ();
	  size_t len = strlen (fileName);
	  if (len > 12)
	    fileName += len - 12;
	  printf("%.*s : %d : 0x%x : <%s>\n", (int)len, fileName, FS_GetCurLineNumber (), areaCurrentSymbol->area.info->curIdx, inputLine ());
#endif
	    /* Read label (in case there is one).  */
	    Lex label;
	  if (!isspace ((unsigned char)inputLook ()))
	    label = Lex_GetDefiningLabel (false);
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
ASM_Assemble (const char *asmFile)
{
  Area_PrepareForPhase (ePassOne);
  Local_PrepareForPhase (ePassOne);
  gASM_Phase = ePassOne;
  ASM_DoPass (asmFile);

  /* Don't do a next pass if we already have errors now.  */
  if (returnExitStatus () == EXIT_SUCCESS)
    {
      Area_PrepareForPhase (ePassTwo);
      Local_PrepareForPhase (ePassTwo);
      gASM_Phase = ePassTwo;
      ASM_DoPass (asmFile);
    }
}


/**
 * Defines a label being at offset of the current AREA.
 * \return NULL when give Lex object can not be a label or has been defined
 * as label before.
 */
Symbol *
ASM_DefineLabel (const Lex *label, int offset)
{
  if (label->tag != LexId)
    return NULL;

  Value value;
  unsigned symbolType;
  if (areaCurrentSymbol->area.info->type & AREA_BASED)
    {
      /* Define label as "ValueAddr AreaBaseReg, #<given area offset>".  */
      value = Value_Addr (Area_GetBaseReg (areaCurrentSymbol->area.info), offset);
      symbolType = SYMBOL_DEFINED;
    }
  else if (areaCurrentSymbol->area.info->type & AREA_ABS)
    {
      value = Value_Int (Area_GetBaseAddress (areaCurrentSymbol) + offset);
      symbolType = SYMBOL_DEFINED | SYMBOL_ABSOLUTE;
    }
  else
    {
      /* Define label as "ValueSymbol(current AREA) + <given area offset>".  */
      const Code values[] =
	{
	    { .Tag = CodeValue,
	      .Data.value = { .Tag = ValueSymbol, .Data.Symbol = { .factor = 1, .symbol = areaCurrentSymbol } } },
	    { .Tag = CodeValue,
	      .Data.value = { .Tag = ValueInt, .Data.Int = { .i = offset } } },
	    { .Tag = CodeOperator,
	      .Data.op = Op_add }
	};
      value = Value_Code (sizeof (values)/sizeof (values[0]), values);
      symbolType = SYMBOL_DEFINED;
    }

  Symbol *symbol = symbolGet (label);
  if (Symbol_Define (symbol, symbolType, &value))
    return NULL;

  if (!(areaCurrentSymbol->area.info->type & AREA_ABS))
    symbol->area.rel = areaCurrentSymbol;

  return symbol;
}
