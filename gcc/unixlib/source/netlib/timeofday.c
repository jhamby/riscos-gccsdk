/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/netlib/c/timeofday,v $
 * $Date: 1997/10/09 20:00:23 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: timeofday,v 1.6 1997/10/09 20:00:23 unixlib Exp $";
#endif

#include <sys/time.h>
#include <time.h>

void
gettimeofday (struct timeval *tv, struct timezone *tz)
{
  time_t t;
  struct tm *tmptr;

  t = time (NULL);
  tmptr = localtime (&t);

  tv->tv_sec = tmptr->tm_sec + tmptr->tm_min * 60 + tmptr->tm_hour * 60 * 60;
  tv->tv_usec = 0;
}
