/* Create a directory.
   Copyright (c) 2005-2010 UnixLib Developers.  */

#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <unixlib/local.h>
#include <internal/os.h>
#include <internal/local.h>
#include <internal/swiparams.h>
#include <internal/unix.h>

int
mkdir (const char *ux_path, __mode_t mode)
{
  PTHREAD_UNSAFE_CANCELLATION

  /* We don't want to do suffix swapping for directory objects.  */
  const int riscosify_control = __get_riscosify_control ();
  __set_riscosify_control (riscosify_control | __RISCOSIFY_NO_SUFFIX);

  char path[_POSIX_PATH_MAX];
  int objtype;
  int rtrn_get_attrs = __object_get_attrs (ux_path, path, sizeof (path),
					   &objtype,
					   NULL, NULL, NULL, NULL, NULL)
			  && errno != ENOENT;

  /* Restore suffix swapping status.  */
  __set_riscosify_control (riscosify_control);

  if (rtrn_get_attrs)
    return -1;

  /* Fail if the directory already exists.  */
  if (objtype != 0)
    return __set_errno (EEXIST);

  /* Create the directory, with default number of entries per directory.  */
  int regs[6];
  regs[4] = 0;
  const _kernel_oserror *err = __os_file (OSFILE_CREATEDIRECTORY, path, regs);
  if (err)
    {
      /* Match with "Not found" RISC OS error */
      if (err->errnum == 0x108d6 || strcasecmp(err->errmess, "Not found") == 0
          || strstr(err->errmess, "not found"))
        return __set_errno (ENOENT);

      return __ul_seterr (err, EOPSYS);
    }

  /* Set the file access permission bits.  */
  regs[5] = __set_protection (mode);
  err = __os_file (OSFILE_WRITECATINFO_ATTR, path, regs);
  if (err)
    return  __ul_seterr (err, EOPSYS);

  return 0;
}
