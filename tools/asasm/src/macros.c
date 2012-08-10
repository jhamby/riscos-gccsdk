/*
 * AS an assembler for ARM
 * Copyright (c) 1997 Darren Salt
 * Copyright (c) 2000-2012 GCCSDK Developers
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

#include <assert.h>
#include <ctype.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif
#include <string.h>
#include <stdlib.h>

#include "common.h"
#include "decode.h"
#include "error.h"
#include "filestack.h"
#include "input.h"
#include "macros.h"
#include "opt.h"
#include "phase.h"
#include "variables.h"

#ifdef DEBUG
//#  define DEBUG_MACRO
#endif

typedef enum
{
  eMKeyword_MACRO,
  eMKeyword_MEND,
  eMKeyword_AnythingElse
} MKeyword_e;

static Macro *macroList;

static bool Macro_GetLine (char *bufP, size_t bufSize);

/**
 * Similar to FS_PopFilePObject().
 */
void
FS_PopMacroPObject (bool noCheck)
{
#ifdef DEBUG_FILESTACK
  ReportFSStack (__func__);
#endif

  assert (gCurPObjP->type == POType_eMacro && "no macro object to pop");

  FS_PopIfWhile (noCheck);

  for (int p = 0; p != MACRO_ARG_LIMIT; ++p)
    {
      free ((void *) gCurPObjP->d.macro.args[p]);
      gCurPObjP->d.macro.args[p] = NULL;
    }

  Var_RestoreLocals (gCurPObjP->d.macro.varListP);
  gOpt_DirectiveValue = gCurPObjP->d.macro.optDirective;
}


/**
 * Similar to FS_PushFilePObject().
 */
static void
FS_PushMacroPObject (const Macro *m, const char *args[MACRO_ARG_LIMIT])
{
#ifdef DEBUG_FILESTACK
  ReportFSStack (__func__);
#endif

  if (gCurPObjP == &gPOStack[PARSEOBJECT_STACK_SIZE - 1])
    errorAbort ("Maximum file/macro nesting level reached (%d)", PARSEOBJECT_STACK_SIZE);
  assert (gCurPObjP != NULL);

  gCurPObjP[1].type = POType_eMacro;

  gCurPObjP[1].d.macro.macro = m;
  gCurPObjP[1].d.macro.curPtr = m->buf;
  memcpy (gCurPObjP[1].d.macro.args, args, sizeof (args)*MACRO_ARG_LIMIT);
  gCurPObjP[1].d.macro.varListP = NULL;
  gCurPObjP[1].d.macro.optDirective = gOpt_DirectiveValue;

  gCurPObjP[1].fileName = m->fileName;
  gCurPObjP[1].lineNum = m->startLineNum;

  gCurPObjP[1].whileIfStartDepth = gCurPObjP[1].whileIfCurDepth = gCurPObjP[0].whileIfCurDepth;
  gCurPObjP[1].GetLine = Macro_GetLine;
  gCurPObjP[1].lastLineSize = 0;

  /* Increase current file stack pointer.  All is ok now.  */
  ++gCurPObjP;
}


