/*
 *  strdup.c
 * Copyright © 1997 Darren Salt
 */

#include <stdlib.h>
#include <string.h>
#include "error.h"

#ifndef UNIXLIB
#if !(defined(UNIX) && defined(USG))
#if !defined(__GNUC_MINOR__) || __GNUC__<2 || __GNUC_MINOR__<7
char *
strdup (char *src)
{
  char *dst = malloc (strlen (src) + 1);
  if (dst)
    strcpy (dst, src);
  return dst;
}
#endif
#endif
#endif

char *
strndup (const char *str, int len)
{
  char *c = malloc (len + 1);
  if (c == 0)
    /*error(ErrorSerious, TRUE, "Internal strndup: out of memory") */ ;
  else
    {
      memcpy (c, str, len);
      c[len] = 0;
    }
  return c;
}
