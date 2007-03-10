/* Change permissions of a file.
   Copyright (c) 2005 UnixLib Developers.  */

#include <errno.h>
#include <limits.h>
#include <unistd.h>

#include <unixlib/os.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <unixlib/local.h>
#include <internal/swiparams.h>

int
chmod (const char *ux_file, mode_t mode)
{
  char file[_POSIX_PATH_MAX];
  _kernel_oserror *err;
  int regs[10];

  if (__object_get_attrs (ux_file, file, sizeof (file),
                          NULL, NULL, NULL, NULL, NULL, NULL))
    return -1;

  /* Set the file access permission bits.  */
  regs[5] = __set_protection (mode);
  err = __os_file (OSFILE_WRITECATINFO_ATTR, file, regs);
  if (err)
    {
      __ul_seterr (err, 0);
      return __set_errno (EPERM);
    }

  return 0;
}
