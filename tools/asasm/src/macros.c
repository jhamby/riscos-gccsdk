/*
 * AS an assembler for ARM
 * Copyright (c) 1997 Darren Salt
 * Copyright (c) 2000-2013 GCCSDK Developers
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

static Macro *oMacroListP;

static bool Macro_GetLine (char *bufP, size_t bufSize);
static const Macro *Macro_Find (const char *name, size_t len, bool addSfxWildcard);

void
Macro_PrepareForPhase (Phase_e phase)
{
  switch (phase)
    {
      case eStartUp:
      case ePassOne:
      case ePassTwo:
      case eOutput:
	break;

      case eCleanUp:
	{
	  for (Macro *macroP = oMacroListP; macroP != NULL; /* */)
	    {
	      Macro *nextMacroP = macroP->nextP;
	      free ((void *)macroP->name); macroP->name = NULL;
	      free ((void *)macroP->buf); macroP->buf = NULL;
	      /* Free argument names and default argument values.  */
	      for (unsigned i = 0; i != macroP->numArgs; ++i)
		{
		  free ((void *)macroP->args[i]); macroP->args[i] = NULL;
		  free ((void *)macroP->defArgs[i]); macroP->defArgs[i] = NULL;
		}
	      free (macroP);
	      macroP = nextMacroP;
	    }
	  oMacroListP = NULL;
	  break;
	}
    }
}


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
    Error_Abort ("Maximum file/macro nesting level reached (%d)", PARSEOBJECT_STACK_SIZE);
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


/**
 * \return true when macro can not be found.  false otherwise.
 */
