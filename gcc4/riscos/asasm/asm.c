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
#include "main.h"

#ifdef DEBUG
//#  define DEBUG_ASM
#endif

/**
 * Parse the input file, and perform the assembly.
 * \param asmFile Filename to assemble.
 */
void
ASM_Assemble (const char *asmFile)
{
  inputInit (asmFile);

  setjmp (asmContinue); // FIXME: this will be wrong when we're skipping if/while contents.
  asmContinueValid = true;

  /* Process input line-by-line.  */
  while (gCurPObjP != NULL && inputNextLine ())
    {
      /* Ignore blank lines and comments.  */
      if (Input_IsEolOrCommentStart ())
	continue;

      /* Read label (if there is one).  */
      Lex label;
      if (!isspace ((unsigned char)inputLook ()))
	label = Lex_GetDefiningLabel (false);
      else
	label.tag = LexNone;
      skipblanks ();

#ifdef DEBUG_ASM
      printf ("%s: %d: ", FS_GetCurFileName (), FS_GetCurLineNumber ());
      lexPrint (&label);
      printf ("\n");
#endif

      decode (&label);
    }

  asmContinueValid = false;
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

  Symbol *symbol = symbolAdd (label);
  if (symbol->value.Tag != ValueIllegal)
    return NULL; /* Label was already defined with a value.  */
  symbol->type |= SYMBOL_ABSOLUTE; /* FIXME: this feels wrong as labels are relative against the base of their AREA where they are defined in.  */
  assert (symbol->value.Tag == ValueIllegal);
  if (areaCurrentSymbol->area.info->type & AREA_BASED)
    {
      /* Define label as "ValueAddr AreaBaseReg, #<given area offset>".  */
      symbol->value = Value_Addr (Area_GetBaseReg (areaCurrentSymbol->area.info), offset);
    }
  else if (areaCurrentSymbol->area.info->type & AREA_ABS)
    symbol->value = Value_Int (areaCurrentSymbol->area.info->baseAddr + offset);
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
      symbol->value = Value_Code (sizeof (values)/sizeof (values[0]), values);
    }

  return symbol;
}
