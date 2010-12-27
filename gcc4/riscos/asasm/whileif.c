/*
 * AS an assembler for ARM
 * Copyright (c) 1997 Darren Salt
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
 * whileif.c
 */

#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif

#include "code.h"
#include "commands.h"
#include "decode.h"
#include "error.h"
#include "expr.h"
#include "filestack.h"
#include "input.h"
#include "lex.h"
#include "os.h"
#include "value.h"
#include "whileif.h"

typedef enum
{
  eSkipToElseElifOrEndif, /* Go to matching ELSE, ELIF or ENDIF and enable assembling (with ELIF, only when its argument is {TRUE}).  */
  eSkipToEndif, /* Go to matching ENDIF and enable assembling.  Ignore matching ELIF.  At matching ELSE, switch to ToDo_eSkipToEndifStrict.  */
  eSkipToEndifStrict /* Go to matching ENDIF (no matching ELSE, ELIF are allowed) and enable assembling.  */
} IfSkip_eToDo;

static bool if_skip (const char *onerror, const char *matchingToken, IfSkip_eToDo toDo);
static void while_skip (void);
static void whileFree (void);
static bool whileReEval (void);

/**
 * Skip following assembler lines.  And this because of
 * either a failed '[' / 'IF' test, so skip until we find the corresponding
 * matching '|', 'ELSE', 'ELIF', ']' or 'ENDIF',
 * either we had a successful '[' / 'IF' test and we now reached the '|',
 * 'ELSE' or 'ELIF' part.
 * \param onerror Error message to given when no matching '|', 'ELSE', 'ELIF',
 * ']' or 'ENDIF' can be found in the current parse object.
 * \param toDo Specifies what to do when encountering matching '|', 'ELSE',
 * 'ELIF', ']' and 'ENDIF'.
 */
static bool
if_skip (const char *onerror, const char *matchingToken, IfSkip_eToDo toDo)
{
  /* We will now skip input lines until a matching '|', 'ELSE', 'ELIF', ']' or
    'ENDIF'.  This means we have to do the final decode check ourselves
    for the current line.  */
  decode_finalcheck ();

  const char *startFileName = FS_GetCurFileName ();
  int startLineNumber = FS_GetCurLineNumber ();

  int nested = 0;
  while (inputNextLineNoSubst ())
    {
      /* Ignore blank lines and comments.  */
      if (Input_IsEolOrCommentStart ())
	continue;

      /* Check for label and skip it.
         Make special exception for '$' starting labels, i.e. macro arguments.  */
      Lex label;
      if (isspace ((unsigned char)inputLook ()))
	label.tag = LexNone;
      else if (inputLook () == '$')
	{
	  size_t len;
	  (void) inputSymbol (&len, '\0');
	  if (Input_Match ('.', false))
	    (void) inputSymbol (&len, '\0');
	  label.tag = LexNone;
	}
      else
	label = Lex_GetDefiningLabel (true);
      skipblanks ();

      /* Check for 'END'.  */
      if (Input_MatchKeyword ("END"))
	break;

      /* Check for '[', '|', ']', 'IF', 'ELSE', 'ELIF', 'ENDIF'.  */
      enum { t_if, t_else, t_elif, t_endif } toktype;
      if (Input_MatchKeyword ("["))
	toktype = t_if;
      else if (Input_MatchKeyword ("|"))
	toktype = t_else;
      else if (Input_MatchKeyword ("]"))
	toktype = t_endif;
      else if (Input_MatchKeyword ("IF"))
	toktype = t_if;
      else if (Input_MatchKeyword ("ELSE"))
	toktype = t_else;
      else if (Input_MatchKeyword ("ELIF"))
	toktype = t_elif;
      else if (Input_MatchKeyword ("ENDIF"))
	toktype = t_endif;
      else
	continue;

      if (toktype != t_if && toktype != t_elif)
	{
          skipblanks ();
          if (!Input_IsEolOrCommentStart ())
	    error (ErrorError, "Spurious characters after %s token", toktype == t_else ? "ELSE" : "ENDIF");
	}

      if (label.tag != LexNone)
	error (ErrorWarning, "Label not allowed here - ignoring");

      switch (toktype)
	{
	  case t_if:
	    nested++;
	    break;

	  case t_else:
	    if (nested == 0)
	      {
		/* Matching.  */
		switch (toDo)
		  {
		    case eSkipToElseElifOrEndif:
		      return false;
		    case eSkipToEndif:
		      toDo = eSkipToEndifStrict; /* From now on, we only expect matching ENDIF (no other matching ELSE, nor matching ELIF).  */
		      break;
		    case eSkipToEndifStrict:
		      error (ErrorError, "Spurious '|' or 'ELSE' is being ignored");
		      break;
		  }
	      }
	    break;

	  case t_elif:
	    if (nested == 0)
	      {
		switch (toDo)
		  {
		    case eSkipToElseElifOrEndif:
		      {
			const Value *flag = exprBuildAndEval (ValueBool);
			bool skipToElseElifOrEndIf;
			if (flag->Tag != ValueBool)
			  {
			    error (ErrorError, "ELIF expression must be boolean (treating as true)");
			    skipToElseElifOrEndIf = false;
			  }
			else
			  skipToElseElifOrEndIf = !flag->Data.Bool.b;
			if (!skipToElseElifOrEndIf)
			  return false;
			break;
		      }
		    case eSkipToEndif:
		      break;
		    case eSkipToEndifStrict:
		      error (ErrorError, "Spurious 'ELIF' is being ignored");
		      break;
		  }
	      }
	    break;

	  case t_endif:
	    if (nested-- == 0)
	      return c_endif ();
	    break;
	}
    }
  /* We reached the end of the current parsing object without finding a matching
     '|', 'ELSE', 'ELIF', ']' nor 'ENDIF'.  */
  error (ErrorError, "%s", onerror);
  errorLine (startFileName, startLineNumber, ErrorError,
	     "note: Corresponding %s was here", matchingToken);

  return false;
}

