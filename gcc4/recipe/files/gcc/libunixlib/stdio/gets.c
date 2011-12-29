/* gets ()
 * Copyright (c) 2000-2011 UnixLib Developers
 */

#include <stdio.h>
#include <errno.h>

#include <pthread.h>
#include <internal/unix.h>

char *
gets (char *s)
{
  PTHREAD_UNSAFE

  FILE *stream = stdin;
  if (!__validfp (stream))
    {
      __set_errno (EINVAL);
      return NULL;
    }

  char *p = s;
  int c;
  while ((c = getc (stream)) != EOF && c != '\n')
    *p++ = c;

  /* Return NULL if we had an error, or if we got EOF before
     writing any chars.  */
  if (ferror (stream) || (p == s && feof (stream)))
    return NULL;

  *p = '\0';
  return s;
}
