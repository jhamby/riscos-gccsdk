/* Unittest expect call for fputc().
   Copyright (c) 2013 UnixLib Developers.
  
   Written by John Tytgat / BASS.  */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/**
 * Call fputc() for testing purposes.
 * \return errno when positive, -1 when fputc() test fails with inconsistent
 * return value vs errno.
 */
static int
Call_FPutC (int c, FILE *fh)
{
  errno = -1;
  int rtrn = fputc (c, fh);
  if (rtrn != EOF && rtrn != c)
    {
      fprintf (stderr, "fputc() returned %d which is not EOF nor value %d written (EOF is %d)\n",
               rtrn, c, EOF);
      return -1;
    }

  return errno == -1 ? 0 : errno;
}

static bool
ExpectCall_FPutC (int c, FILE *fh, int err)
{
  int realErr = Call_FPutC (c, fh);
  if (realErr == -1)
    return true;
  if (realErr != err)
    {
      fprintf (stderr, "fputc() got error %d (%s) while we would have expected error %d (%s)\n",
               realErr, strerror (realErr), err, strerror (err));
      return true;
    }
  return false;
}

