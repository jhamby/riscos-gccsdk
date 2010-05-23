/* mkstemp ()
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

int
mkstemp (char *file_template)
{
  return open (mktemp (file_template), O_RDWR | O_CREAT | O_TRUNC, 0666);
}
