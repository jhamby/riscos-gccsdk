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

static bool if_skip (const char *onerror, bool closeOnly);
static void while_skip (void);
static void whileFree (void);
static bool whileReEval (void);

/**
 * Skip following assembler lines because of
 * either a failed '[' test, so until we find the corresponding matching '|' or ']',
 * either we had a successful '[' test and we reached the '|' part.
 * \param onerror Error message to given when no matching '|' or ']' can be found
 * in the current parse object.
 * \param closeOnly When true, expect only ']' to match.
 */
static bool
if_skip (const char *onerror, bool closeOnly)
{
  /* We will now skip input lines until a matching '|' or ']'.  This means
     we have to do the final decode check ourselves.  */
  decode_finalcheck ();

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

      /* Check for '[', '|', ']', 'IF', 'ELSE', 'ENDIF'.  */
      enum { t_unknown, t_if, t_else, t_endif } toktype;
      int numSkip = 1;
      char c = inputLookN (0);
      if (c == '[')
	toktype = t_if;
      else if (c == '|')
	toktype = t_else;
      else if (c == ']')
	toktype = t_endif;
      else
	{
	  if (c == 'I' && inputLookN (1) == 'F')
	    {
	      toktype = t_if;
	      numSkip = 2;
	    }
	  else if (c == 'E')
	    {
	      if (inputLookN (1) == 'N'
		  && inputLookN (2) == 'D'
		  && inputLookN (3) == 'I'
		  && inputLookN (4) == 'F')
		{
		  toktype = t_endif;
		  numSkip = 5;
		}
	      else if (inputLookN (1) == 'L'
		       && inputLookN (2) == 'S'
		       && inputLookN (3) == 'E')
		{
		  toktype = t_else;
		  numSkip = 4;
		}
	      else
		toktype = t_unknown;
	    }
	  else
	    toktype = t_unknown;
	}
      if (toktype == t_unknown || (inputLookN (numSkip) && !isspace ((unsigned char)inputLookN (numSkip))))
	continue;
      inputSkipN (numSkip);
      if (toktype != t_if)
	{
          skipblanks ();
          if (!Input_IsEolOrCommentStart ())
	    error (ErrorError, "Spurious characters after %s token", toktype == t_else ? "ELSE" : "ENDIF");
	}

      switch (toktype)
	{
	  case t_if:
	    if (label.tag != LexNone)
	      error (ErrorWarning, "Label not allowed here - ignoring");
	    nested++;
	    break;
	  case t_else:
	    if (nested == 0)
	      {
		if (label.tag != LexNone)
		  error (ErrorWarning, "Label not allowed here - ignoring");
		if (!closeOnly)
		  return false;
		error (ErrorError, "Spurious '|' is being ignored");
	      }
	    break;
	  case t_endif:
	    if (nested-- == 0)
	      return c_endif (&label);
	    break;
	}
    }
  /* We reached the end of the current parsing object without finding a matching
     '|' nor ']'.  */
  error (ErrorError, "%s", onerror);
  return false;
}

/**
 * Implements '['
 * Only called from decode().
 */
bool
c_if (const Lex *label)
{
  if (label->tag != LexNone)
    error (ErrorWarning, "Label not allowed here - ignoring");

  gCurPObjP->if_depth++;

  const Value *flag = exprBuildAndEval (ValueBool);
  bool skipToEndIf;
  if (flag->Tag != ValueBool)
    {
      error (ErrorError, "IF expression must be boolean (treating as true)");
      skipToEndIf = false;
    }
  else
    skipToEndIf = !flag->Data.Bool.b;

  if (skipToEndIf)
    return if_skip ("No matching | or ]", false);

  return false;
}

/**
 * Implements '|'
 * The previous '[' evaluated to {TRUE} and we're now about to enter the 'else'
 * clause which we have to ignore.
 * Only called from decode().
 */
bool
c_else (const Lex *label)
{
  if (!gCurPObjP->if_depth)
    {
      error (ErrorError, "Mismatched |");
      return false;
    }

  if (label->tag != LexNone)
    error (ErrorWarning, "Label not allowed here - ignoring");

  return if_skip ("No matching ]", true);
}

/**
 * Implements ']'
 * Called from decode() (the previous lines were being assembled) and
 * if_skip() (the previous lines were being skipped).
 */
bool
c_endif (const Lex *label)
{
  if (!gCurPObjP->if_depth)
    {
      error (ErrorError, "Mismatched ]");
      return false;
    }

  gCurPObjP->if_depth--;

  if (label->tag != LexNone)
    error (ErrorWarning, "Label not allowed here - ignoring");

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
      if (inputGet () == 'W')
	{
	  switch (inputGet ())
	    {
	      case 'H':	/* WHILE? */
		if (!notinput ("ILE")
		    && (isspace ((unsigned char)inputLook ()) || Input_IsEolOrCommentStart ()))
		  nested++;
		break;
	      case 'E':	/* WEND? */
		if (!notinput ("ND")
		    && (isspace ((unsigned char)inputLook ()) || Input_IsEolOrCommentStart ())
		    && nested-- == 0)
		  return;
	        break;
	    }
	}
      skiprest ();
    }
  error (ErrorError, "Missing WEND");
}


/**
 * Implements WHILE.
 */
bool
c_while (const Lex *label)
{
  if (label->tag != LexNone)
    error (ErrorWarning, "Label not allowed here - ignoring");

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
c_wend (const Lex *label)
{
  if (!gCurPObjP->whilestack)
    {
      error (ErrorError, "Mismatched WEND");
      return false;
    }

  if (label->tag != LexNone)
    error (ErrorWarning, "Label not allowed here - ignoring");

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
