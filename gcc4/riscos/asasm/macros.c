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
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "commands.h"
#include "decode.h"
#include "error.h"
#include "filestack.h"
#include "input.h"
#include "macros.h"
#include "os.h"
#include "variables.h"

#ifdef DEBUG
//#  define DEBUG_MACRO
#endif

static Macro *macroList;

static bool Macro_GetLine (char *bufP, size_t bufSize);

/**
 * Similar to FS_PopFilePObject().
 */
void
FS_PopMacroPObject (bool noCheck)
{
  if (gCurPObjP->type != POType_eMacro)
    errorAbort ("Internal FS_PopMacroPObject: unexpected call");

  FS_PopIfWhile (noCheck);

  for (int p = 0; p < MACRO_ARG_LIMIT; ++p)
    {
      free ((void *) gCurPObjP->d.macro.args[p]);
      gCurPObjP->d.macro.args[p] = NULL;
    }

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
  memcpy (gCurPObjP[1].d.macro.args, args, sizeof (args)*MACRO_ARG_LIMIT);
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
Macro_Call (const Macro *m, const Lex *label)
{
  const char *args[MACRO_ARG_LIMIT];

  int marg = 0;
  if (label->tag == LexId)
    {
      if (m->labelarg)
	{
	  const char *c = label->Data.Id.str;
	  size_t len;
	  for (len = (c[0] == '#') ? 1 : 0; isalnum (c[len]) || c[len] == '_'; ++len)
	    /* */;
	  if ((args[marg++] = strndup (label->Data.Id.str, len)) == NULL)
	    errorOutOfMem();
	}
      else
	error (ErrorWarning, "Label argument is ignored by macro %s", m->name);
    }
  else if (m->labelarg)
    args[marg++] = NULL;		/* Null label argument */

  skipblanks ();
  bool tryEmptyParam = false;
  while (tryEmptyParam || !Input_IsEolOrCommentStart ())
    {
      if (marg == m->numargs)
	{
	  error (ErrorError, "Too many arguments");
	  skiprest ();
	  return;
	}
      const char *arg;
      size_t len;
      if (Input_Match ('"', false))
	{
	  arg = inputSymbol (&len, '"');	/* FIXME: handles "x\"y", but not "x""y" */
	  inputSkip ();
	}
      else
	{
	  arg = inputSymbol (&len, ',');
	  /* Discard the white space characters before comma.  */
	  while (len != 0 && isspace ((unsigned char)arg[len - 1]))
	    len--;
	}
      if (len == 1 && arg[0] == '|')
	{
	  arg = m->defArgs[marg];
	  len = strlen (arg);
	}
      if ((args[marg] = strndup (arg, len)) == NULL)
	errorOutOfMem();
      ++marg;
      skipblanks ();
      if (!Input_Match (',', true))
	break;
      /* Following deals with terminating "," character.  */
      tryEmptyParam = true;
    }

  for (/* */; marg < MACRO_ARG_LIMIT; ++marg)
    args[marg] = NULL;

#ifdef DEBUG_MACRO
  printf ("Macro call = %s\n", inputLine ());
  for (int i = 0; i < MACRO_ARG_LIMIT; ++i)
    printf ("  Arg %i = <%s>\n", i, args[i] ? args[i] : "NULL");
#endif

  FS_PushMacroPObject (m, args);
}

/**
 * Called during macro expansion, called line-per-line of the macro.
 * \param bufP Buffer to fill
 * \param bufSize Buffer size
 * \return true for failure or end of data (i.e. end of macro), false otherwise.
 */
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
	  /* Argument substitution.  */
	  const char *argP = gCurPObjP->d.macro.args[*curPtr - MACRO_ARG0];
	  if (argP != NULL)
	    {
	      size_t argLen = strlen (argP);
	      if (bufEndP < bufP + argLen)
		errorAbort ("Line too long");
	      memcpy (bufP, argP, argLen);
	      bufP += argLen;
	    }
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
Macro_Find (const char *name, size_t len)
{
  for (const Macro *m = macroList; m != NULL; m = m->next)
    {
      if (!memcmp (name, m->name, len) && m->name[len] == '\0')
	return m;
    }
  return NULL;
}


/* Macro builder code */

/**
 * \return true when MEND is read.
 */
static bool
c_mend (void)
{
  if (!isspace ((unsigned char)inputLook ()))
    return false;

  skipblanks ();
  if (inputGet () != 'M' || inputGet () != 'E'
      || inputGet () != 'N' || inputGet () != 'D')
    return false;
  /* We only need to check for EOL or space.  Upon return from Macro_Call()
     in decode(), decode_finalcheck() will deal with this.  */
  char c = inputLook ();
  return c == '\0' || isspace ((unsigned char)c);
}


/**
 * Processes:
 *         MACRO
 * $<lbl> <marco name> [$<param1>[=<default value>]]*
 */
bool
c_macro (const Lex *label)
{
  if (label->tag != LexNone)
    error (ErrorWarning, "Label not allowed here - ignoring");

  Macro m;
  memset (&m, 0, sizeof(Macro));

  char *buf = NULL;

  skipblanks ();
  if (!Input_IsEolOrCommentStart ())
    error (ErrorWarning, "Skipping characters following MACRO");

  /* Read optional '$' + label name.  */
  if (!inputNextLineNoSubst ())
    errorAbort ("End of file found within macro definition");
  if (Input_Match ('$', false))
    {
      size_t len;
      const char *ptr = inputSymbol (&len, 0);
      if (len)
	{
	  m.labelarg = m.numargs = 1;
	  if ((m.args[0] = strndup (ptr, len)) == NULL)
	    errorOutOfMem ();
	}
    }
  else if (!isspace ((unsigned char)inputLook ()))
    {
      error (ErrorError, "Illegal parameter start in macro definition");
      goto lookforMEND;
    }
  skipblanks ();

  /* Read macro name.  */
  size_t len;
  const char *ptr;
  if ((ptr = Input_Symbol (&len)) == NULL)
    errorAbort ("Missing macro name");
  const Macro *prevDefMacro = Macro_Find (ptr, len);
  if (prevDefMacro != NULL)
    {
      error (ErrorError, "Macro '%.*s' is already defined", (int)len, ptr);
      errorLine (prevDefMacro->file, prevDefMacro->startline, ErrorError,
		 "note: Previous definition of macro '%.*s' was here", (int)len, ptr);
      goto lookforMEND;
    }
  if ((m.name = strndup (ptr, len)) == NULL)
    errorOutOfMem ();
  m.startline = FS_GetCurLineNumber ();
  skipblanks ();

  /* Read zero or more macro parameters.  */
  while (!Input_IsEolOrCommentStart ())
    {
      if (m.numargs == MACRO_ARG_LIMIT)
	{
	  error (ErrorError, "Too many arguments in macro definition");
	  skiprest ();
	  break;
	}
      if (!Input_Match ('$', false))
	{
	  error (ErrorError, "Illegal parameter start in macro definition");
	  skiprest ();
	  break;
	}
      ptr = Input_Symbol (&len);
      if (ptr == NULL)
	{
	  error (ErrorError, "Failed to parse macro parameter");
	  skiprest ();
	  break;
	}
      if ((m.args[m.numargs] = strndup (ptr, len)) == NULL)
	errorOutOfMem ();
      skipblanks ();
      if (Input_Match ('=', false))
	{
	  /* There is a default argument value.  */
	  const char *defarg;
	  size_t defarglen;
	  /* If there is a '"' as start of the default argument value, it needs
	     to be right after the '='.  */
	  if (Input_Match ('"', false))
	    {
	      defarg = inputSymbol (&defarglen, '"');	/* FIXME: handles "x\"y", but not "x""y" */
	      inputSkip ();
	      skipblanks ();
	    }
	  else
	    {
	      /* We do NOT skip spaces, nor do we remove the spaces before
	         the next comma found.  */
	      defarg = inputSymbol (&defarglen, ',');
	    }
	  if ((m.defArgs[m.numargs] = strndup (defarg, defarglen)) == NULL)
	    errorOutOfMem ();
	}
      else
	m.defArgs[m.numargs] = NULL;
      ++m.numargs;
      if (!Input_Match (',', true))
	break;
    }
  decode_finalcheck ();

  /* Process the macro body.  */
  int bufptr = 0, buflen = 0;
  do
    {
      if (!inputNextLineNoSubst ())
	goto noMEND;

      const char * const inputMark = Input_GetMark ();
      if (c_mend ())
	break;
      Input_RollBackToMark (inputMark);

      char c;
      while ((c = inputGet ()) != 0)
	{
	  const char * const inputMark2 = Input_GetMark ();
	  if (c == '$')
	    {
	      if (!Input_Match ('$', false))
		{ /* Token? Check list and substitute */
		  ptr = inputSymbol (&len, '\0');
		  Input_Match ('.', false);
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
		      Input_RollBackToMark (inputMark2);
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

  return false;

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
  return false;
}


/**
 * Implements MEXIT.
 */
bool
c_mexit (const Lex *label)
{
  if (label->tag != LexNone)
    error (ErrorWarning, "Label not allowed here - ignoring");

  if (gCurPObjP->type != POType_eMacro)
    error (ErrorError, "MEXIT found outside a macro");
  else
    FS_PopMacroPObject (false);
  return false;
}
