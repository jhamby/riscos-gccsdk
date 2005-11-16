/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/time/times.c,v $
 * $Date: 2002/02/14 15:56:38 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

/* Territory time support, written by Nick Burrett on 12 July 1997.  */

#include <time.h>
#include <sys/times.h>

clock_t
times (struct tms *tmsp)
{
  clock_t utime = clock ();
  if (tmsp)
    {
      tmsp->tms_utime = utime;	/* user time */
      tmsp->tms_stime = 0;	/* system time */
      tmsp->tms_cutime = 0;	/* user time, children */
      tmsp->tms_cstime = 0;	/* system time, children */
    }
  return utime;
}
