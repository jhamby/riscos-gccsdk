/*
 *  strdup.c
 * Copyright © 1997 Darren Salt
 */

#include "sdk-config.h"
#include <stdlib.h>
#include <string.h>
#include "os.h"
#include "error.h"

#if (defined (CROSS_COMPILE) && ! defined (HAVE_STRNDUP))
char *
strndup (const char *str, int len)
{
  char *c = malloc (len + 1);
  if (c == 0)
    error(ErrorSerious, TRUE, "Internal strndup: out of memory");
  else
    {
      memcpy (c, str, len);
      c[len] = 0;
    }
  return c;
}
#endif

