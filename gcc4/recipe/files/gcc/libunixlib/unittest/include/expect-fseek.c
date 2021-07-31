/* Unittest expect call for fseek().
   Copyright (c) 2013 UnixLib Developers.
  
   Written by John Tytgat / BASS.  */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/**
 * Call fseek() for testing purposes.
 * \return errno when positive, -1 when fputc() test fails with inconsistent
 * return value vs errno.
 */
static int
Call_FSeek (FILE *fh, long offset, int whence)
{
  errno = -1;
  int rtrn = fseek (fh, offset, whence);
  if (rtrn != 0 && rtrn != -1)
    {
      fprintf (stderr, "fseek(, %ld, %d) returned %d which is 0 nor -1\n",
               offset, whence, rtrn);
      return -1;
    }
  if (rtrn == 0 && errno != -1)
    {
      fprintf (stderr, "fseek(, %ld, %d) was successful but errno got changed to %d (%s)\n",
               offset, whence, errno, strerror (errno));
      return -1;
    }
  if (rtrn == -1 && (errno == -1 || errno == 0))
    {
      fprintf (stderr, "fseek(, %ld, %d) failed but errno is %d (%s) which is 0 or unchanged\n",
               offset, whence, errno, strerror (errno));
      return -1;
    }

  return rtrn == 0 ? 0 : errno;
}

static bool
ExpectCall_FSeek (FILE *fh, long offset, int whence, int err)
{
  int realErr = Call_FSeek (fh, offset, whence);
  if (realErr != err)
    {
      fprintf (stderr, "fseek(, %ld, %d) got error %d (%s) while we would have expected error %d (%s)\n",
               offset, whence, realErr, strerror (realErr), err, strerror (err));
      return true;
    }
  return false;
}

