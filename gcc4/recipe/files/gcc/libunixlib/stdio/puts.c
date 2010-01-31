/* puts ()
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#include <stdio.h>

#include <pthread.h>
#include <internal/unix.h>

int
puts (const char *s)
{
  PTHREAD_UNSAFE

  if (fputs (s, stdout) || fputc ('\n', stdout) == EOF)
    return EOF;

  return 0;
}
