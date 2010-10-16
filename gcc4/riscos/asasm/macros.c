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
 * macros.c
 */

#include "config.h"
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "commands.h"
#include "error.h"
#include "filestack.h"
#include "input.h"
#include "macros.h"
#include "os.h"
#include "variables.h"

static Macro *macroList;

static bool Macro_GetLine (char *bufP, size_t bufSize);

void
FS_PopMacroPObject (bool noCheck)
{
  if (gCurPObjP->type != POType_eMacro)
    errorAbort ("Internal FS_PopMacroPObject: unexpected call");

  FS_PopIfWhile (noCheck);

  for (int p = 0; p < MACRO_ARG_LIMIT; ++p)
    free ((void *) gCurPObjP->d.macro.args[p]);

  var_restoreLocals (gCurPObjP->d.macro.varListP);
}


/**
 * Similar to FS_PushFilePObject().
 */
static void
FS_PushMacroPObject (const Macro *m, const char *args[MACRO_ARG_LIMIT])
{
  if (gCurPObjP == &gPOStack[PARSEOBJECT_STACK_SIZE - 1])
    errorAbort ("Maximum file/macro nesting level reached (%d)", PARSEOBJECT_STACK_SIZE);

  gCurPObjP[1].type = POType_eMacro;

  gCurPObjP[1].d.macro.macro = m;
  gCurPObjP[1].d.macro.curPtr = m->buf;
  memcpy (gCurPObjP[1].d.macro.args, args, sizeof (args));
  gCurPObjP[1].d.macro.varListP = NULL;
  
  gCurPObjP[1].name = m->file;
  gCurPObjP[1].lineNum = m->startline;
  
  gCurPObjP[1].if_depth = 0;
  gCurPObjP[1].whilestack = NULL;
  gCurPObjP[1].GetLine = Macro_GetLine;

  /* Increase current file stack pointer.  All is ok now.  */
  ++gCurPObjP;
}


void
macroCall (const Macro *m, const Lex *label)
{
  const char *args[MACRO_ARG_LIMIT];

  int marg = 0;
  if (label->tag == LexId)
    {
      if (m->labelarg)
	{
	  const char *c = label->LexId.str;
	  int len;
	  for (len = (c[0] == '#') ? 1 : 0; isalnum (c[len]) || c[len] == '_'; ++len)
	    /* */;
	  if ((args[marg++] = strndup (label->LexId.str, len)) == NULL)
	    errorOutOfMem();
	}
      else
	{
	  error (ErrorError, "Label argument not allowed");
	  return;
	}
    }
  else if (m->labelarg)
    args[marg++] = NULL;		/* Null label argument */

  skipblanks ();
  while (!inputComment ())
    {
      if (marg == m->numargs)
	{
	  error (ErrorError, "Too many arguments");
	  skiprest ();
	  break;
	}
      inputMark ();
      const char *c;
      int len;
      if (inputLook () == '"')
	{
	  inputSkip ();
	  c = inputSymbol (&len, '"');	/* handles "x\"y", but not "x""y" */
	  inputSkip ();
	  skipblanks ();
	}
      else
	{
	  /*inputRollback(); */
	  c = inputSymbol (&len, ',');
	  while (len > 0 && (c[len - 1] == ' ' || c[len - 1] == '\t'))
	    len--;
	}
      if ((args[marg++] = strndup (c, len)) == NULL)
	errorOutOfMem();
      if (inputLook () == ',')
	inputSkip ();
      else
	break;
      skipblanks ();
    }

  for (/* */; marg < MACRO_ARG_LIMIT; ++marg)
    args[marg] = NULL;

#ifdef DEBUG
  printf ("Macro call = %s\n", inputLine ());
  for (int i = 0; i < MACRO_ARG_LIMIT; ++i)
    printf ("  Arg %i = <%s>\n", i, args[i] ? args[i] : "NULL");
#endif

  FS_PushMacroPObject (m, args);
}


static bool
Macro_GetLine (char *bufP, size_t bufSize)
{
  const char *curPtr = gCurPObjP->d.macro.curPtr;
  
  const char * const bufStartP = bufP;
  const char * const bufEndP = bufP + bufSize - 1;
  while (*curPtr != '\0' && bufP != bufEndP)
    {
      if (MACRO_ARG0 <= *curPtr && *curPtr <= MACRO_ARG15)
	{
	  /* Argument substitution */
	  const char *argP = gCurPObjP->d.macro.args[*curPtr - MACRO_ARG0];
	  if (argP == NULL)
	    argP = "";
	  size_t argLen = strlen (argP);
	  if (bufEndP < bufP + argLen)
	    errorAbort ("Line too long");
	  memcpy (bufP, argP, argLen);
	  bufP += argLen;
	  ++curPtr;
	}
      else if (*curPtr == '\n')
	{
	  ++curPtr;
	  break;
	}
      else
	*bufP++ = *curPtr++;
    }
  *bufP = '\0';

  gCurPObjP->d.macro.curPtr = curPtr;

  return bufP == bufStartP;
}


