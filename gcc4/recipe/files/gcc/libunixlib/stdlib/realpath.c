/* realpath ()
 * Copyright 2002-2011 UnixLib Developers
 */

#include <errno.h>
#include <stdlib.h>
#include <kernel.h>
#include <limits.h>

#include <unixlib/local.h>
#include <internal/os.h>

/* Canonicalise a filename */
/* resolved_name points to a buffer of at least PATH_MAX bytes */
char *
realpath (const char *file_name, char *resolved_name)
{
  if (file_name == NULL || resolved_name == NULL)
    {
      __set_errno (EINVAL);
      return NULL;
    }

  int filetype;
  if (__riscosify_std (file_name, 0, resolved_name, PATH_MAX, &filetype) == NULL)
    {
      __set_errno (ENAMETOOLONG);
      return NULL;
    }

  char buffer[PATH_MAX];
  const _kernel_oserror *err;
  if ((err = SWI_OS_FSControl_Canonicalise (resolved_name, NULL,
					    buffer, sizeof (buffer),
					    NULL)) != NULL)
    {
      __ul_seterr (err, EIO);
      return NULL;
    }

  if (__unixify_std (buffer, resolved_name, PATH_MAX, filetype) == NULL)
    {
      __set_errno (ENAMETOOLONG);
      return NULL;
    }

  return resolved_name;
}
