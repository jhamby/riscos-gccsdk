
/*
 *  asm.c
 * Copyright © 1992 Niklas Röjemo
 */
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include "input.h"
#include "error.h"
#include "decode.h"
#include "lex.h"
#include "area.h"
#include "asm.h"
#include "code.h"


void
asm_ (void)
{
  Lex label;
  Symbol *symbol;

  while (inputNextLine ())
    {
      if (inputLook () && !isspace (inputLook ()) && !inputComment ())
	{
	  label = isdigit (inputLook ())? lexGetLocal () : lexGetId ();
	  /* Check for local label here */
	  skipblanks ();
	  if (inputLook () == ':')
	    inputSkip ();
	}
      else
	{
	  label.tag = LexNone;
	  symbol = 0;
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
