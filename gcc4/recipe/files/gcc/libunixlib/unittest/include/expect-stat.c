/* Unittest expect call for stat().
   Copyright (c) 2013 UnixLib Developers.
  
   Written by John Tytgat / BASS.  */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

/**
 * Calls stat() for testing purposes.  We check on errno consistency
 * (i.e. when stat() errno should be unchanged, when stat() fails, errno
 * should be non-zero).
 * \return errno
 */
static int
Call_Stat (const char *path, struct stat *buf)
{
  errno = -1;
  int rtrn = stat (path, buf);
  if (rtrn != 0 && rtrn != -1)
    {
      fprintf (stderr, "stat(%s, ...) returned %d which is 0 nor -1\n",
               path, rtrn);
      return -1;
    }
  if (rtrn == 0 && errno != -1)
    {
      fprintf (stderr, "stat(%s, ...) was successful but errno got changed to %d (%s)\n",
               path, errno, strerror (errno));
      return -1;
    }
  if (rtrn == -1 && (errno == -1 || errno == 0))
    {
      fprintf (stderr, "stat(%s, ...) failed but errno is %d (%s) which is 0 or unchanged\n",
               path, errno, strerror (errno));
      return -1;
    }

  return rtrn == 0 ? 0 : errno;
}

static bool
ExpectCall_Stat (const char *path, struct stat *buf, int err)
{
  int realErr = Call_Stat (path, buf);
  if (realErr != err)
    {
      fprintf (stderr, "stat(%s, ...) got error %d (%s) while we would have expected error %d (%s)\n",
               path, realErr, strerror (realErr), err, strerror (err));
      return true;
    }
  return false;
}

