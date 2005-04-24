/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/nice.c,v $
 * $Date: 2001/01/29 15:10:22 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: nice.c,v 1.2 2001/01/29 15:10:22 admin Exp $";
#endif

#include <unistd.h>
#include <sys/resource.h>

int
nice (int increment)
{
  int old = getpriority (PRIO_PROCESS, 0);
  return setpriority (PRIO_PROCESS, 0, old + increment);
}