void
Macro_Call (const Macro *m, const Lex *label)
{
  const char *args[MACRO_ARG_LIMIT];

  int marg = 0;
  if (label->tag == LexId || label->tag == LexLocalLabel)
    {
      if (m->labelArg)
	{
	  const char *lblP = label->tag == LexId ? label->Data.Id.str : label->Data.LocalLabel.str;
	  size_t lblSize = label->tag == LexId ? label->Data.Id.len : label->Data.LocalLabel.len;
	  if ((args[marg++] = strndup (lblP, lblSize)) == NULL)
	    errorOutOfMem();
	}
      else
	{
	  error (ErrorWarning, "Label argument is ignored by macro %s", m->name);
	  errorLine (m->fileName, m->startLineNum, ErrorWarning, "note: Marco %s was defined here", m->name);
	}
    }
  else if (m->labelArg)
    args[marg++] = NULL; /* Null label argument */
    
  skipblanks ();
  bool tryEmptyParam = false;
  while (tryEmptyParam || !Input_IsEolOrCommentStart ())
    {
      if (marg == m->numArgs)
	{
	  error (ErrorError, "Too many arguments");
	  Input_Rest ();
	  return;
	}
      const char *arg;
      size_t len;
      if (Input_Match ('"', false))
	{
	  /* Double quoted argument.  */
	  arg = Input_GetString (&len);
	}
      else
	{
	  /* Unquoted argument.  */
	  arg = inputSymbol (&len, ',');
	  /* Discard comment start.  */
	  for (size_t i = 0; i != len; ++i)
	    if (arg[i] == ';')
	      {
	        len = i;
		break;
	      }
	  /* Discard the white space characters before comma.  */
	  while (len != 0 && isspace ((unsigned char)arg[len - 1]))
	    len--;
	  if ((arg = strndup (arg, len)) == NULL)
	    errorOutOfMem();
	}
      if (len == 1 && arg[0] == '|')
	{
	  /* Argument '|' means taking the default argument value.  */
	  free ((void *)arg);
	  arg = strdup (m->defArgs[marg]);
	}
      args[marg++] = arg;
      skipblanks ();
      if (!Input_Match (',', true))
	break;
      /* Following deals with terminating "," character.  */
      tryEmptyParam = true;
    }

  for (/* */; marg != MACRO_ARG_LIMIT; ++marg)
    args[marg] = NULL;

#ifdef DEBUG_MACRO
  printf ("Macro call = %s\n", inputLine ());
  for (int i = 0; i != MACRO_ARG_LIMIT; ++i)
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

  if (*curPtr == '\0')
    {
      gCurPObjP->lastLineSize = 0;
      return true;
    }

  const char * const bufEndP = bufP + bufSize - 1;
  while (*curPtr != '\0' && bufP != bufEndP)
    {
      if (MACRO_ARG0 <= *curPtr && *curPtr < MACRO_ARG0 + MACRO_ARG_LIMIT)
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
  /* lastLineSize is the difference of our input curPtr when reading one
     macro line, *not* the difference of our output bufP.  */
  gCurPObjP->lastLineSize = curPtr - gCurPObjP->d.macro.curPtr;

  gCurPObjP->d.macro.curPtr = curPtr;

  return false;
}


/**
 * Find macro with given name.
 * \param name Macro name to look for, not NUL terminated.
 * \param len Macro name length.
 * \return When macro is known, return pointer to corresponding Macro
 * object.  NULL when macro is not known.
 */
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


/**
 * \return Keyword indication ("MACRO", "MEND" or anything else/no keyword).
 * When MACRO or MEND keyword are parsed, those are consumed.  Any leading
 * white space characters are always consumed.
 */
static MKeyword_e
Macro_GetKeyword (void)
{
  if (!isspace ((unsigned char)inputLook ()))
    return eMKeyword_AnythingElse;

  skipblanks ();
  /* We only need to check for "MEND" and the end of keyword (i.e. a space,
     start comment character (';') or EOL).  Upon return from Macro_Call()
     in decode(), decode_finalcheck() will deal with the rest of the line
     after "MEND".  */
  if (Input_MatchKeyword ("MEND"))
    return eMKeyword_MEND;
  if (Input_MatchKeyword ("MACRO"))
    return eMKeyword_MACRO;
  return eMKeyword_AnythingElse;
}


/**
 * Implements MACRO:
 *         MACRO
 * $<lbl> <marco name> [$<param>[=<default value>]]*
 */
bool
c_macro (void)
{
  Macro m;
  memset (&m, 0, sizeof(Macro));

  char *buf = NULL;

  if (gPhase == ePassTwo)
    goto lookforMEND;
  
  skipblanks ();
  if (!Input_IsEolOrCommentStart ())
    error (ErrorWarning, "Skipping characters following MACRO");

  /* Process macro prototype statement (= optional label, macro name,
     zero or more macro parameters).  */
  /* Read optional '$' + label name.  */
  if (!Input_NextLine (eNoVarSubst))
    errorAbort ("End of file found within macro definition");
  if (Input_Match ('$', false))
    {
      size_t len;
      const char *ptr = inputSymbol (&len, '\0');
      if (len)
	{
	  m.labelArg = true;
	  m.numArgs = 1;
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
      errorLine (prevDefMacro->fileName, prevDefMacro->startLineNum, ErrorError,
		 "note: Previous definition of macro '%.*s' was here", (int)len, ptr);
      goto lookforMEND;
    }
  if ((m.name = strndup (ptr, len)) == NULL)
    errorOutOfMem ();
  skipblanks ();

  /* Read zero or more macro parameters.  */
  while (!Input_IsEolOrCommentStart ())
    {
      if (m.numArgs == MACRO_ARG_LIMIT)
	{
	  error (ErrorError, "Too many arguments in macro definition");
	  Input_Rest ();
	  break;
	}
      if (!Input_Match ('$', false))
	{
	  error (ErrorError, "Illegal parameter start in macro definition");
	  Input_Rest ();
	  break;
	}
      ptr = Input_Symbol (&len);
      if (ptr == NULL)
	{
	  error (ErrorError, "Failed to parse macro parameter");
	  Input_Rest ();
	  break;
	}
      if ((m.args[m.numArgs] = strndup (ptr, len)) == NULL)
	errorOutOfMem ();
      skipblanks ();
      if (Input_Match ('=', false))
	{
	  /* There is a default argument value.  */
	  const char *defarg;
	  /* If there is a '"' as start of the default argument value, it needs
	     to be right after the '='.  */
	  if (Input_Match ('"', false))
	    {
	      size_t defarglen;
	      defarg = Input_GetString (&defarglen);
	      skipblanks ();
	    }
	  else
	    {
	      /* We do NOT skip spaces, nor do we remove the spaces before
	         the next comma found.  */
	      size_t defarglen;
	      defarg = inputSymbol (&defarglen, ',');
	      if ((defarg = strndup (defarg, defarglen)) == NULL)
		errorOutOfMem ();
	    }
	  m.defArgs[m.numArgs] = defarg;
	}
      else
	m.defArgs[m.numArgs] = NULL;
      ++m.numArgs;
      if (!Input_Match (',', true))
	break;
    }
  decode_finalcheck ();

  /* Process the macro body.  */
  m.startLineNum = FS_GetCurLineNumber ();
  size_t bufptr = 0, buflen = 128;
  if ((buf = malloc (buflen)) == NULL)
    errorOutOfMem ();
  do
    {
      if (!Input_NextLine (eNoVarSubst))
	goto noMEND;

      const char * const inputMark = Input_GetMark ();
      bool isMacroEnd = false;
      switch (Macro_GetKeyword ())
	{
	  case eMKeyword_MEND:
	    isMacroEnd = true;
	    break;

	  case eMKeyword_MACRO:
	    /* Nested MACRO construction, that's not allowed.  */
	    error (ErrorError, "Macro definitions cannot be nested");
	    /* Fall through.  */

	  case eMKeyword_AnythingElse:
	    Input_RollBackToMark (inputMark);
	    break;
	}
      if (isMacroEnd)
	break;

      bool vbar = false; /* true when there is an unmatched | character.  */
      while (1)
	{
	  char c = inputGet ();
	  if (c == '|')
	    vbar ^= true;
	  else if (c == '$' && !vbar)
	    {
	      const char * const inputMark2 = Input_GetMark ();
	      if (!Input_Match ('$', false))
		{
		  /* Token ? Check list and substitute.  */
		  bool vbar_symbol = Input_Match ('|', false);
		  ptr = inputSymbol (&len, '\0');
		  if (vbar_symbol && !Input_Match ('|', false))
		    error (ErrorError, "Missing vertical bar");
		  (void) Input_Match ('.', false);
		  int i;
		  for (i = 0;
		       i != m.numArgs
			 && (memcmp (ptr, m.args[i], len) || m.args[i][len] != '\0');
		       ++i)
		    /* */;
		  if (i != m.numArgs)
		    c = MACRO_ARG0 + i;
		  else
		    Input_RollBackToMark (inputMark2);
		}
	    }
	  /* Ensure there is always at least space for 2 extra characters.  */
	  if (bufptr + 2 >= buflen)
	    {
	      char *tmp;
	      if ((tmp = realloc (buf, buflen += 1024)) == NULL)
		errorOutOfMem ();
	      buf = tmp;
	    }
	  if (c == '\0')
	    {
	      buf[bufptr++] = '\n';
	      break;
	    }
	  buf[bufptr++] = c;
	}
    } while (1);
  buf[bufptr] = '\0';
  m.fileName = FS_GetCurFileName ();
  m.buf = buf;

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
      if (!Input_NextLine (eNoVarSubst))
	{
noMEND:
	  errorAbort ("End of file found while looking for MEND");
	  break;
	}
    }
  while (Macro_GetKeyword () != eMKeyword_MEND);

  free (buf);
  free ((void *)m.name);
  for (int i = 0; i != MACRO_ARG_LIMIT; ++i)
    free ((void *) m.args[i]);
  return false;
}


/**
 * Implements MEXIT.
 */
bool
c_mexit (void)
{
  if (gCurPObjP->type != POType_eMacro)
    error (ErrorError, "MEXIT found outside a macro");
  else
    FS_PopPObject (true);
  return false;
}


/**
 * Implements MEND (but without MACRO start).
 */
bool
c_mend (void)
{
  error (ErrorError, "MEND found outside a macro");
  return false;
}
