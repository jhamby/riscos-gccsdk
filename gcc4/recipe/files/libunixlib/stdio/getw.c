/* getw ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <stdio.h>
#include <pthread.h>

#define INTSIZE 4

__STDIOLIB__

int
getw (FILE * f)
{
  int i;

  PTHREAD_UNSAFE

  i = getc (f);
  i |= (getc (f) << 8);
#if INTSIZE > 2
  i |= (getc (f) << 16);
  i |= (getc (f) << 24);
#if INTSIZE > 4
  i |= (getc (f) << 32);
  i |= (getc (f) << 40);
  i |= (getc (f) << 48);
  i |= (getc (f) << 56);
#endif
#endif
  return (ferror (f) ? -1 : i);
}
