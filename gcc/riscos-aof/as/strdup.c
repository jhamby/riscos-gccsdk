/*
 *  strdup.c
 * Copyright © 1997 Darren Salt
 */

#include "sdk-config.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "os.h"

#if !defined (HAVE_STRNDUP)
char *
strndup (const char *str, size_t len)
{
  char *c;
  if ((c = malloc (len + 1)) == NULL)
    return NULL;
  memcpy (c, str, len);
  c[len] = 0;

  return c;
}
#endif

#if !defined (HAVE_STRDUP)
char *
strdup (const char *str)
{
  size_t len = strlen(str) + 1;
  char *c;
  if ((c = malloc (len)) == NULL)
    return NULL;
  memcpy (c, str, len);

  return c;
}
#endif

#if !defined (HAVE_STRNCASECMP)
int
strncasecmp(const char *str1, const char *str2, size_t n)
{
  int i, j;

  if (n == 0)
    return 0;

  do
    {
      i = *str1++, j = *str2++;
      i = toupper (i);
      j = toupper (j);
    }
  while (i && i == j && --n);

  return i - j;
}
#endif