/**
 * Implements '[' and 'IF'.
 * Only called from decode().
 */
bool
c_if (void)
{
  gCurPObjP->if_depth++;

  const Value *flag = exprBuildAndEval (ValueBool);
  bool skipToElseElifOrEndIf;
  if (flag->Tag != ValueBool)
    {
      error (ErrorError, "IF expression must be boolean (treating as true)");
      skipToElseElifOrEndIf = false;
    }
  else
    skipToElseElifOrEndIf = !flag->Data.Bool.b;

  if (skipToElseElifOrEndIf)
    return if_skip ("No matching |, ELSE, ELIF, ] nor ENDIF", "IF", eSkipToElseElifOrEndif);

  return false;
}


/**
 * Implements '|' and 'ELSE'.
 * The previous '[' or 'IF' evaluated to {TRUE} and we're now about to enter
 * the '|' or 'ELSE' clause which we have to ignore.
 * The difference between c_else() and c_elif() is that the latter has an
 * argument we should ignore.
 * Only called from decode() and c_elif().
 */
bool
c_else (void)
{
  if (gCurPObjP->if_depth == 0)
    {
      error (ErrorError, "Mismatched | or ELSE");
      return false;
    }

  return if_skip ("No matching ] nor ENDIF", "ELSE", eSkipToEndifStrict);
}

/**
 * Implements 'ELIF'.
 * The previous '[' or 'IF' evaluated to {TRUE} and we're now about to enter
 * the 'ELIF' clause which we have to ignore.
 * The difference between c_else() and c_elif() is that the latter has an
 * argument we should ignore.
 * Only called from decode().
 */
bool
c_elif (void)
{
  /* Skip the argument of ELIF.  Not of interest now.  */
  skiprest ();

  if (gCurPObjP->if_depth == 0)
    {
      error (ErrorError, "Mismatched ELIF");
      return false;
    }

  return if_skip ("No matching ] nor ENDIF", "ELIF", eSkipToEndif);
}

/**
 * Implements ']' and 'ENDIF'
 * Called from decode() (the previous lines were being assembled) and
 * if_skip() (the previous lines were being skipped).
 */
bool
c_endif (void)
{
  if (gCurPObjP->if_depth == 0)
    {
      error (ErrorError, "Mismatched ] or ENDIF");
      return false;
    }

  gCurPObjP->if_depth--;

  return false;
}


/**
 * Skip following assembler input lines until we find matching WEND.
 */
static void
while_skip (void)
{
  int nested = 0;
  while (inputNextLine ()) /* FIXME: call inputNextLineNoSubst() instead, like in if_skip() ? */
    {
      /* Skip label (if there is one).  */
      if (!isspace ((unsigned char)inputLook ()))
	(void) Lex_GetDefiningLabel (true);
      skipblanks ();

      /* Look for WHILE and WEND.  */
      if (Input_MatchKeyword ("WHILE"))
	++nested;
      else if (Input_MatchKeyword ("WEND"))
	{
	  if (nested-- == 0)
	    return;
	}
      skiprest ();
    }
  error (ErrorError, "Missing WEND");
}


