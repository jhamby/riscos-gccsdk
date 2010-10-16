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

static bool ignore_else = false;

static void if_skip (const char *onerror);
static void while_skip (void);
static void whileFree (void);
static bool whileReEval (void);

static void
if_skip (const char *onerror)
{
  int c;
  int nested = 0;
  int tmp_inputExpand = inputExpand;
  inputExpand = false;
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
  error (ErrorError, "%s", onerror);
  inputExpand = tmp_inputExpand;
  return;

skipped:
  ignore_else = true;
  inputRewind = true;
  inputExpand = tmp_inputExpand;
}


void
c_if (void)
{
  gCurPObjP->if_depth++;

  exprBuild ();
  Value flag = exprEval (ValueBool);
  if (flag.Tag.t != ValueBool)
    {
      error (ErrorError, "IF expression must be boolean (treating as true)");
      return;
    }
  if (!flag.ValueBool.b)
    if_skip ("No matching | or ]");
}


void
c_else (Lex *label)
{
  if (!gCurPObjP->if_depth)
    error (ErrorError, "Mismatched |");
  if (label->tag != LexNone)
    {
      error (ErrorError, "Label not allowed with |");
      ignore_else = false;
      return;
    }
  if (!ignore_else)
    if_skip ("No matching ]");
  ignore_else = false;
}


void
c_endif (Lex *label)
{
  if (!gCurPObjP->if_depth)
    errorAbort ("Mismatched ]");
  else
    gCurPObjP->if_depth--;
  if (label->tag != LexNone)
    error (ErrorError, "Label not allowed with ]");
  ignore_else = false;
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
		if (!(notinput ("nd") || (inputLook () && !isspace (inputGet ())))
		    && nested-- == 0)
		  return;
	        break;
	    }
	}
      skiprest ();
    }
  error (ErrorError, "Missing WEND");
}



void
c_while (Lex *label)
{
  label = label;

  inputMark ();
  /* Evaluate expression */
  exprBuild ();
  Value flag = exprEval (ValueBool);
  if (flag.Tag.t != ValueBool)
    {
      error (ErrorError, "WHILE expression must be boolean (treating as false)");
      while_skip ();
      return;
    }
  if (!exprNotConst)
    {
      error (ErrorError, "WHILE expression is constant (treating as false)");
      while_skip ();
      return;
    }
#ifdef DEBUG
  printf("c_while() : expr is <%s>\n", flag.ValueBool.b ? "true" : "false");
#endif
  if (!flag.ValueBool.b)
    {
      while_skip ();
      return;
    }

  WhileBlock *whileNew;
  if ((whileNew = malloc (sizeof (WhileBlock))) == NULL)
    errorOutOfMem ();
  /* Copy expression */
  inputRollback ();
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
  exprBuild ();
  Value flag = exprEval (ValueBool);
  if (flag.Tag.t != ValueBool)
    {
      error (ErrorError, "WHILE expression must be boolean (treating as false)");
      return false;
    }
#ifdef DEBUG
  printf("whileReEval() : expr is <%s>\n", flag.ValueBool.b ? "true" : "false");
#endif
  if (flag.ValueBool.b)
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


void
c_wend (Lex *label)
{
  label = label;

  if (!gCurPObjP->whilestack)
    {
      error (ErrorError, "Mismatched WEND");
      return;
    }
  switch (gCurPObjP->whilestack->tag)
    {
      case WhileInFile:
      case WhileInMacro:
	if (whileReEval ())
	  return;
	break;
      default:
	errorAbort ("Internal c_wend: unrecognised WHILE type");
	break;
    }
  whileFree ();
}


void
FS_PopIfWhile (bool noCheck)
{
  if (gCurPObjP->if_depth)
    {
      if (!noCheck)
	errorAbort ("Unmatched IF%s", "s" + (gCurPObjP->if_depth > 1));
      gCurPObjP->if_depth = 0;
    }

  int i;
  for (i = 0; gCurPObjP->whilestack != NULL; ++i)
    whileFree ();
  if (i && !noCheck)
    errorAbort ("Unmatched WHILE%s", "s" + (i > 1));
}
