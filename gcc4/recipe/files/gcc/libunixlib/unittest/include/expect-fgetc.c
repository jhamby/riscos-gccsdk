/* Unittest expect call for fgetc().
   Copyright (c) 2013 UnixLib Developers.
  
   Written by John Tytgat / BASS.  */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/**
 * Call fgetc() for testing purposes.
 * \return errno when positive, -1 when fgetc() test fails with inconsistent
 * return value vs errno.
 */
static int
Call_FGetC (int *result, FILE *fh)
{
  errno = -1;
  *result = fgetc (fh);
  if (*result != EOF && !(*result >= 0 && *result < 256))
    {
      fprintf (stderr, "fgetc() returned %d which is not EOF nor between 0 and 256\n",
               *result);
      return -1;
    }

  return errno == -1 ? 0 : errno;
}

static bool
ExpectCall_FGetC (FILE *fh, int expectResult, int err)
{
  int realResult;
  int realErr = Call_FGetC (&realResult, fh);
  if (realErr == -1)
    return true;
  if (realErr != err)
    {
      fprintf (stderr, "fgetc() got error %d (%s) while we would have expected error %d (%s)\n",
               realErr, strerror (realErr), err, strerror (err));
      return true;
    }
  if (realResult != expectResult)
    {
      fprintf (stderr, "fgetc() returned %d while we would have expected %d (EOF is %d)\n",
               realResult, expectResult, EOF);
      return true;
    }
  return false;
}

