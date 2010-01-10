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
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
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

int if_depth = 0;
WhileBlock *whileCurrent = NULL;

static BOOL ignore_else = FALSE;

static void if_skip (const char *onerror);
static void while_skip (void);
static void whileFree (void);
static BOOL whileReEval (void);

static void
if_skip (const char *onerror)
{
  int c;
  int nested = 0;
  int tmp_inputExpand = inputExpand;
  inputExpand = FALSE;
  while (inputNextLine ())
   {
      if (inputLook () && !isspace (c = inputGet ()))
	{
	  char del = c == '|' ? c : 0;
	  inputSymbol (&c, del);
	  if (del && inputLook () == del)
	    inputSkip ();
	}
      skipblanks ();
      c = inputGet ();
      if (inputLook () && !isspace (inputGet ()))
	continue;
      switch (c)
	{
	case ']':
	  if (nested-- == 0)
	    goto skipped;
	  break;
	case '|':
	  if (nested == 0)
	    goto skipped;
	  break;
	case '[':
	  nested++;
	}
    }
  error (ErrorSerious, TRUE, onerror);
  inputExpand = tmp_inputExpand;
  return;

skipped:
  ignore_else = TRUE;
  inputRewind = TRUE;
  inputExpand = tmp_inputExpand;
}


void
c_if (void)
{
  Value flag;
  if_depth++;
/*  if (label->tag!=LexNone)
 *   error(ErrorWarning,TRUE,"Label not allowed with [ - ignored"); */
  exprBuild ();
  flag = exprEval (ValueBool);
  if (flag.Tag.t != ValueBool)
    {
      error (ErrorError, TRUE,
	     "IF expression must be boolean (treating as TRUE)");
      return;
    }
  if (!flag.ValueBool.b)
    if_skip ("No matching | or ]");
}


void
c_else (Lex * label)
{
  if (!if_depth)
    error (ErrorSerious, TRUE, "Mismatched |");
  if (label->tag != LexNone)
    {
      error (ErrorError, TRUE, "Label not allowed with |");
      ignore_else = FALSE;
      return;
    }
  if (!ignore_else)
    if_skip ("No matching ]");
  ignore_else = FALSE;
}


void
c_endif (Lex * label)
{
  if (!if_depth)
    error (ErrorSerious, TRUE, "Mismatched ]");
  else
    if_depth--;
  if (label->tag != LexNone)
    error (ErrorError, TRUE, "Label not allowed with ]");
  ignore_else = FALSE;
}


static void
while_skip (void)
{
  int nested = 0;
  while (inputNextLine ())
    {
      if (inputLook () && !isspace (inputGet ()))
	{
	  int c;
	  inputSymbol (&c, 0);
	}
      skipblanks ();
      if (inputGetLower () == 'w')
	{
	  switch (inputGetLower ())
	    {
	    case 'h':		/* WHILE? */
	      if (!(notinput ("ile") || (inputLook () && !isspace (inputGet ()))))
		nested++;
	      break;
	    case 'e':		/* WEND? */
	      if (!(notinput ("nd") || (inputLook () && !isspace (inputGet ()))))
		if (nested-- == 0)
		  return;
	    }
	}
      skiprest ();
    }
  error (ErrorSerious, TRUE, "Missing WEND");
}



void
c_while (Lex * label)
{
  WhileBlock *whileNew;
  Value flag;

  label = label;

  inputMark ();
  /* Evaluate expression */
  exprBuild ();
  flag = exprEval (ValueBool);
  if (flag.Tag.t != ValueBool)
    {
      error (ErrorError, TRUE,
	     "WHILE expression must be boolean (treating as FALSE)");
      while_skip ();
      return;
    }
  if (!exprNotConst)
    {
      error (ErrorError, TRUE,
	     "WHILE expression is constant (treating as FALSE)");
      while_skip ();
      return;
    }
#ifdef DEBUG
  printf("c_while() : expr is <%s>\n", flag.ValueBool.b ? "TRUE" : "FALSE");
#endif
  if (!flag.ValueBool.b)
    {
      while_skip ();
      return;
    }
  if ((whileNew = malloc (sizeof (WhileBlock))) == NULL)
    {
    nomem:
      errorOutOfMem ("c_while");
      return;
    }
  /* Copy expression */
  inputRollback ();
  if ((whileNew->expr = strdup (inputRest ())) == NULL)
    {
      free (whileNew);
      goto nomem;
    }
  /* Put on stack */
  whileNew->prev = whileCurrent;
  whileNew->lineno = inputLineNo;
  if (macroCurrent)
    {
      whileNew->tag = WhileInMacro;
      whileNew->ptr.macro.callno = macroCurrentCallNo;
      whileNew->ptr.macro.offset = macroPtr;
    }
  else
    {
      whileNew->tag = WhileInFile;
      whileNew->ptr.file.offset = ftell (asmfile);
    }
  whileCurrent = whileNew;
}


static void
whileFree (void)
{
  WhileBlock *p = whileCurrent;
  whileCurrent = p->prev;
  free ((void *)p->expr);
  free (p);
}


static BOOL
whileReEval (void)
{
  Value flag;

  inputThisInstead (whileCurrent->expr);
  exprBuild ();
  flag = exprEval (ValueBool);
  if (flag.Tag.t != ValueBool)
    {
      error (ErrorError, TRUE,
	     "WHILE expression must be boolean (treating as FALSE)");
      return FALSE;
    }
#ifdef DEBUG
  printf("whileReEval() : expr is <%s>\n", flag.ValueBool.b ? "TRUE" : "FALSE");
#endif
  if (flag.ValueBool.b)
    switch (whileCurrent->tag)
      {
      case WhileInFile:
	fseek (asmfile, whileCurrent->ptr.file.offset, SEEK_SET);
	inputLineNo = whileCurrent->lineno;
	return TRUE;
      case WhileInMacro:
	macroPtr = whileCurrent->ptr.macro.offset;
	inputLineNo = whileCurrent->lineno;
	return TRUE;
      default:
	error (ErrorError, TRUE, "Internal whileReEval: unrecognised WHILE type");
	break;
      }
  return FALSE;
}


void
c_wend (Lex * label)
{
  label = label;

  if (!whileCurrent)
    {
      error (ErrorError, TRUE, "Mismatched WEND");
      return;
    }
  switch (whileCurrent->tag)
    {
    case WhileInFile:
    case WhileInMacro:
      if (whileReEval ())
	return;
      break;
    default:
      error (ErrorError, TRUE, "Internal c_wend: unrecognised WHILE type");
      break;
    }
  whileFree ();
}


void
testUnmatched (void)
{
  if (if_depth)
    {
      error (ErrorSerious, TRUE, "Unmatched IF%s", "s" + (if_depth > 1));
      if_depth = 0;
    }

  int i;
  for (i = 0; whileCurrent != NULL; ++i)
    whileFree ();
  if (i)
    error (ErrorSerious, TRUE, "Unmatched WHILE%s", "s" + (i > 1));
}
