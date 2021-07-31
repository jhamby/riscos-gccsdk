/* Unittest expect call for lstat().
   Copyright (c) 2013 UnixLib Developers.
  
   Written by John Tytgat / BASS.  */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

/**
 * Calls lstat() for testing purposes.  We check on errno consistency
 * (i.e. when lstat() errno should be unchanged, when lstat() fails, errno
 * should be non-zero).
 * \return errno
 */
static int
Call_LStat (const char *path, struct stat *buf)
{
  errno = -1;
  int rtrn = lstat (path, buf);
  if (rtrn != 0 && rtrn != -1)
    {
      fprintf (stderr, "lstat(%s, ...) returned %d which is 0 nor -1\n",
               path, rtrn);
      return -1;
    }
  if (rtrn == 0 && errno != -1)
    {
      fprintf (stderr, "lstat(%s, ...) was successful but errno got changed to %d (%s)\n",
               path, errno, strerror (errno));
      return -1;
    }
  if (rtrn == -1 && (errno == -1 || errno == 0))
    {
      fprintf (stderr, "lstat(%s, ...) failed but errno is %d (%s) which is 0 or unchanged\n",
               path, errno, strerror (errno));
      return -1;
    }

  return rtrn == 0 ? 0 : errno;
}

static bool
ExpectCall_LStat (const char *path, struct stat *buf, int err)
{
  int realErr = Call_LStat (path, buf);
  if (realErr != err)
    {
      fprintf (stderr, "lstat(%s, ...) got error %d (%s) while we would have expected error %d (%s)\n",
               path, realErr, strerror (realErr), err, strerror (err));
      return true;
    }
  return false;
}

