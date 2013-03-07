/* Unittest expect call for fopen().
   Copyright (c) 2013 UnixLib Developers.
  
   Written by John Tytgat / BASS.  */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/**
 * Calls fopen() for testing purposes.
 * \return errno when positive, -1 when fopen() test fails with inconsistent
 * return value vs errno.
 */
static int
Call_FOpen (FILE **result, const char *fname, const char *mode)
{
  errno = -1;
  *result = fopen (fname, mode);
  if (*result != NULL && errno != -1)
    {
      fprintf (stderr, "fopen(%s, %s) was successful but errno got changed to %d (%s)\n",
               fname, mode, errno, strerror (errno));
      return -1;
    }
  if (*result == NULL && (errno == -1 || errno == 0))
    {
      fprintf (stderr, "fopen(%s, %s) failed but errno is %d (%s) which is 0 or unchanged\n",
               fname, mode, errno, strerror (errno));
      return -1;
    }

  return *result != NULL ? 0 : errno;
}

static bool
ExpectCall_FOpen (FILE **result, const char *fname, const char *mode, int err)
{
  int realErr = Call_FOpen (result, fname, mode);
  if (realErr != err)
    {
      fprintf (stderr, "fopen(%s, %s) got error %d (%s) while we would have expected error %d (%s)\n",
               fname, mode, realErr, strerror (realErr), err, strerror (err));
      return true;
    }
  return false;
}

