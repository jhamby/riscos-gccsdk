/* Change access and/or modification times of an inode.
   Copyright (c) 2005-2010 UnixLib Developers.  */

#include <errno.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>
#include <utime.h>

#include <internal/os.h>
#include <internal/local.h>
#include <internal/swiparams.h>

int
utime (const char *ux_filename, const struct utimbuf *times)
{
  time_t unix_time;
  unsigned int low, high;
  _kernel_oserror *err;
  int regs[10], ftype, attr;
  char filename[_POSIX_PATH_MAX];

  if (__object_get_attrs (ux_filename, filename, sizeof (filename),
                          NULL, &ftype, NULL, NULL, NULL, &attr))
    return -1;

  /* If times is null then get the current time.  */
  unix_time = times ? times->modtime : time (NULL);

  /* Convert time to RISC OS and set the file's time.  */
  __cvt_unix_time (unix_time, &high, &low);
  regs[2] = ((ftype & 0xfffU) << 8) | 0xfff00000U | high;
  regs[3] = (int) low;
  regs[5] = attr;
  err = __os_file (OSFILE_WRITECATINFO_ALL, filename, regs);
  if (err)
    return __ul_seterr (err, EOPSYS);

  return 0;
}
