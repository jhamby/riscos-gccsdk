/****************************************************************************
 *
 * $Source$
 * $Date$
 * $Revision$
 * $State$
 * $Author$
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id$";
#endif

#include <unistd.h>
#include <unixlib/unix.h>
#include <pthread.h>

int
setpgrp (__pid_t pid, __pid_t pgid)
{
  PTHREAD_UNSAFE
  
  if (__u->pid == pid)
    __u->pgrp = pgid;
  return 0;
}
