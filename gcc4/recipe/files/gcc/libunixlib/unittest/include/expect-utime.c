/* Unittest expect call for utime().
   Copyright (c) 2013 UnixLib Developers.
  
   Written by John Tytgat / BASS.  */

#include <errno.h>
#include <stdbool.h>
#include <sys/types.h>
#include <utime.h>

/**
 * Calls utime() for testing purposes.  We check on errno consistency
 * (i.e. when utime() errno should be unchanged, when utime() failses, errno
 * should be non-zero).
 * \return errno
 */
static int
Call_UTime (const char *fname, const struct utimbuf *times)
{
  errno = -1;
  int rtrn = utime (fname, times);
  if (rtrn != 0 && rtrn != -1)
    {
      fprintf (stderr, "utime(%s, %p) returned %d which is 0 nor -1\n",
               fname, times, rtrn);
      return -1;
    }
  if (rtrn == 0 && errno != -1)
    {
      fprintf (stderr, "utime(%s, %p) was successful but errno got changed to %d (%s)\n",
               fname, times, errno, strerror (errno));
      return -1;
    }
  if (rtrn == -1 && (errno == -1 || errno == 0))
    {
      fprintf (stderr, "utime(%s, %p) failed but errno is %d (%s) which is 0 or unchanged\n",
               fname, times, errno, strerror (errno));
      return -1;
    }

  return rtrn == 0 ? 0 : errno;
}

static bool
ExpectCall_UTime (const char *fname, const struct utimbuf *times, int err)
{
  int realErr = Call_UTime (fname, times);
  if (realErr != err)
    {
      fprintf (stderr, "utime(%s, %p) got error %d (%s) while we would have expected error %d (%s)\n",
               fname, times, realErr, strerror (realErr), err, strerror (err));
      return true;
    }
  return false;
}

