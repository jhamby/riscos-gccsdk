/*
 * riscos.c
 * Copyright © 1992 Niklas Röjemo
 */

#include "sdk-config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "main.h"
#include "error.h"
#include "os.h"

#ifndef CROSS_COMPILE
#ifdef UNIXLIB
#include <unixlib/local.h>
#else
#include "uname.h"
#endif /* UNIXLIB */

char *toriscos (char *name, char *oldsuffixes, char newsuffix)
{
  char *suf, *last;

  if ((suf = strrchr (name, '.')) == 0)
    {
      if (newsuffix != 'o')
	error (ErrorError, TRUE, "Missing suffix/prefix in %s", name);
    }
  else if (strchr (oldsuffixes, suf[1]) != 0 && suf[2] == 0)
    {				/* Unix style name.[so] */
      last = strrchr (name, 0);	/* End of file name */
      while (--suf >= name && *suf != '.')
	*--last = *suf;
      suf[1] = newsuffix;
      suf[2] = '.';
    }
  else if (suf > name && strchr (oldsuffixes, suf[-1]) != 0 && (suf == name + 1 || suf[-2] == '.'))
    {
      suf[-1] = newsuffix;	/* RISC/OS style name s.name */
    }				/* else
				   if(newsuffix != 'o')
				   error(ErrorWarning,TRUE,"File %s is not an assembler file (s.name or name.s)",name);
				 */

  return name;
}

static char filename[1024];

char *CanonicalisePath (const char *path1)
{
  int size;
  char *buffer;
#ifdef UNIXLIB
  char path[1024];
  __riscosify (path1, 0, 0, path, sizeof (path), NULL);
#else
  char *path = uname (path1, dde);
#endif
  if (dde && *path == '@')
    {				/* Replace @ with <Prefix$Dir> if dde flags */
      strcpy (filename, "<Prefix$Dir>");
      strcat (filename, path + 1);
    }
  else
    {
      strcpy (filename, path);
    }
  size = 1 - OSCanonicalisePath (filename, 0, 0, 0, 0);
  buffer = malloc (size);
  if (buffer)
    {
      size = OSCanonicalisePath (filename, buffer, size, 0, 0);
      if (size == 1)
	return buffer;
    }

  error (ErrorAbort, TRUE, "Internal error in CanonicalisePath");
  exit (-1);
  return 0;			/* keep the compiler happy */
}

char *
CanonicaliseFile (const FILE * fh)
{
  int size;
  char *buffer;
  size = 1 - OSArgs7 (fh, 0, 0);
  if (size)
    {
      buffer = malloc (size);
      if (buffer)
	{
	  OSArgs7 (fh, buffer, size);
	  buffer[size] = 0;
	  return buffer;
	}
    }
  error (ErrorAbort, TRUE, "Internal error in CanonicaliseFile");
  exit (-1);
  return 0;			/* keep the compiler happy */
}

#endif /* ! CROSS_COMPILE */
