/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/time/time.c,v $
 * $Date: 2001/09/04 16:32:04 $
 * $Revision: 1.2.2.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: time.c,v 1.2.2.2 2001/09/04 16:32:04 admin Exp $";
#endif

/* Territory time support, written by Nick Burrett on 12 July 1997.  */

#include <time.h>
#include <unixlib/os.h>
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
  err = __os_word (14, buf);
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
