/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/time/utimes.c,v $
 * $Date: 2000/07/15 14:52:43 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: utimes.c,v 1.1.1.1 2000/07/15 14:52:43 nick Exp $";
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
