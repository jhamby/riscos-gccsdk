/* putw ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <stdio.h>

#include <pthread.h>
#include <internal/unix.h>

#define INTSIZE 4

int
putw (int i, FILE * f)
{
  PTHREAD_UNSAFE

  putc (i & 0xff, f);
  putc ((i >> 8) & 0xff, f);
#if INTSIZE > 2
  putc ((i >> 16) & 0xff, f);
  putc ((i >> 24) & 0xff, f);
#if INTSIZE > 4
  putc ((i >> 32) & 0xff, f);
  putc ((i >> 40) & 0xff, f);
  putc ((i >> 48) & 0xff, f);
  putc ((i >> 56) & 0xff, f);
#endif
#endif

  return ferror (f) ? -1 : i;
}
