/* utime () : change access and/or modification times of an inode.
   Copyright (c) 2005-2013 UnixLib Developers.  */

#include <stddef.h>
#include <time.h>
#include <utime.h>
#include <sys/time.h>

/* The access time is ignored.  RISC OS does not support that.  */
int
utime (const char *ux_filename, const struct utimbuf *times)
{
  struct timeval tvp[2];
  if (times != NULL)
    {
      /* Don't bother filling in tvp[0].  */
      tvp[1].tv_sec = times->modtime;
      tvp[1].tv_usec = 0;
    }

  return utimes (ux_filename, times ? tvp : NULL);
}

