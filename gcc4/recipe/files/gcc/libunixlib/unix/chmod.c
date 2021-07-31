/* Change permissions of a file.
   Copyright (c) 2005-2012 UnixLib Developers.  */

#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <internal/os.h>
#include <internal/local.h>
#include <internal/swiparams.h>

int
chmod (const char *ux_file, mode_t mode)
{
  char file[_POSIX_PATH_MAX];
  if (__object_get_attrs (ux_file, file, sizeof (file),
                          NULL, NULL, NULL, NULL, NULL, NULL))
    return -1;

  /* Set the file access permission bits.  */
  int attr = __set_protection (mode);
  const _kernel_oserror *err;
  if ((err = SWI_OS_File_WriteCatInfoAttr (file, attr)) != NULL)
    return __ul_seterr (err, EPERM);

  return 0;
}
