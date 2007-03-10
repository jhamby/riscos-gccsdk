/* puts ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <stdio.h>
#include <pthread.h>

__STDIOLIB__

int
puts (const char *s)
{
  PTHREAD_UNSAFE

  if (fputs (s, stdout) || putchar ('\n') == EOF)
    return EOF;

  return 0;
}
