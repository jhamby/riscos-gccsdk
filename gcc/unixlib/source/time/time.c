/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/time/c/time,v $
 * $Date: 1997/12/17 22:27:08 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: time,v 1.3 1997/12/17 22:27:08 unixlib Exp $";
#endif

/* Territory time support, written by Nick Burrett on 12 July 1997.  */

#include <time.h>
#include <sys/os.h>
#include <unixlib/local.h>

/* #define DEBUG */

#ifdef DEBUG
#include <stdio.h>
#endif

time_t
time (time_t * time)
{
  unsigned int buf[2];
  _kernel_oserror *err;
  time_t time1;

  buf[0] = 3;
  err = os_word (14, buf);
  if (err)
    {
      __seterr (err);
      return -1;
    }

  time1 = __cvt_riscos_time (buf[1] & 0xff, buf[0]);

#ifdef DEBUG
  printf ("time():  t1 = %x, b[1] = %x, b[0] = %x\n", time1, buf[1], buf[0]);
#endif

  if (time != NULL)
    *time = time1;

  return time1;
}
