/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/time/c/times,v $
 * $Date: 1997/10/09 20:00:47 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: times,v 1.2 1997/10/09 20:00:47 unixlib Exp $";
#endif

/* Territory time support, written by Nick Burrett on 12 July 1997.  */

#include <time.h>
#include <sys/times.h>

clock_t
times (struct tms *tmsp)
{
  tmsp->tms_utime = clock ();	/* user time */
  tmsp->tms_stime = 0;		/* system time */
  tmsp->tms_cutime = 0;		/* user time, children */
  tmsp->tms_cstime = 0;		/* system time, children */
  return tmsp->tms_utime;
}
