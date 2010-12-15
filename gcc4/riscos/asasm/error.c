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
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
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

#ifdef __riscos__
static int ThrowbackStarted;
#endif

static char errbuf[2048];


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


static void
DoThrowback (int level, int lineno, const char *errstr, const char *file)
{
#ifdef __riscos__
  if (!option_throwback)
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
#else
  level = level;
  lineno = lineno;
  errstr = errstr;
  file = file;
#endif
}


int
returnExitStatus (void)
{
  return no_errors ? EXIT_FAILURE : EXIT_SUCCESS;
}


static void __attribute__ ((noreturn))
fixup (void)
{
  if (!asmContinueValid)
    {
      if (asmAbortValid)
        longjmp (asmAbort, 1);
      else
	abort ();	
    }
  else
    longjmp (asmContinue, 1);
}


static void
errorCore (ErrorTag e, const char *format, va_list ap)
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
      case ErrorAbort:
      default:
        str = "Fatal error";
        no_errors++;
#ifdef __riscos__
        t = ThrowbackSeriousError;
#endif
        break;
    }
  vsnprintf (errbuf, sizeof (errbuf), format, ap);
  if (gCurPObjP != NULL)
    {
      const PObject *pObjP = gCurPObjP;
      switch (pObjP->type)
	{
	  case POType_eFile:
	    /* When lineNum is zero, we're processing the -PD options.  */
	    if (pObjP->lineNum != 0)
	      fprintf (stderr, "%s:%d:%zd: %s: %s\n",
		       pObjP->name ? pObjP->name : "{standard input}",
	               pObjP->lineNum,
		       Input_GetColumn (),
		       str, errbuf);
	    else
	      fprintf (stderr, "%s: %s\n", str, errbuf);
	    break;
	  case POType_eMacro:
	    fprintf (stderr, "%s:%d:%zd: %s: %s in macro %s\n",
		     pObjP->name ? pObjP->name : "{standard input}",
	             pObjP->lineNum,
		     Input_GetColumn (),
		     str, errbuf,
		     pObjP->d.macro.macro->name);
	    break;
	}

      if (pObjP->name)
	DoThrowback (t, pObjP->lineNum, errbuf, pObjP->name);

      while (pObjP != &gPOStack[0])
	{
	  --pObjP;
	  switch (pObjP->type)
	    {
	      case POType_eFile:
		/* When lineNum is zero, we're processing the -PD options.  */
		if (pObjP->lineNum != 0)
		  fprintf (stderr, "  called from line %d from file %s\n",
		           pObjP->lineNum, pObjP->name ? pObjP->name : "stdout");
		else
		  fputc ('\n', stderr);
		break;
	      case POType_eMacro:
		fprintf (stderr, "  called from macro %s at line %d in file %s\n",
			 pObjP->d.macro.macro->name, pObjP->lineNum,
			 pObjP->name ? pObjP->name : "stdout");
		break;
	    }
	  if (pObjP->name)
	    DoThrowback (t, pObjP->lineNum, errbuf, pObjP->name);
	}
    }
  else
    fprintf (stderr, "%s: %s\n", str, errbuf);
}


/**
 * ErrorAbort or too many ErrorError won't make this function return.
 * ErrorError will consume the rest of the line and set the current
 * input pointer to a NUL character.
 */
void
error (ErrorTag e, const char *format, ...)
{
  va_list ap;
  va_start (ap, format);
  errorCore (e, format, ap);
  va_end (ap);

  Input_ShowLine ();

  if (e == ErrorAbort || no_errors > MAXERR)
    fixup ();
  else if (e == ErrorError)
    skiprest ();
}


/**
 * Gives fatal error and does not return.
 */
void
errorAbort (const char *format, ...)
{
  va_list ap;
  va_start (ap, format);
  errorCore (ErrorAbort, format, ap);
  va_end (ap);

  Input_ShowLine ();

  fixup ();
}


/**
 * Gives fatal out-of-memory error and does not return.
 */
void
errorOutOfMem (void)
{
  errorAbort ("Out of memory");
}


static void
errorCoreLine (const char *file, int lineno, ErrorTag e,
	       const char *format, va_list ap)
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
      case ErrorAbort:
      default:
        str = "Fatal error";
#ifdef __riscos__
        t = ThrowbackSeriousError;
#endif
        no_errors++;
        break;
    }

  vsprintf (errbuf, format, ap);
  /* No column support (as it probably does not make sense).  */
  if (lineno == 0)
    lineno = FS_GetCurLineNumber ();
  if (file == NULL)
    file = FS_GetCurFileName ();
  if (lineno == 0)
    fprintf (stderr, "%s: %s: %s\n", file, str, errbuf);
  else
    fprintf (stderr, "%s:%d: %s: %s\n", file, lineno, str, errbuf);

  DoThrowback (t, lineno, errbuf, file);
}

/**
 * Report Info/Warning/Error/Abort message after the input parsing (e.g.
 * during output time).
 * An ErrorAbort or too many ErrorError's won't make this function return.
 */
void
errorLine (const char *file, int lineno, ErrorTag e, const char *format, ...)
{
  va_list ap;
  va_start (ap, format);
  errorCoreLine (file, lineno, e, format, ap);
  va_end (ap);
                 
  if (e == ErrorAbort || no_errors > MAXERR)
    fixup ();
  else if (e == ErrorError)
    skiprest ();
}

/**
 * Reports fatal error for given lineno and file and does not return.
 * To be used after the input parsing (e.g. during output time).
 */
void
errorAbortLine (const char *file, int lineno, const char *format, ...)
{
  va_list ap;
  va_start (ap, format);
  errorCoreLine (file, lineno, ErrorAbort, format, ap);
  va_end (ap);
                 
  fixup ();
}
