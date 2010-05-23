/* utimes ()
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#include <utime.h>
#include <sys/time.h>

/* The utimes function is like utime, but also lets you specify the
   fractional part of the file times.

   Change the access time of 'file' to tvp[0] and
   the modification time of 'file' to tvp[1].

   On RISC OS, we just alter the modification time.  */
int
utimes (const char *file, const struct timeval tvp[2])
{
  /* We must convert the micro-seconds to seconds.  */
  struct utimbuf time_buf;
  time_buf.modtime = (tvp[1].tv_usec / 1000) + tvp[1].tv_sec;
  return utime (file, &time_buf);
}