const Macro *
macroFind (size_t len, const char *name)
{
  for (const Macro *m = macroList; m != NULL; m = m->next)
    {
      if (!strncmp (name, m->name, len) && m->name[len] == '\0')
	return m;
    }
  return NULL;
}


/* Macro builder code */


static bool
c_mend (void)
{
  if (!isspace (inputLook ()))
    return false;

  skipblanks ();
  if (inputGetLower () != 'm')
    return false;
  if (inputGetLower () != 'e')
    return false;
  if (inputGetLower () != 'n')
    return false;
  if (inputGetLower () != 'd')
    return false;
  char c = inputLook ();
  return c == '\0' || isspace (c);
}


void
c_macro (const Lex *label)
{
  Macro m;
  memset(&m, 0, sizeof(Macro));

  char *buf = NULL;

  inputExpand = false;
  if (label->tag != LexNone)
    error (ErrorWarning, "Label not allowed here - ignoring");
  skipblanks ();
  if (!inputComment ())
    error (ErrorWarning, "Skipping characters following MACRO");
  if (!inputNextLine ())
    errorAbort ("End of file found within macro definition");
  if (inputComment ())
    errorAbort ("Missing macro name");
  if (inputLook () == '$')
    inputSkip ();
  int len;
  char *ptr = inputSymbol (&len, 0);
  if (len)
    {
      m.labelarg = m.numargs = 1;
      if ((m.args[0] = strndup (ptr, len)) == NULL)
	errorOutOfMem ();
    }
  skipblanks ();
  if (inputLook () == '|')
    {
      inputSkip ();
      ptr = inputSymbol (&len, '|');
      if (inputGet () != '|')
	error (ErrorError, "Macro name continues over newline");
    }
  else
    ptr = inputSymbol (&len, 0);
  if (!len)
    errorAbort ("Missing macro name");
  if (macroFind (len, ptr))
    {
      error (ErrorError, "Macro %.*s is already defined", len, ptr);
      goto lookforMEND;
    }
  if ((m.name = strndup (ptr, len)) == NULL)
    errorOutOfMem ();
  m.startline = FS_GetCurLineNumber ();
  skipblanks ();
  while (!inputComment ())
    {
      if (m.numargs == MACRO_ARG_LIMIT)
	{
	  error (ErrorError, "Too many arguments in macro definition");
	  skiprest ();
	  break;
	}
      skipblanks ();
      if (inputLook () == '$')
	inputSkip ();
      ptr = inputSymbol (&len, ',');
      if ((m.args[m.numargs++] = strndup (ptr, len)) == NULL)
	errorOutOfMem ();
      if (inputLook () == ',')
	inputSkip ();
    }
  int bufptr = 0, buflen = 0;
  do
    {
      if (!inputNextLine ())
	goto noMEND;
      inputMark ();
      if (c_mend ())
	break;
      inputRollback ();
      char c;
      while ((c = inputGet ()) != 0)
	{
	  inputMark ();
	  if (c == '$')
	    {
	      if (inputLook () == '$')
		inputSkip ();
	      else
		{		/* Token? Check list and substitute */
		  ptr = inputSymbol (&len, '\0');
		  if (inputLook () == '.')
		    inputSkip ();
		  int i;
		  for (i = 0;
		       i < m.numargs
		         && (strlen (m.args[i]) != (size_t)len
		             || memcmp (ptr, m.args[i], len));
		       ++i)
		    /* */;
		  if (i < m.numargs)
		    c = MACRO_ARG0 + i;
		  else
		    {
		      /* error(ErrorWarning, true, "Unknown macro argument encountered"); */
		      inputRollback ();
		    }
		}
	    }
	  if (bufptr + 2 >= buflen)
	    {
	      char *tmp;;
	      if ((tmp = realloc (buf, buflen += 1024)) == NULL)
		errorOutOfMem ();
	      buf = tmp;
	    }
	  buf[bufptr++] = c;
	}
      if (buf)
	buf[bufptr++] = 10;	/* cope with blank line */
    }
  while (1);
  if (buf)
    buf[bufptr] = 0;
  m.file = FS_GetCurFileName ();
  if ((m.buf = buf ? buf : strdup("")) == NULL)
    errorOutOfMem ();

  Macro *p;
  if ((p = malloc (sizeof (Macro))) == NULL)
    errorOutOfMem ();
  *p = m;
  p->next = macroList;
  macroList = p;

  return;

lookforMEND:
  do
    {
      if (!inputNextLine ())
	{
noMEND:
	  errorAbort ("End of file found while looking for MEND");
	  break;
	}
    }
  while (!c_mend ())
    /* */;

  free (buf);
  free ((void *)m.name);
  for (int i = 0; i < MACRO_ARG_LIMIT; ++i)
    free ((void *) m.args[i]);
}


void
c_mexit (const Lex *label)
{
  if (label->tag != LexNone)
    error (ErrorWarning, "Label not allowed here - ignoring");

  if (gCurPObjP->type != POType_eMacro)
    error (ErrorError, "MEXIT found outside a macro");
  else
    FS_PopMacroPObject (false);
}
