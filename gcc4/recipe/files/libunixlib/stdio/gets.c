/* gets ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <stdio.h>
#include <errno.h>

#include <pthread.h>
#include <internal/unix.h>

char *
gets (char *_s)
{
  FILE *stream = stdin;
  char *s = _s;
  int c = 0;

  PTHREAD_UNSAFE

  if (!__validfp (stream) || s == NULL)
    {
      __set_errno (EINVAL);
      return NULL;
    }

  if (feof (stream) || ferror (stream))
    return NULL;

  while ((c = getc (stream)) != EOF)
    if (c == '\n')
      break;
    else
      *s++ = c;

  *s = '\0';

  /* Return NULL if we had an error, or if we got EOF before
     writing any chars.  */
  if (ferror (stream) || (feof (stream) && s == _s))
    return NULL;

  return _s;
}
