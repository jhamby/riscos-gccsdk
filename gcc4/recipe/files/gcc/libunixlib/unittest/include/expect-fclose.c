/* Unittest expect call for fclose().
   Copyright (c) 2013 UnixLib Developers.
  
   Written by John Tytgat / BASS.  */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/**
 * Calls fclose() for testing purposes.
 * \return errno when positive, -1 when fclose() test fails with inconsistent
 * return value vs errno.
 */
static int
Call_FClose (FILE *fh)
{
  errno = -1;
  int rtrn = fclose (fh);
  if (rtrn != 0 && rtrn != EOF)
    {
      fprintf (stderr, "fclose() returned %d which is 0 nor -1\n",
               rtrn);
      return -1;
    }
  if (rtrn == 0 && errno != -1)
    {
      fprintf (stderr, "fclose() was successful but errno got changed to %d (%s)\n",
               errno, strerror (errno));
      return -1;
    }
  if (rtrn == EOF && (errno == -1 || errno == 0))
    {
      fprintf (stderr, "fclose() failed (returned EOF) but errno is %d (%s) which is 0 or unchanged\n",
               errno, strerror (errno));
      return -1;
    }

  return rtrn == 0 ? 0 : errno;
}

static bool
ExpectCall_FClose (FILE *fh, int err)
{
  int realErr = Call_FClose (fh);
  if (realErr != err)
    {
      fprintf (stderr, "fclose() got error %d (%s) while we would have expected error %d (%s)\n",
               realErr, strerror (realErr), err, strerror (err));
      return true;
    }
  return false;
}

