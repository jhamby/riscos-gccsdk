/* Unittest expect call for utimes().
   Copyright (c) 2013 UnixLib Developers.
  
   Written by John Tytgat / BASS.  */

#include <errno.h>
#include <stdbool.h>
#include <sys/time.h>

/**
 * Calls utimes() for testing purposes.  We check on errno consistency
 * (i.e. when utime() errno should be unchanged, when utime() failses, errno
 * should be non-zero).
 * \return errno
 */
static int
Call_UTimes (const char *fname, const struct timeval times[2])
{
  errno = -1;
  int rtrn = utimes (fname, times);
  if (rtrn != 0 && rtrn != -1)
    {
      fprintf (stderr, "utimes(%s, %p) returned %d which is 0 nor -1\n",
               fname, times, rtrn);
      return -1;
    }
  if (rtrn == 0 && errno != -1)
    {
      fprintf (stderr, "utimes(%s, %p) was successful but errno got changed to %d (%s)\n",
               fname, times, errno, strerror (errno));
      return -1;
    }
  if (rtrn == -1 && (errno == -1 || errno == 0))
    {
      fprintf (stderr, "utimes(%s, %p) failed but errno is %d (%s) which is 0 or unchanged\n",
               fname, times, errno, strerror (errno));
      return -1;
    }

  return rtrn == 0 ? 0 : errno;
}

static bool
ExpectCall_UTimes (const char *fname, const struct timeval times[2], int err)
{
  int realErr = Call_UTimes (fname, times);
  if (realErr != err)
    {
      fprintf (stderr, "utimes(%s, %p) got error %d (%s) while we would have expected error %d (%s)\n",
               fname, times, realErr, strerror (realErr), err, strerror (err));
      return true;
    }
  return false;
}

