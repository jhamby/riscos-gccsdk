/*
 *  asm.c
 * Copyright © 1992 Niklas Röjemo
 */

#include "sdk-config.h"
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
#include "input.h"
#include "lex.h"

/******************************************************************
* Parse the input file, and perform the assembly.
*
******************************************************************/
void
asm_ (void)
{
  /* read each line from the input into input.c:workBuff,
  ** if inputExpand is true, then expand the input line into where necessary
  */
  while (inputNextLine ())
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
	{
	  if (decode (&label))
	    break;
	}
      else
	asm_label (&label);

      inputExpand = TRUE;
    }
}


Symbol *
asm_label (Lex * label)
{
  Symbol *symbol;
  if (label->tag != LexId)
    return 0;
/*  if (label->LexId.len>1) {
   if (label->LexId.str[label->LexId.len-2]=='$' &&
   label->LexId.str[label->LexId.len-2]=='l')
   error(ErrorWarning,FALSE,"Local labels not yet implemented");
   }
 */
  symbol = symbolAdd (*label);
//  symbol = symbolGet(*label);
  if (areaCurrent)
    {
      symbol->value = valueLateToCode (areaCurrent->value.ValueInt.i,
				       codeNewLateInfo (areaCurrent));
      symbol->type |= SYMBOL_ABSOLUTE;
      /* If the current area is a based area, then set the based bit.  */
      if (areaCurrent->area.info->type & AREA_BASED)
	symbol->type |= SYMBOL_BASED;
    }
  else
    {
      symbol->type = 0;
      symbol->value.Tag.t = ValueIllegal;
    }
  return symbol;
}
