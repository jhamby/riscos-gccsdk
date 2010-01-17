/*
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2002-2010 GCCSDK Developers
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
 * error.c
 */

#include "config.h"
#include <setjmp.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "error.h"
#include "filestack.h"
#include "input.h"
#include "macros.h"
#include "main.h"
#include "os.h"

#define MAXERR (100)

const char InsertCommaAfter[] = "Inserting missing comma after ";

static int no_errors = 0;
static int no_warnings = 0;
static const char *source = NULL; /* Can remain NULL when reading from stdin */

#ifdef __riscos__
static int ThrowbackStarted;
#endif

static char errbuf[2048];

void
errorInit (const char *filename)
{
  source = (filename == NULL || !strcmp(filename, "-")) ? NULL : filename;
}


void
errorFinish (void)
{
#ifdef __riscos__
  if (ThrowbackStarted > 0)
    {
      _kernel_oserror *err;
      if ((err = ThrowbackEnd ()) != NULL && option_verbose > 1)
        fprintf (stderr, "ThrowbackEnd error: %s\n", err->errmess);
    }
#endif
}

#ifdef __riscos__
static void
TB (int level, long int lineno, const char *errstr, const char *file)
{
  if (!option_throwback || source == NULL)
    return;

  if (!ThrowbackStarted)
    ThrowbackStarted = ThrowbackStart () ? -1 : 1;

  if (ThrowbackStarted == 1)
    {
      _kernel_oserror *err;

      if ((err = ThrowbackSendStart (file)) != NULL && option_verbose > 1)
        fprintf (stderr, "ThrowbackSendStart error: %s\n", err->errmess);
      if ((err = ThrowbackSendError (level, lineno, errstr)) != NULL && option_verbose > 1)
        fprintf (stderr, "ThrowbackSendError error: %s\n", err->errmess);
    }
}
#else
#define TB(x,y,z,f)
#endif


int
returnExitStatus (void)
{
  return no_errors ? EXIT_FAILURE : EXIT_SUCCESS;
}


static void
fixup (ErrorTag t)
{
  skiprest ();
  if (t == ErrorAbort || no_errors > MAXERR || !asmContinueValid)
    {
      if (asmAbortValid)
        longjmp (asmAbort, 1);
    }
  else
    longjmp (asmContinue, 1);
}

static void
doline (int t, long int line, int sameline)
{
#ifndef __riscos__
  t = t;
#endif

  if (line > 0)
    {
      TB (t, line, errbuf, inputName);
      if (get_file (NULL, NULL))
	{
	  const char *nfile;
	  long int diffline = 0, nline, i;
	  do
	    {
	      fprintf (stderr, "%s at line %li", diffline ? "\n " : (" " + sameline), line);
	      if ((i = get_file (&nfile, &nline)) != 0)
		fprintf (stderr, " in file '%s'", nfile);
	      if (diffline)
		{
		  sprintf (errbuf, "  included from here");
		  /* I'd like to be able to put
		   *   t=ThrowbackInfo;
		   * here without Zap spewing forth "Information for file"... :-|
		   */
		  TB (t, line, errbuf, nfile);
		}
	      diffline = 1;
	      sameline = 0;
	      line = nline;
	    }
	  while (i);
	  fputc ('\n', stderr);
	}
    }
  else
    {
      fprintf (stderr, "  at line %li\n", line);
      TB (t, line < 0 ? -line : line, errbuf, inputName);
    }
}

void
error (ErrorTag e, BOOL c, const char *format,...)
{
  const char *str;
  int t = 0;
  switch (e)
    {
      case ErrorInfo:
        str = "Info";
#ifdef __riscos__
        t = ThrowbackInfo;
#endif
        break;
      case ErrorWarning:
        str = "Warning";
        no_warnings++;
#ifdef __riscos__
        t = ThrowbackWarning;
#endif
        break;
      case ErrorError:
        str = "Error";
        no_errors++;
#ifdef __riscos__
        t = ThrowbackError;
#endif
        break;
      default:
        str = "Serious error";
        no_errors++;
#ifdef __riscos__
        t = ThrowbackSeriousError;
#endif
        break;
    }
  va_list ap;
  va_start (ap, format);
  vsprintf (errbuf, format, ap);
  va_end (ap);
  fprintf (stderr, "%s: %s", str, errbuf);
  long int line = inputLineNo;
  if (macroSP)
    {
      int i = macroSP;
      const char *name = macroCurrent->name;
      const char *file = macroCurrent->file;
      BOOL sameline = TRUE;
      do
	{
	  TB (t, line, errbuf, file);
	  fprintf (stderr, "%s at line %li in macro %s", " " + sameline, line, name);
	  if (file != inputName)
	    fprintf (stderr, " in file '%s'\n", file);
	  else
	    fputc ('\n', stderr);
	  sameline = FALSE;
	  /* I'd like to be able to put
	   *   t=ThrowbackInfo;
	   * here without Zap spewing forth "Information for file"... :-|
	   */
	  if (!--i)
	    break;
	  name = macroStack[i].macro->name;
	  file = macroStack[i].macro->file;
	  line = macroStack[i].lineno;
	  sprintf (errbuf, i ? "  in macro %s" : "  called from here", name);
	}
      while (i);
      line = macroStack[0].lineno;
    }
  doline (t, line, !macroSP);
#ifdef DEBUG
  printf ("--> %s\n", inputLine ());
#endif
  if (!c || no_errors > MAXERR)
    fixup (e);
}


void
errorLine (long int lineno, const char *file,
	   ErrorTag e, BOOL c, const char *format,...)
{
  const char *str;
#ifdef __riscos__
  int t;
#endif
  switch (e)
    {
      case ErrorInfo:
        str = "Info";
#ifdef __riscos__
        t = ThrowbackInfo;
#endif
        break;
      case ErrorWarning:
        str = "Warning";
#ifdef __riscos__
        t = ThrowbackWarning;
#endif
        no_warnings++;
        break;
      case ErrorError:
        str = "Error";
#ifdef __riscos__
        t = ThrowbackError;
#endif
        no_errors++;
        break;
      default:
        str = "Fatal error";
#ifdef __riscos__
        t = ThrowbackSeriousError;
#endif
        no_errors++;
        break;
    }

  va_list ap;
  va_start (ap, format);
  vsprintf (errbuf, format, ap);
  va_end (ap);
  fprintf (stderr, "%s: %s", str, errbuf);
  if (lineno > 0)
    fprintf (stderr, " at line %li", lineno);
  if (!file)
    file = inputName;
  if (file != inputName && source != NULL)
    fprintf (stderr, " in file '%s'\n", source);
  else
    fputc ('\n', stderr);
  TB (t, lineno, errbuf, file);
  if (!c || no_errors > MAXERR)
    fixup (e);
}


void
errorOutOfMem (const char *fn)
{
  error (ErrorSerious, FALSE, "Internal %s: out of memory", fn);
}
