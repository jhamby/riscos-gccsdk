/*
 * riscos.c
 * Copyright © 1992 Niklas Röjemo
 */

#ifdef __riscos
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "error.h"
#include "os.h"
#ifdef UNIXLIB
#include <unixlib/local.h>
#else
#include "uname.h"
#endif /* UNIXLIB */

char *
toriscos (char *name, char *oldsuffixes, char newsuffix)
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
extern int dde;

char *
CanonicalisePath (const char *path1)
{
  int size;
  char *buffer;
  char path[256];
#ifdef UNIXLIB
#if 0
  path = __uname (path, 0);
#else
  __riscosify (path, 0, __RISCOSIFY_DONT_TRUNCATE, path, sizeof (path), NULL);
#endif
#else
  path = uname (path, dde);
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
#endif /* __riscos */
 
