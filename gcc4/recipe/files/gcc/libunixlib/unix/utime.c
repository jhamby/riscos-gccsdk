/* Change access and/or modification times of an inode.
   Copyright (c) 2005-2012 UnixLib Developers.  */

#include <errno.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>
#include <utime.h>
#include <unixlib/local.h>

#include <internal/os.h>
#include <internal/local.h>
#include <internal/swiparams.h>

/* The access time is ignored.  RISC OS does not support that.  */
int
utime (const char *ux_filename, const struct utimbuf *times)
{
  int ftype, attr;
#ifndef __TARGET_SCL__
  char filename[_POSIX_PATH_MAX];
  if (__object_get_attrs (ux_filename, filename, sizeof (filename),
                          NULL, &ftype, NULL, NULL, NULL, &attr))
    return -1;
  const _kernel_oserror *err;
#else
  const char *filename = ux_filename;
  const _kernel_oserror *err;
  if ((err = SWI_OS_File_ReadCatInfo (filename, NULL, &ftype, NULL, NULL,
				      &attr)) != NULL)
    return __ul_seterr (err, EOPSYS);
  if ((ftype & 0xfff00000U) == 0xfff00000U)
    ftype = (ftype >> 8) & 0xfff;
  else
    ftype = __RISCOSIFY_FILETYPE_NOTFOUND;
#endif
  if (ftype == __RISCOSIFY_FILETYPE_NOTFOUND)
    return 0;

  /* If times is null then get the current time.  */
  time_t unix_time = times ? times->modtime : time (NULL);

  /* Convert time to RISC OS and set the file's time.  */
  unsigned int low, high;
  __cvt_unix_time (unix_time, &high, &low);
  err = SWI_OS_File_WriteCatLoadExecAttr (filename,
					  ((ftype & 0xfffU) << 8) | 0xfff00000U | high,
					  low, attr);
  if (err)
    return __ul_seterr (err, EOPSYS);

  return 0;
}
