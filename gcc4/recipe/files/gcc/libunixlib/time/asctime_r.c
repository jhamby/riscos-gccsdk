/* asctime_r ()
 * Written by Nick Burrett on 12 July 1997.
 * Copyright (c) 1997-2011 UnixLib Developers
 */

#include <errno.h>
#include <kernel.h>
#include <time.h>

#include <internal/os.h>

char *
asctime_r (const struct tm *brokentime, char *buffer)
{
  const _kernel_oserror *err;
  char riscos_time[5];
  if ((err = __cvt_broken_time (brokentime, riscos_time)) != NULL)
    {
      __ul_seterr (err, EOPSYS);
      return NULL;
    }
  return __standard_time (riscos_time, buffer);
}
