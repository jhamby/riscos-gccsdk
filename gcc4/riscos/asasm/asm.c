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
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif

#include "area.h"
#include "asm.h"
#include "code.h"
#include "decode.h"
#include "error.h"
#include "filestack.h"
#include "input.h"
#include "lex.h"

#ifdef DEBUG
//#  define DEBUG_ASM
#endif

/**
 * Parse the input file, and perform the assembly.
 */
void
assemble (void)
{
  /* Process input line-by-line.  */
  while (gCurPObjP != NULL && inputNextLine ())
    {
      /* Ignore blank lines and comments.  */
      if (Input_IsEolOrCommentStart ())
	continue;

      /* Read label (if there is one).  */
      Lex label;
      if (!isspace ((unsigned char)inputLook ()))
	label = Lex_GetDefiningLabel ();
      else
	label.tag = LexNone;
      skipblanks ();
#ifdef DEBUG_ASM
      printf ("%s: %d: ", FS_GetCurFileName (), FS_GetCurLineNumber ());
      lexPrint (&label);
      printf ("\n");
#endif
      skipblanks ();
      if (!Input_IsEolOrCommentStart ())
	decode (&label);
      else
	asm_label (&label);
    }
}


Symbol *
asm_label (const Lex *label)
{
  if (label->tag != LexId)
    return NULL;

  Symbol *symbol = symbolAdd (label);
  symbol->value = valueLateToCode (areaCurrentSymbol->value.Data.Int.i,
				   codeNewLateInfo (areaCurrentSymbol));
  symbol->type |= SYMBOL_ABSOLUTE;

  return symbol;
}
