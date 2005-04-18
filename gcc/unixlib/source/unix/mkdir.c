/* Make a directory on disk.
   Copyright (c) 2005 UnixLib Developers.  */

#include <errno.h>
#include <limits.h>
#include <unistd.h>

#include <unixlib/os.h>
#include <sys/stat.h>

#include <unixlib/local.h>
#include <internal/swiparams.h>

int
mkdir (const char *ux_path, __mode_t mode)
{
  int regs[6], objtype;
  _kernel_oserror *err;
  char path[_POSIX_PATH_MAX];
  int riscosify_control, rtrn_get_attrs;

  /* We don't want to do suffix swapping for directory objects.  */
  riscosify_control = __get_riscosify_control ();
  __set_riscosify_control (riscosify_control | __RISCOSIFY_NO_SUFFIX);

  rtrn_get_attrs = __object_get_attrs (ux_path, path, sizeof (path),
                                       &objtype, NULL, NULL, NULL, NULL, NULL)
                   && errno != ENOENT;

  /* Restore suffix swapping status.  */
  __set_riscosify_control (riscosify_control);

  if (rtrn_get_attrs)
    return -1;

  /* Fail if the directory already exists.  */
  if (objtype != 0)
    return __set_errno (EEXIST);

  /* Create the directory, with default number of entries per directory.  */
  regs[4] = 0;
  err = __os_file (OSFILE_CREATEDIRECTORY, path, regs);
  if (err)
    {
      __ul_seterr (err, 1);
      return -1;
    }

  /* Set the file access permission bits.  */
  regs[5] = __set_protection (mode);
  err = __os_file (OSFILE_WRITECATINFO_ATTR, path, regs);
  if (err)
    {
      __ul_seterr (err, 1);
      return -1;
    }

  return 0;
}
