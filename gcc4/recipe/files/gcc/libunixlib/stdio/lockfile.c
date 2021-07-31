/* UnixLib flockfile, ftrylockfile and funlockfile implementation.
   Copyright 2011 UnixLib Developers.  */

#include <stdio.h>

#include <pthread.h>
#include <internal/unix.h>

void
flockfile (FILE *stream)
{
  if (__validfp (stream))
    {
      /* FIXME: implementation missing.  */
    }
}

int
ftrylockfile (FILE *stream)
{
  if (!__validfp (stream))
    return -1;

  /* FIXME: implementation missing.  */

  return 0;
}

void
funlockfile (FILE *stream)
{
  if (__validfp (stream))
    {
      /* FIXME: implementation missing.  */
    }
}

