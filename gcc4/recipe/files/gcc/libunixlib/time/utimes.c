/* utimes ()
 * Copyright (c) 2000-2013 UnixLib Developers
 */

#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <sys/time.h>
#include <unixlib/local.h>

#include <internal/os.h>
#include <internal/local.h>
#include <internal/swiparams.h>

/* The utimes function is like utime, but also lets you specify the
   fractional part of the file times.

   Change the access time of 'file' to tvp[0] and
   the modification time of 'file' to tvp[1].

   On RISC OS, we just use the modification time.  */
int
utimes (const char *ux_filename, const struct timeval tvp[2])
{
  int ftype, attr;
#ifndef __TARGET_SCL__
  char filename[_POSIX_PATH_MAX];
  if (__object_get_attrs (ux_filename, filename, sizeof (filename),
                          NULL, &ftype, NULL, NULL, NULL, &attr))
    return -1; /* errno already set.  */
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
  /* Do we have load/exec address instead of filetype/datestamp ? */
  if (ftype == __RISCOSIFY_FILETYPE_NOTFOUND)
    return 0;

  unsigned time5b[2];
  if (tvp)
    {
      /* Convert to centiseconds since 1900.  */
      __int64_t timestamp = __cvt_unix_time_csec ((__int64_t)tvp[1].tv_sec * 100
						  + tvp[1].tv_usec / 10000);
      time5b[0] = (unsigned)timestamp;
      time5b[1] = (unsigned)(timestamp >> 32);
    }
  else
    {
      /* Use current time.  */
      time5b[0] = 3;
      if (_kernel_osword (14, time5b) < 0)
	return -1;
    }

  err = SWI_OS_File_WriteCatLoadExecAttr (filename,
					  ((ftype & 0xfffU) << 8) | 0xfff00000U | (time5b[1] & 0xFF),
					  time5b[0], attr);
  if (err)
    return __ul_seterr (err, EOPSYS);

  return 0;
}

