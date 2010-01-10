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
#include "input.h"
#include "macros.h"
#include "os.h"
#include "variables.h"

#define MACRO_LIMIT (16)
#define MACRO_DEPTH (10)

static Macro *macroList = NULL;

MacroStack macroStack[MACRO_DEPTH];
int macroSP = 0;

const Macro *macroCurrent = NULL;
long int macroCurrentCallNo = 0;
const char *macroPtr = NULL;
char *macroArgs[MACRO_LIMIT] = {0};


static void
macroPop (void)
{
  int p;

  if (!macroSP)
    error (ErrorAbort, FALSE, "Internal macroPop: unexpected call");
  testUnmatched ();
  for (p = MACRO_LIMIT; p; free ((void *) macroArgs[--p]))
    /* */;
  var_restoreLocals ();
  inputLineNo = macroStack[--macroSP].lineno + 1;
  whileCurrent = macroStack[macroSP].whilestack;
  if_depth = macroStack[macroSP].if_depth;
  if (macroSP)
    {
      memcpy (macroArgs, macroStack[macroSP].args, sizeof (macroArgs));
      macroCurrent = macroStack[macroSP].macro;
      macroPtr = macroStack[macroSP].offset;
      macroCurrentCallNo = macroStack[macroSP].callno;
    }
  else
    macroCurrent = NULL;
}


const Macro *
macroFind (size_t len, const char *name)
{
  for (const Macro *m = macroList; m != NULL; m = m->next)
    {
      if (strlen (m->name) == len && !strncmp (name, m->name, len))
	return m;
    }
  return NULL;
}


void
macroCall (const Macro *m, Lex *label)
{
  int i;
  const char *c;
  int len;
  static long int macroCallNo = 0;

  if (macroSP == MACRO_DEPTH)
    {
      error (ErrorSerious, TRUE, "Too many nested macro calls");
      return;
    }
  macroStack[macroSP].macro = macroCurrent;
  macroStack[macroSP].callno = macroCurrentCallNo;
  macroStack[macroSP].offset = macroPtr;
  memcpy (macroStack[macroSP].args, macroArgs, sizeof (macroArgs));
  macroStack[macroSP].whilestack = whileCurrent;
  macroStack[macroSP].if_depth = if_depth;
  macroStack[macroSP++].lineno = inputLineNo;
  macroCurrent = m;
  macroCurrentCallNo = macroCallNo++;
  macroPtr = m->buf;
  inputLineNo = m->startline;
  whileCurrent = 0;
  if_depth = 0;

  for (i = MACRO_LIMIT; i; macroArgs[--i] = 0)
    /* */;
  if (label->tag == LexId)
    {
      if (m->labelarg)
	{
	  c = label->LexId.str;
	  if (c[i] == '#')
	    ++i;
	  while (isalnum (c[i]) || c[i] == '_')
	    i++;
	  if ((macroArgs[0] = strndup (label->LexId.str, i)) == NULL)
	    {
	      errorOutOfMem("macroCall");
	      return;
	    }
	  i = 1;
	}
      else
	{
	  macroPop ();
	  error (ErrorError, TRUE, "Label argument not allowed");
	}
    }
  else if (m->labelarg)
    macroArgs[i++] = 0;		/* Null label argument */

  skipblanks ();
  while (!inputComment ())
    {
      if (i == m->numargs)
	{
	  macroPop ();
	  inputLineNo--;	/* because of the +1 in macroPop() */
	  error (ErrorError, TRUE, "Too many arguments");
	  skiprest ();
	  break;
	}
      inputMark ();
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
      if ((macroArgs[i++] = strndup (c, len)) == NULL)
        {
          errorOutOfMem("macroCall");
	  break;
	}
      if (inputLook () == ',')
	inputSkip ();
      else
	break;
      skipblanks ();
    }
#ifdef DEBUG
  printf ("Macro call = %s\n", inputLine ());
  for (i = 0; i < MACRO_LIMIT; ++i)
    if (macroArgs[i])
      printf ("  Arg %i = <%s>\n", i, macroArgs[i]);
#endif
}


BOOL
macroGetLine (char *buf)	/* returns 0 if already at end of macro */
{
  if (macroPtr == NULL || *macroPtr == '\0')
    {
      macroPop ();
      if (macroSP == 0)
	return FALSE;
    }
  /* Hardcoded buffer size (4K) */
  int p = 0;
  while (1)
    {
      if (*macroPtr == '\0')
	{
	  macroPop ();
	  break;
	}
      if (*macroPtr == '\n')
	{
	  macroPtr++;
	  break;
	}
      if (p == 4095)
	goto linetoolong;
      if (*macroPtr >= MACRO_ARG0 && *macroPtr <= MACRO_ARG15)
	{
	  /* Argument substitution */
	  const char *arg = macroArgs[*macroPtr - MACRO_ARG0];
	  if (arg == 0)
	    arg = "";
	  if (p + strlen (arg) > 4095)
	    goto linetoolong;
	  strcpy (buf + p, arg);
	  p += strlen (arg);
	}
      else
	buf[p++] = *macroPtr;
      macroPtr++;
    }
  buf[p] = 0;
#ifdef DEBUG
  printf ("M-> %s\n", buf);
#endif
  return TRUE;

linetoolong:
  error (ErrorSerious, TRUE, "Line truncated");
  buf[p] = '\0';
  while (*macroPtr != '\n' && *macroPtr != '\0')
    macroPtr++;
  return TRUE;
}


