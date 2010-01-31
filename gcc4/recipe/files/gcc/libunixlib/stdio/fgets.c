/* fgets ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <stdio.h>
#include <errno.h>

#include <pthread.h>
#include <internal/unix.h>

char *
fgets (char *s, int n, FILE * stream)
{
  char *p = s;
  int c = 0;

  PTHREAD_UNSAFE

  if (!__validfp (stream) || s == NULL || n == 0)
    {
      __set_errno (EINVAL);
      return NULL;
    }

  if (ferror (stream))
    return NULL;

  /* Repeat, obtaining characters until we hit a newline,
     or exceed our count, n.  */
  while (--n > 0 && (c = getc (stream)) != EOF)
    if ((*p++ = c) == '\n')
      break;

  if (c == EOF && (p == s || ferror (stream)))
    return NULL;

  *p = '\0';
  return s;
}
