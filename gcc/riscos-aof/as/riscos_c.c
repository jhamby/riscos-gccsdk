/*
 * riscos.c
 * Copyright © 1992 Niklas Röjemo
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#if defined(CROSS_COMPILE) || defined(UNIXLIB)
#include <unistd.h>
#endif
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "error.h"
#include "main.h"
#include "os.h"

#ifndef CROSS_COMPILE
# ifdef UNIXLIB
#  include <unixlib/local.h>
# endif /* UNIXLIB */

static char filename[1024];

char *CanonicalisePath (const char *path1)
{
  int size;
  char *buffer;
  char path[1024];
  __riscosify (path1, 0, 0, path, sizeof (path), NULL);
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
  if ((buffer = malloc (size)) == NULL)
    {
      errorOutOfMem("CanonicalisePath");
      return NULL;
    }
  size = OSCanonicalisePath (filename, buffer, size, 0, 0);
  if (size == 1)
    return buffer;

  error (ErrorAbort, TRUE, "Internal error in CanonicalisePath");
  return NULL;			/* keep the compiler happy */
}

#endif /* ! CROSS_COMPILE */
