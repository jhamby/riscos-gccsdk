/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/timeofday.c,v $
 * $Date: 2000/07/15 14:52:26 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: timeofday.c,v 1.1.1.1 2000/07/15 14:52:26 nick Exp $";
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
