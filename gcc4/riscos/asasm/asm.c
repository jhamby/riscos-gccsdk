/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2010 GCCSDK Developers
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
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "area.h"
#include "asm.h"
#include "code.h"
#include "decode.h"
#include "error.h"
#include "filestack.h"
#include "input.h"
#include "lex.h"

/**
 * Parse the input file, and perform the assembly.
 */
void
assemble (void)
{
  /* Read each line from the input into input.c:workBuff, if inputExpand is
     true, then expand the input line into where necessary.  */
  while (gCurPObjP != NULL && inputNextLine ())
    {
      Lex label;
      Symbol *symbol;

      /* ignore blank lines and comments */
      if (inputLook () && !isspace (inputLook ()) && !inputComment ())
	{
	  /* Deal with any label */
	  label = isdigit (inputLook ())? lexGetLocal () : lexGetId ();

	  /* Check for local label here */
	  skipblanks ();
	  if (inputLook () == ':')
	    inputSkip ();
	}
      else
	{
	  label.tag = LexNone;
	  symbol = NULL;
	}
      skipblanks ();
      if (!inputComment ())
	decode (&label);
      else
	asm_label (&label);

      inputExpand = true;
    }
}


Symbol *
asm_label (const Lex *label)
{
  if (label->tag != LexId)
    return NULL;

  Symbol *symbol = symbolAdd (label);
  if (areaCurrentSymbol)
    {
      symbol->value = valueLateToCode (areaCurrentSymbol->value.ValueInt.i,
				       codeNewLateInfo (areaCurrentSymbol));
      symbol->type |= SYMBOL_ABSOLUTE;
      /* If the current area is a based area, then set the based bit.  */
      if (areaCurrentSymbol->area.info->type & AREA_BASED)
	symbol->type |= SYMBOL_BASED;
    }
  else
    {
      symbol->type = 0;
      symbol->value.Tag.t = ValueIllegal;
    }
  return symbol;
}
