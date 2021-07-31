/* Unittest expect call for readlink().
   Copyright (c) 2013 UnixLib Developers.

   Written by John Tytgat / BASS.  */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>

#include <unistd.h>

/**
 * Calls readlink() for testing purposes.
 * \return errno when positive, -1 when readlink() test fails with inconsistent
 * return value vs errno.
 */
static int
Call_ReadLink (const char *path, char *buf, size_t bufsize)
{
  errno = -1;
  int rtrn = readlink (path, buf, bufsize);
  if (rtrn < -1)
    {
      fprintf (stderr, "readlink(%s, ..., %zd) returned %d which is >= 0 nor -1\n",
               path, bufsize, rtrn);
      return -1;
    }
  if (rtrn >= 0 && errno != -1)
    {
      fprintf (stderr, "readlink(%s, ..., %zd) was successful but errno got changed to %d (%s)\n",
               path, bufsize, errno, strerror (errno));
      return -1;
    }
  if (rtrn == -1 && (errno == -1 || errno == 0))
    {
      fprintf (stderr, "readlink(%s, ..., %zd) failed (returned -1) but errno is %d (%s) which is 0 or unchanged\n",
               path, bufsize, errno, strerror (errno));
      return -1;
    }

  return rtrn;
}

/**
 * \param exp_rtrn Expected return value for readlink().
 * \param exp_errno Expected errno value (or 0 in case of no expected error).
 */
static bool
ExpectCall_ReadLink (const char *path, char *buf, size_t bufsize, int exp_rtrn, int exp_errno)
{
  int real_rtrn = Call_ReadLink (path, buf, bufsize);
  if (real_rtrn != exp_rtrn)
    {
      fprintf (stderr, "readlink(%s, ..., %zd) returned %d (%s) while we would have expected %d\n",
               path, bufsize, real_rtrn, real_rtrn == -1 ? strerror (errno) : "", exp_rtrn);
      return true;
    }
  if (exp_errno && errno != exp_errno)
    {
      fprintf (stderr, "readlink(%s, ..., %zd) returned errno %d (%s) but we expected errno %d instead\n",
               path, bufsize, errno, strerror (errno), exp_errno);
      return true;
    }
  if (exp_errno == 0 && errno != -1)
    {
      fprintf (stderr, "readlink(%s, ..., %zd) returned errno %d (%s) but we did not expect an error\n",
               path, bufsize, errno, strerror (errno));
      return true;
    }
  return false;
}

static bool
Check_ReadLink (const char *path, const char *expect_link)
{
  size_t bufsize = strlen (expect_link) + 1;
  char *buf = malloc (bufsize);
  if (buf == NULL)
    {
      fprintf (stderr, "Check_ReadLink(%s, %s): malloc() failed while claiming %zd bytes\n",
               path, expect_link, bufsize);
      return true;
    }
  buf[bufsize - 1] = '\xde'; /* Place cookie.  */
  int err = ExpectCall_ReadLink (path, buf, bufsize, bufsize - 1, 0);
  if (err != 0)
    {
      free (buf);
      fprintf (stderr, "Check_ReadLink(%s, %s): Call_ReadLink() failed\n",
               path, expect_link);
      return true;
    }
  if (buf[bufsize - 1] != '\xde')
    {
      free (buf);
      fprintf (stderr, "Check_ReadLink(%s, %s): readlink() did a buffer overflow\n",
               path, expect_link);
      return true;
    }

  bool rslt = memcmp (buf, expect_link, bufsize - 1) != 0;
  if (rslt)
    fprintf (stderr, "Check_ReadLink(%s, %s): got '%.*s', expected '%s'\n",
             path, expect_link, (int)(bufsize - 1), buf, expect_link);
  free (buf);
  return rslt;
}
