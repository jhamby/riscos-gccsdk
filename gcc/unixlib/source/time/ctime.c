/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/time/c/ctime,v $
 * $Date: 1997/10/09 20:00:45 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: ctime,v 1.2 1997/10/09 20:00:45 unixlib Exp $";
#endif

/* Territory time support, written by Nick Burrett on 12 July 1997.  */

#include <time.h>
#include <unixlib/local.h>

char *
ctime (const time_t *timer)
{
  unsigned int riscos_time[2];

  /* Convert calendar time to 5 byte RISC OS time.  */
  __cvt_unix_time (*timer, &riscos_time[1], &riscos_time[0]);
  return __standard_time ((const char *)riscos_time);
}
