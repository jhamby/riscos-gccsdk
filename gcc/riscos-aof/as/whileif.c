/*
 * whileif.c
 * Copyright 1997 Darren Salt
 */
#include "sdk-config.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "commands.h"
#include "decode.h"
#include "error.h"
#include "input.h"
#include "lex.h"
#include "expr.h"
#include "strdup.h"
#include "value.h"
#include "os.h"

#include "whileif.h"

int if_depth = 0;
static BOOL ignore_else = FALSE;

static void 
if_skip (const char *onerror)
{
  char *str;
  int c;
  int nested = 0;
  int tmp_inputExpand = inputExpand;
  inputExpand = FALSE;
  while (inputNextLine ())
   {
      if (inputLook () && !isspace (c = inputGet ()))
	{
	  char del = c == '|' ? c : 0;
	  str = inputSymbol (&c, del, 1);
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


WhileBlock *whileCurrent = 0;

extern FILE *asmfile;
extern long int fileCurrentNo;

extern int exprNotConst;	/* from code.c */


static void 
while_skip (void)
{
  char *str;
  int c;
  int nested = 0;
  while (inputNextLine ())
    {
      if (inputLook () && !isspace (inputGet ()))
	str = inputSymbol (&c, 0, 1);
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
  whileNew = malloc (sizeof (WhileBlock));
  if (whileNew == 0)
    {
    nomem:
      errorOutOfMem ("c_while");
      return;
    }
  /* Copy expression */
  inputRollback ();
  whileNew->expr = strdup (inputRest ());
  if (!whileNew->expr)
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
      whileNew->ptr.file.fileno = fileCurrentNo;
      whileNew->ptr.file.offset = ftell (asmfile);
    }
  whileCurrent = whileNew;
}


void 
whileFree (void)
{
  WhileBlock *p = whileCurrent;
  whileCurrent = p->prev;
  free (p->expr);
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
      }
  return FALSE;
}


void 
c_wend (Lex * label)
{
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
    }
  whileFree ();
}


void 
testUnmatched (void)
{
  int i = 0;
  if (if_depth)
    error (ErrorSerious, TRUE, "Unmatched IF%s", "s" + (if_depth > 1));
  while (whileCurrent)
    {
      i++;
      whileFree ();
    }
  if (i)
    error (ErrorSerious, TRUE, "Unmatched WHILE%s", "s" + (i > 1));
}