/**
 * Implements WHILE.
 */
bool
c_while (void)
{
  const char * const inputMark = Input_GetMark ();
  /* Evaluate expression */
  const Value *flag = exprBuildAndEval (ValueBool);
  if (flag->Tag != ValueBool)
    {
      error (ErrorError, "WHILE expression must be boolean (treating as false)");
      while_skip ();
      return false;
    }
#if 0
  /* FIXME: this needs to be implemented differently.  */
  if (!exprNotConst)
    {
      error (ErrorError, "WHILE expression is constant (treating as false)");
      while_skip ();
      return;
    }
#endif
#ifdef DEBUG
  printf("c_while() : expr is <%s>\n", flag->Data.Bool.b ? "true" : "false");
#endif
  if (!flag->Data.Bool.b)
    {
      while_skip ();
      return false;
    }

  WhileBlock *whileNew;
  if ((whileNew = malloc (sizeof (WhileBlock))) == NULL)
    errorOutOfMem ();
  /* Copy expression */
  Input_RollBackToMark (inputMark);
  if ((whileNew->expr = strdup (inputRest ())) == NULL)
    errorOutOfMem ();
  /* Put on stack */
  whileNew->prev = gCurPObjP->whilestack;
  whileNew->lineno = gCurPObjP->lineNum;
  switch (gCurPObjP->type)
    {
      case POType_eMacro:
	whileNew->tag = WhileInMacro;
	whileNew->ptr.macro.offset = gCurPObjP->d.macro.curPtr;
	break;
      case POType_eFile:
	whileNew->tag = WhileInFile;
	whileNew->ptr.file.offset = ftell (gCurPObjP->d.file.fhandle);
	break;
    }
  gCurPObjP->whilestack = whileNew;
  return false;
}


static void
whileFree (void)
{
  WhileBlock *p = gCurPObjP->whilestack;
  gCurPObjP->whilestack = p->prev;
  free ((void *)p->expr);
  free (p);
}


static bool
whileReEval (void)
{
  inputThisInstead (gCurPObjP->whilestack->expr);
  const Value *flag = exprBuildAndEval (ValueBool);
  if (flag->Tag != ValueBool)
    {
      error (ErrorError, "WHILE expression must be boolean (treating as false)");
      return false;
    }
#ifdef DEBUG
  printf("whileReEval() : expr is <%s>\n", flag->Data.Bool.b ? "true" : "false");
#endif
  if (flag->Data.Bool.b)
    {
      switch (gCurPObjP->whilestack->tag)
	{
	  case WhileInFile:
	    fseek (gCurPObjP->d.file.fhandle, gCurPObjP->whilestack->ptr.file.offset, SEEK_SET);
	    gCurPObjP->lineNum = gCurPObjP->whilestack->lineno;
	    return true;
	  case WhileInMacro:
	    gCurPObjP->d.macro.curPtr = gCurPObjP->whilestack->ptr.macro.offset;
	    gCurPObjP->lineNum = gCurPObjP->whilestack->lineno;
	    return true;
	  default:
	    errorAbort ("Internal whileReEval: unrecognised WHILE type");
	    break;
	}
    }
  return false;
}


/**
 * Implements WEND.
 */
bool
c_wend (void)
{
  if (!gCurPObjP->whilestack)
    {
      error (ErrorError, "Mismatched WEND");
      return false;
    }

  switch (gCurPObjP->whilestack->tag)
    {
      case WhileInFile:
      case WhileInMacro:
	if (whileReEval ())
	  return false;
	break;
      default:
	errorAbort ("Internal c_wend: unrecognised WHILE type");
	break;
    }
  whileFree ();
  return false;
}


void
FS_PopIfWhile (bool noCheck)
{
  if (gCurPObjP->if_depth)
    {
      if (!noCheck)
	error (ErrorError, "Unmatched IF%s", "s" + (gCurPObjP->if_depth > 1));
      gCurPObjP->if_depth = 0;
    }

  int i;
  for (i = 0; gCurPObjP->whilestack != NULL; ++i)
    whileFree ();
  if (i && !noCheck)
    error (ErrorError, "Unmatched WHILE%s", "s" + (i > 1));
}