BOOL
macroAdd (Macro * m)
{
  Macro *p;
  if ((p = malloc (sizeof (Macro))) == NULL)
    {
      errorOutOfMem ("macroAdd");
      return FALSE;
    }
  *p = *m;			/* Block copy */
  p->next = macroList;
  macroList = p;
  return TRUE;
}


/* Macro builder code */


static BOOL
c_mend (void)
{
  char c;
  if (isspace (inputLook ()))
    {
      skipblanks ();
      if (inputGetLower () != 'm')
	return FALSE;
      if (inputGetLower () != 'e')
	return FALSE;
      if (inputGetLower () != 'n')
	return FALSE;
      if (inputGetLower () != 'd')
	return FALSE;
      c = inputLook ();
      return c == '\0' || isspace (c);
    }
  else
    return FALSE;
}


void
c_macro (Lex * label)
{
  int len, bufptr = 0, buflen = 0, i;
  char *ptr, *buf = NULL, c;
  Macro m;

  memset(&m, 0, sizeof(Macro));

  inputExpand = FALSE;
  if (label->tag != LexNone)
    error (ErrorWarning, TRUE, "Label not allowed here - ignoring");
  skipblanks ();
  if (!inputComment ())
    error (ErrorWarning, TRUE, "Skipping characters following MACRO");
  if (!inputNextLine ())
    {
      error (ErrorSerious, TRUE, "End of file found within macro definition");
      return;
    }
  if (inputComment ())
    goto missingname;
  if (inputLook () == '$')
    inputSkip ();
  ptr = inputSymbol (&len, 0);
  if (len)
    {
      m.labelarg = m.numargs = 1;
      if ((m.args[0] = strndup (ptr, len)) == NULL)
        {
          errorOutOfMem("c_macro");
	  goto lookforMEND;
	}
    }
  skipblanks ();
  if (inputLook () == '|')
    {
      inputSkip ();
      ptr = inputSymbol (&len, '|');
      if (inputGet () != '|')
	error (ErrorError, TRUE, "Macro name continues over newline");
    }
  else
    ptr = inputSymbol (&len, 0);
  if (!len)
    goto missingname;
  if (macroFind (len, ptr))
    {
      ptr[len] = 0;		/* nasty hack */
      error (ErrorError, TRUE, "Macro %s is already defined", ptr);
      goto lookforMEND;
    }
  if ((m.name = strndup (ptr, len)) == NULL)
    {
      errorOutOfMem("c_macro");
      goto lookforMEND;
    }
  m.startline = inputLineNo;
  skipblanks ();
  while (!inputComment ())
    {
      if (m.numargs == MACRO_LIMIT)
	{
	  error (ErrorError, TRUE, "Too many arguments in macro definition");
	  skiprest ();
	  break;
	}
      skipblanks ();
      if (inputLook () == '$')
	inputSkip ();
      ptr = inputSymbol (&len, ',');
      if ((m.args[m.numargs++] = strndup (ptr, len)) == NULL)
        {
          errorOutOfMem("c_macro");
	  goto lookforMEND;
	}
      if (inputLook () == ',')
	inputSkip ();
    }
  do
    {
      if (!inputNextLine ())
	goto noMEND;
      inputMark ();
      if (c_mend ())
	break;
      inputRollback ();
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
		  i = 0;
		  while (i < m.numargs &&
			 !(strlen (m.args[i]) == (size_t)len && !strncmp (ptr, m.args[i], len))
		    )
		    i++;
		  if (i < m.numargs)
		    c = MACRO_ARG0 + i;
		  else
		    {
		      /*
		         error(ErrorWarning, TRUE, "Unknown macro argument encountered");
		       */
		      inputRollback ();
		    }
		}
	    }
	  if (bufptr + 2 >= buflen)
	    {
	      char *tmp = realloc (buf, buflen += 1024);
	      if (!tmp)
		{
		  errorOutOfMem ("c_macro");
		  free (buf);
		  goto lookforMEND;
		}
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
  m.file = inputName;
  if ((m.buf = buf ? buf : strdup("")) == NULL)
    errorOutOfMem("c_macro");
  macroAdd (&m);
  return;

missingname:
  error (ErrorSerious, TRUE, "Missing macro name");

lookforMEND:
  do
    {
      if (!inputNextLine ())
	{
	noMEND:
	  error (ErrorSerious, TRUE, "End of file found while looking for MEND");
	  break;
	}
    }
  while (!c_mend ());
  free (buf);
  free (m.name);
  for (len = MACRO_LIMIT; len; free ((void *) m.args[--len]));
}


void
c_mexit (Lex * label)
{
  if (label->tag != LexNone)
    error (ErrorWarning, TRUE, "Label not allowed here - ignoring");
  if (macroSP)
    macroPop ();
  else
    error (ErrorSerious, TRUE, "MEXIT found outside a macro");
}
