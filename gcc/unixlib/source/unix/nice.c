/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/nice.c,v $
 * $Date: 2005/04/24 20:26:59 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: joty $
 *
 ***************************************************************************/

#include <unistd.h>
#include <sys/resource.h>

int
nice (int increment)
{
  int old = getpriority (PRIO_PROCESS, 0);
  return setpriority (PRIO_PROCESS, 0, old + increment);
}
