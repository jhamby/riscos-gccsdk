/* realpath ()
 * Copyright 2002-2008 UnixLib Developers
 */

#include <errno.h>
#include <stdlib.h>
#include <kernel.h>
#include <swis.h>
#include <limits.h>

#include <unixlib/local.h>
#include <internal/os.h>

/* Canonicalise a filename */
/* resolved_name points to a buffer of at least PATH_MAX bytes */
char *
realpath (const char *file_name, char *resolved_name)
{
  char buffer[PATH_MAX];
  int r[10];
  int filetype;

  if (file_name == NULL || resolved_name == NULL)
    {
      __set_errno (EINVAL);
      return NULL;
    }

  if (__riscosify_std (file_name, 0, resolved_name, PATH_MAX, &filetype) == NULL)
    {
      __set_errno (ENAMETOOLONG);
      return NULL;
    }

  r[0] = 37; /* Canonicalise path */
  r[1] = (int) resolved_name;
  r[2] = (int) buffer;
  r[3] = r[4] = 0;
  r[5] = sizeof (buffer);
  if (__os_swi (OS_FSControl, r) != NULL)
    {
      __set_errno (EIO);
      return NULL;
    }

  if (__unixify_std (buffer, resolved_name, PATH_MAX, filetype) == NULL)
    {
      __set_errno (ENAMETOOLONG);
      return NULL;
    }

  return resolved_name;
}
