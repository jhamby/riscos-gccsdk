/* fgets ()
 * Copyright (c) 2000-2011 UnixLib Developers
 */

#include <stdio.h>
#include <errno.h>

#include <pthread.h>
#include <internal/unix.h>

char *
fgets (char *s, int n, FILE * stream)
{
  PTHREAD_UNSAFE

  if (!__validfp (stream))
    return NULL;

  /* Repeat, obtaining characters until we hit a newline,
     or exceed our count, n.  */
  char *p = s;
  int c = 0;
  while (--n > 0 && (c = getc (stream)) != EOF)
    if ((*p++ = c) == '\n')
      break;

  /* Return NULL if we had an error, or if we got EOF before
     writing any chars.  */
  if (ferror (stream) || (p == s && feof (stream)))
    return NULL;

  *p = '\0';
  return s;
}
strong_alias (fgets, fgets_unlocked)
