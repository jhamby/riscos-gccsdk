/*
 * error.c
 * Copyright © 1992 Niklas Röjemo
 */
#include "sdk-config.h"
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

#include "filestack.h"
#include "error.h"
#include "input.h"
#include "output.h"
#include "os.h"
#include "macros.h"

#define MAXERR 100

extern int pedantic, throwback, verbose;

static int no_errors = 0;
static int no_warnings = 0;
static char *source = NULL;

#ifdef __riscos
static int ThrowbackStarted;
#endif

static char errbuf[2048];
char er[1024];

void 
errorInit (int throwback, char *filename)
{
  source = filename;
}


#ifdef __riscos
void 
errorFinish (void)
{
  if (ThrowbackStarted > 0)
    ThrowbackEnd ();
}
#endif


#ifdef __riscos
char *
LF (char *buf)
{
  char *p = buf;
  p--;
  while (*++p)
    if (*p == '\n' || *p == '\r')
      *p = ' ';
  while (*--p == ' ' && p > buf)
    *p = 0;
  *++p = '\n';
  *++p = 0;
  return buf;
}

static void 
TB (int level, long int lineno, char *error, const char *file)
{
  os_error *err;

  if (!throwback || !source)
    return;

  if (!ThrowbackStarted)
    ThrowbackStarted = ThrowbackStart ()? -1 : 1;
  if (ThrowbackStarted == 1)
    {
      err = ThrowbackSendStart (file);
      if (err)
	{
	  if (verbose > 1)
	    fprintf (stderr, "ThrowbackSendStart %s", err->errmess);
	  ThrowbackStarted = -1;
	}
      ThrowbackStarted = 1;
    }
  if (ThrowbackStarted > 0)
    ThrowbackSendError (level, lineno, error);
}
#endif /* __riscos */

#ifndef __riscos
#define TB(x,y,z,f)
#endif


int 
noerrors (void)
{
  /* if (no_errors) outputRemove(); */
  return no_errors;
}


int 
nowarnings (void)
{
  return no_warnings;
}


extern jmp_buf asmContinue;
extern jmp_buf asmAbort;


static void 
fixup (ErrorTag t)
{
  skiprest ();
  if (t == ErrorAbort || no_errors > MAXERR)
    longjmp (asmAbort, 1);
  else
    longjmp (asmContinue, 1);
}

static void 
doline (int t, long int line, int sameline)
{
  if (line > 0)
    {
      TB (t, line, errbuf, inputName);
      if (get_file (0, 0))
	{
	  const char *file = inputName, *nfile;
	  long int diffline = 0, nline, i;
	  do
	    {
	      fprintf (stderr, "%s at line %li", diffline ? "\n " : (" " + sameline), line);
	      if ((i = get_file (&nfile, &nline)) != 0)
		fprintf (stderr, " in file '%s'", file);
	      if (diffline)
		{
		  sprintf (errbuf, "  included from here");
		  /* I'd like to be able to put
		   *   t=ThrowbackInfo;
		   * here without Zap spewing forth "Information for file"... :-|
		   */
		  TB (t, line, errbuf, file);
		}
	      diffline = 1;
	      sameline = 0;
	      file = nfile;
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
  char *str;
  va_list ap;
  long int line;
  int sameline = 1;
  int t = 0;

#ifdef __riscos
  switch (e)
    {
    case ErrorInfo:
      str = "Info";
      t = ThrowbackInfo;
      break;
    case ErrorWarning:
      str = "Warning";
      no_warnings++;
      t = ThrowbackWarning;
      break;
    case ErrorError:
      str = "Error";
      no_errors++;
      t = ThrowbackError;
      break;
    default:
      str = "Serious error";
      no_errors++;
      t = ThrowbackSeriousError;
      break;
    }
#else
  switch (e)
    {
    case ErrorInfo:
      str = "Info";
      break;
    case ErrorWarning:
      str = "Warning";
      no_warnings++;
      break;
    case ErrorError:
      str = "Error";
      no_errors++;
      break;
    default:
      str = "Serious error";
      no_errors++;
      break;
    }
#endif
  va_start (ap, format);
  vsprintf (errbuf, format, ap);
  va_end (ap);
  fprintf (stderr, "%s: %s", str, errbuf);
  line = inputLineNo;
  if (macroSP)
    {
      int i = macroSP;
      const char *name = macroCurrent->name;
      const char *file = macroCurrent->file;
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
  char *str;
  int t;
  va_list ap;

#ifdef __riscos
  switch (e)
    {
    case ErrorInfo:
      str = "Info";
      t = ThrowbackInfo;
      break;
    case ErrorWarning:
      str = "Warning";
      t = ThrowbackWarning;
      no_warnings++;
      break;
    case ErrorError:
      str = "Error";
      t = ThrowbackError;
      no_errors++;
      break;
    default:
      str = "Fatal error";
      t = ThrowbackSeriousError;
      no_errors++;
      break;
    }
#else
  switch (e)
    {
    case ErrorInfo:
      str = "Info";
      break;
    case ErrorWarning:
      str = "Warning";
      no_warnings++;
      break;
    case ErrorError:
      str = "Error";
      no_errors++;
      break;
    default:
      str = "Fatal error";
      no_errors++;
    }
#endif

  va_start (ap, format);
  vsprintf (errbuf, format, ap);
  va_end (ap);
  fprintf (stderr, "%s: %s", str, errbuf);
  if (lineno > 0)
    fprintf (stderr, " at line %li", lineno);
  if (!file)
    file = inputName;
  if (file != inputName)
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


const char *const InsertCommaAfter = "Inserting missing comma after ";
