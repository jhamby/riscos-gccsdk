/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/time/times.c,v $
 * $Date: 2000/07/15 14:52:43 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: times.c,v 1.1.1.1 2000/07/15 14:52:43 nick Exp $";
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
