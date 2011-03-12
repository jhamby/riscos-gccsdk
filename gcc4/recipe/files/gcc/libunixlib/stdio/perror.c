/* perror ()
 * Copyright (c) 2000-2011 UnixLib Developers
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>

void
perror (const char *string)
{
  const char *error = strerror (errno);

  if (string && *string)
    fprintf (stderr, "%s: %s\n", string, error);
  else
    fprintf (stderr, "%s\n", error);
}
