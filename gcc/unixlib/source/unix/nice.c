/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/unix/c/nice,v $
 * $Date: 1997/10/09 20:00:52 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: nice,v 1.6 1997/10/09 20:00:52 unixlib Exp $";
#endif

#include <sys/resource.h>

int
nice (int increment)
{
  int old = getpriority (PRIO_PROCESS, 0);
  return setpriority (PRIO_PROCESS, 0, old + increment);
}
