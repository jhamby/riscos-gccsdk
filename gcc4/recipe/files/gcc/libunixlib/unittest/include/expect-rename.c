/* Unittest expect call for rename().
   Copyright (c) 2013 UnixLib Developers.
  
   Written by John Tytgat / BASS.  */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/**
 * Calls rename() for testing purposes.  We check on errno consistency
 * (i.e. when rename() errno should be unchanged, when rename() failses, errno
 * should be non-zero).
 * \return errno
 */
static int
Call_Rename (const char *src, const char *dst)
{
  errno = -1;
  int rtrn = rename (src, dst);
  if (rtrn != 0 && rtrn != -1)
    {
      fprintf (stderr, "rename(%s, %s) returned %d which is 0 nor -1\n",
               src, dst, rtrn);
      return -1;
    }
  if (rtrn == 0 && errno != -1)
    {
      fprintf (stderr, "rename(%s, %s) was successful but errno got changed to %d (%s)\n",
               src, dst, errno, strerror (errno));
      return -1;
    }
  if (rtrn == -1 && (errno == -1 || errno == 0))
    {
      fprintf (stderr, "rename(%s, %s) failed but errno is %d (%s) which is 0 or unchanged\n",
               src, dst, errno, strerror (errno));
      return -1;
    }

  return rtrn == 0 ? 0 : errno;
}

static bool
ExpectCall_Rename (const char *src, const char *dst, int err)
{
  int realErr = Call_Rename (src, dst);
  if (realErr != err)
    {
      fprintf (stderr, "rename(%s, %s) got error %d (%s) while we would have expected error %d (%s)\n",
               src, dst, realErr, strerror (realErr), err, strerror (err));
      return true;
    }
  return false;
}

