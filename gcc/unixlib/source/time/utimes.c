/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/time/c/utimes,v $
 * $Date: 1997/10/09 20:00:47 $
 * $Revision: 1.9 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: utimes,v 1.9 1997/10/09 20:00:47 unixlib Exp $";
#endif

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
  struct utimbuf time_buf;

  /* We must convert the micro-seconds to centi-seconds.  */
  time_buf.modtime = (tvp[1].tv_usec / 10000) + tvp[1].tv_sec * 100;
  return utime (file, &time_buf);
}
