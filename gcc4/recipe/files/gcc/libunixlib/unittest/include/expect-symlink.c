/* Unittest expect call for symlink().
   Copyright (c) 2013 UnixLib Developers.

   Written by John Tytgat / BASS.  */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>

#include <unistd.h>

/**
 * Calls symlink() for testing purposes.
 * \return errno when positive, -1 when symlink() test fails with inconsistent
 * return value vs errno.
 */
static int
Call_SymLink (const char *actpath, const char *sympath)
{
  errno = -1;
  int rtrn = symlink (actpath, sympath);
  if (rtrn != 0 && rtrn != -1)
    {
      fprintf (stderr, "symlink(%s, %s) returned %d which is 0 nor -1\n",
               actpath, sympath, rtrn);
      return -1;
    }
  if (rtrn == 0 && errno != -1)
    {
      fprintf (stderr, "symlink(%s, %s) was successful but errno got changed to %d (%s)\n",
               actpath, sympath, errno, strerror (errno));
      return -1;
    }
  if (rtrn == -1 && (errno == -1 || errno == 0))
    {
      fprintf (stderr, "symlink(%s, %s) failed but errno is %d (%s) which is 0 or unchanged\n",
               actpath, sympath, errno, strerror (errno));
      return -1;
    }

  return rtrn == 0 ? 0 : errno;
}

static bool
ExpectCall_SymLink (const char *actpath, const char *sympath, int err)
{
  int realErr = Call_SymLink (actpath, sympath);
  if (realErr == -1)
    return true;
  if (realErr != err)
    {
      fprintf (stderr, "symlink(%s, %s) got error %d (%s) while we would have expected error %d (%s)\n",
               actpath, sympath, realErr, strerror (realErr), err, strerror (err));
      return true;
    }
  return false;
}

/**
 * Can be used in non-symlink() related tests.
 */
static bool
SymLink (const char *actpath, const char *sympath)
{
  return ExpectCall_SymLink (actpath, sympath, 0);
}