bool
Macro_Call (const char *macroName, size_t macroNameLen, const Lex *label)
{
  const Macro *m = Macro_Find (macroName, macroNameLen, false);
  if (m == NULL)
    return true;

  const char *args[MACRO_ARG_LIMIT];
  unsigned marg = 0;

  /* Handle label argument.  */
  if (label->tag == LexId || label->tag == LexLocalLabel)
    {
      if (m->labelArg)
	{
	  const char *lblP = label->tag == LexId ? label->Data.Id.str : label->Data.LocalLabel.str;
	  size_t lblSize = label->tag == LexId ? label->Data.Id.len : label->Data.LocalLabel.len;
	  if ((args[marg++] = strndup (lblP, lblSize)) == NULL)
	    Error_OutOfMem ();
	}
      else
	{
	  Error (ErrorWarning, "Label argument is ignored by macro %s", m->name);
	  Error_Line (m->fileName, m->startLineNum, ErrorWarning, "note: Marco %s was defined here", m->name);
	}
    }
  else if (m->labelArg)
    args[marg++] = NULL; /* No label argument given.  */

  /* Handle macro suffix argument.  */
  assert (macroNameLen >= m->nameLen);
  if (macroNameLen > m->nameLen)
    {
      /* Macro suffix given.  */
      assert (m->suffixArg);
      if ((args[marg++] = strndup (&macroName[m->nameLen], macroNameLen - m->nameLen)) == NULL)
	Error_OutOfMem ();
    }
  else if (m->suffixArg)
    args[marg++] = NULL; /* No macro suffix given.  */
  
  Input_SkipWS ();
  bool tryEmptyParam = false;
  while (tryEmptyParam || !Input_IsEolOrCommentStart ())
    {
      if (marg == m->numArgs)
	{
	  Error (ErrorError, "Too many macro arguments");
	  Error_Line (m->fileName, m->startLineNum, ErrorWarning, "note: Marco %s was defined here", m->name);
	  return false;
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
	  arg = Input_Symbol2 (&len, ',');
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
	    Error_OutOfMem ();
	}
      if (len == 1 && arg[0] == '|')
	{
	  /* Argument '|' means taking the default argument value.  */
	  free ((void *)arg);
	  arg = strdup (m->defArgs[marg]);
	}
      args[marg++] = arg;
      Input_SkipWS ();
      if (!Input_Match (',', true))
	break;
      /* Following deals with terminating "," character.  */
      tryEmptyParam = true;
    }

  for (/* */; marg != MACRO_ARG_LIMIT; ++marg)
    args[marg] = NULL;

#ifdef DEBUG_MACRO
  printf ("Macro call = %s\n", Input_Line ());
  for (int i = 0; i != MACRO_ARG_LIMIT; ++i)
    printf ("  Arg %i = <%s>\n", i, args[i] ? args[i] : "NULL");
#endif

  FS_PushMacroPObject (m, args);
  return false;
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
		Error_Abort ("Line too long");
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
 * \param addSfxWildcard Match only up to len macro name bytes, i.e. add
 * an implicit wildcard to macroname (name,len).
 * \return When macro is known, return pointer to corresponding Macro
 * object.  NULL when macro is not known.
 */
static const Macro *
Macro_Find (const char *name, size_t len, bool addSfxWildcard)
{
  for (const Macro *mP = oMacroListP; mP != NULL; mP = mP->nextP)
    {
      /* Check length requirement first.  */
      if (mP->nameLen == len
	  || (mP->nameLen < len && mP->suffixArg)
	  || (mP->nameLen > len && addSfxWildcard))
	{
	  size_t charsToMatch = len < mP->nameLen ? len : mP->nameLen;
	  if (!memcmp (name, mP->name, charsToMatch))
	    return mP;
	}
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
  if (!isspace ((unsigned char)Input_Look ()))
    return eMKeyword_AnythingElse;

  Input_SkipWS ();
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
 * Add given argument name and optionally default argument value to given
 * macro object.
 * \param macro Macro object.
 * \param arg Argument name (length terminated).
 * \param argLen Length argument name.
 * \param defValue NULL or malloced string representing default argument value.
 */
static void
AddMacroArg (Macro *macro, const char *arg, size_t argLen, const char *defValue)
{
  for (size_t i = 0; i != macro->numArgs; ++i)
    {
      if (!strncmp (macro->args[i], arg, argLen) && macro->args[i][argLen] == '\0')
	Error (ErrorError, "Macro argument %s is already in use", macro->args[i]);
    }
  assert (macro->numArgs < MACRO_ARG_LIMIT);
  if ((macro->args[macro->numArgs] = strndup (arg, argLen)) == NULL)
    Error_OutOfMem ();
  macro->defArgs[macro->numArgs] = defValue;
  macro->numArgs++;
}


/**
 * Implements MACRO:
 *         MACRO
 * $<lbl> <marco name>[$<suffix>] [$<param>[=<default value>] [, $<param>[=<default value>]]*
 */
bool
c_macro (void)
{
  Macro m;
  memset (&m, 0, sizeof(Macro));

  char *buf = NULL;

  /* Only process macro bodies during the first pass.  */
  if (gPhase == ePassTwo)
    goto lookforMEND;

  Input_SkipWS ();
  if (!Input_IsEolOrCommentStart ())
    Error (ErrorError, "Spurious characters following MACRO");

  /* Process macro prototype statement (= optional label, macro name,
     optionally a suffix and followed by zero or more macro parameters
     separated by a comma).  */

  if (!Input_NextLine (eNoVarSubst))
    Error_Abort ("End of file found within macro definition");

  /* Read optional '$' + label name.  */
  if (Input_Match ('$', false))
    {
      size_t lblLen;
      const char *lbl = Input_Symbol2 (&lblLen, '\0');
      if (lblLen)
	{
	  m.labelArg = true;
	  AddMacroArg (&m, lbl, lblLen, NULL);
	}
    }
  else if (!isspace ((unsigned char)Input_Look ()))
    {
      Error (ErrorError, "Illegal parameter start in macro definition");
      goto lookforMEND;
    }
  Input_SkipWS ();

  /* Read macro name.  */
  size_t macroNameLen;
  const char *macroName;
  if ((macroName = Input_Symbol (&macroNameLen)) == NULL)
    Error_Abort ("Missing macro name");
  if (Input_Match ('$', false))
    {
      const char *suffix;
      size_t suffixLen;
      if ((suffix = Input_Symbol (&suffixLen)) == NULL)
	{
	  Error (ErrorError, "Missing macro name suffix");
	  goto lookforMEND;
	}
      m.suffixArg = true;
      AddMacroArg (&m, suffix, suffixLen, NULL);
    }
  else
    m.suffixArg = false;
  const Macro *prevDefMacro = Macro_Find (macroName, macroNameLen, m.suffixArg);
  if (prevDefMacro != NULL)
    {
      assert ((!m.suffixArg && ((prevDefMacro->nameLen == macroNameLen && !prevDefMacro->suffixArg) || (prevDefMacro->nameLen <= macroNameLen && prevDefMacro->suffixArg)))
              || (m.suffixArg && ((prevDefMacro->nameLen >= macroNameLen && !prevDefMacro->suffixArg) || prevDefMacro->suffixArg)));
      /* Macro definition is matching an already given macro definition
         (possibly via a macro suffix).  */
      if (prevDefMacro->nameLen == macroNameLen && prevDefMacro->suffixArg == m.suffixArg)
	{
	  Error (ErrorError, "Macro '%.*s' is already defined", (int)macroNameLen, macroName);
	  Error_Line (prevDefMacro->fileName, prevDefMacro->startLineNum, ErrorError,
		     "note: Previous macro definition '%s' was here", prevDefMacro->name);
	}
      else if (prevDefMacro->nameLen <= macroNameLen && prevDefMacro->suffixArg)
	{
	  Error (ErrorError, "Macro definition '%.*s' is eclipsed by macro definition '%s' with suffix $%s",
		 (int)macroNameLen, macroName, prevDefMacro->name, prevDefMacro->args[prevDefMacro->labelArg ? 1 : 0]);
	  Error_Line (prevDefMacro->fileName, prevDefMacro->startLineNum, ErrorError,
		     "note: Macro definition '%s' was here", prevDefMacro->name);
	}
      else if (macroNameLen <= prevDefMacro->nameLen && m.suffixArg)
	{
	  Error (ErrorError, "Macro definition '%.*s' with suffix $%s eclipses macro definition '%s'",
		 (int)macroNameLen, macroName, m.args[m.labelArg ? 1 : 0], prevDefMacro->name);
	  Error_Line (prevDefMacro->fileName, prevDefMacro->startLineNum, ErrorError,
		     "note: Macro definition '%s' was here", prevDefMacro->name);
	}
      else
	assert (0);
      goto lookforMEND;
    }
  if ((m.name = strndup (macroName, macroNameLen)) == NULL)
    Error_OutOfMem ();
  m.nameLen = macroNameLen;
  Input_SkipWS ();

  /* Read zero or more macro parameters.  */
  while (!Input_IsEolOrCommentStart ())
    {
      if (m.numArgs == MACRO_ARG_LIMIT)
	{
	  Error (ErrorError, "Too many arguments in macro definition");
	  break;
	}
      if (!Input_Match ('$', false))
	{
	  Error (ErrorError, "Illegal parameter start in macro definition");
	  break;
	}
      const char *arg;
      size_t argLen;
      arg = Input_Symbol (&argLen);
      if (arg == NULL)
	{
	  Error (ErrorError, "Failed to parse macro parameter");
	  break;
	}
      Input_SkipWS ();
      const char *defValue;
      if (Input_Match ('=', false))
	{
	  /* There is a default argument value.  */
	  /* If there is a '"' as start of the default argument value, it needs
	     to be right after the '='.  */
	  if (Input_Match ('"', false))
	    {
	      size_t defValueLen;
	      defValue = Input_GetString (&defValueLen);
	      Input_SkipWS ();
	    }
	  else
	    {
	      /* We do NOT skip spaces, nor do we remove the spaces before
	         the next comma found.  */
	      size_t defValueLen;
	      const char *defValueRaw = Input_Symbol2 (&defValueLen, ',');
	      if ((defValue = strndup (defValueRaw, defValueLen)) == NULL)
		Error_OutOfMem ();
	    }
	}
      else
	defValue = NULL;
      AddMacroArg (&m, arg, argLen, defValue);

      if (!Input_Match (',', true))
	break;
    }
  Decode_FinalCheck ();

  /* Process the macro body.  */
  m.startLineNum = FS_GetCurLineNumber ();
  size_t bufptr = 0, buflen = 128;
  if ((buf = malloc (buflen)) == NULL)
    Error_OutOfMem ();
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
	    Error (ErrorError, "Macro definitions cannot be nested");
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
	  char c = Input_GetC ();
	  if (c == '|')
	    vbar ^= true;
	  else if (c == '$' && !vbar)
	    {
	      const char * const inputMark2 = Input_GetMark ();
	      if (!Input_Match ('$', false))
		{
		  /* Token ? Check list and substitute.  */
		  bool vbar_symbol = Input_Match ('|', false);
		  size_t tokenLen;
		  const char *token = Input_Symbol2 (&tokenLen, '\0');
		  if (vbar_symbol && !Input_Match ('|', false))
		    Error (ErrorError, "Missing vertical bar");
		  (void) Input_Match ('.', false);
		  int i;
		  for (i = 0;
		       i != m.numArgs && (memcmp (token, m.args[i], tokenLen) || m.args[i][tokenLen] != '\0');
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
		Error_OutOfMem ();
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
    Error_OutOfMem ();
  *p = m;
  p->nextP = oMacroListP;
  oMacroListP = p;

  return false;

lookforMEND:
  do
    {
      if (!Input_NextLine (eNoVarSubst))
	{
noMEND:
	  Error_Abort ("End of file found while looking for MEND");
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
    Error (ErrorError, "MEXIT found outside a macro");
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
  Error (ErrorError, "MEND found outside a macro");
  return false;
}
