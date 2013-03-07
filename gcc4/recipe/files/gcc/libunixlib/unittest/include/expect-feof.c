/* Unittest expect call for feof().
   Copyright (c) 2013 UnixLib Developers.
  
   Written by John Tytgat / BASS.  */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static bool
ExpectCall_FEOF (FILE *fh, bool set)
{
  errno = -1;
  int feofSet = feof (fh);
  if (errno != -1)
    {
      fprintf (stderr, "feof() incorrectly changed errno, is now %d (%s)\n",
               errno, strerror (errno));
      return true;
    }
  if (set != (feofSet != 0))
    {
      fprintf (stderr, "feof() returned %d, while we exected it to be %s\n",
               feofSet, set ? "non-zero" : "zero");
      return true;
    }
  return false;
}

