/* perror ()
 * Copyright (c) 2000-2007 UnixLib Developers
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>

void
perror (const char *string)
{
  const char *error = strerror (errno);

  if (string)
    fprintf (stderr, "%s: %s\n", string, error);
  else
    fprintf (stderr, "%s\n", error);
}
