/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/resource/setpgrp.c,v $
 * $Date: 2003/01/21 17:54:22 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: setpgrp.c,v 1.4 2003/01/21 17:54:22 admin Exp $";
#endif

#include <unistd.h>
#include <unixlib/unix.h>
#include <pthread.h>

int
setpgrp (__pid_t pid, __pid_t pgid)
{
  PTHREAD_UNSAFE
 
  if (__proc->pid == pid)
    __proc->pgrp = pgid;
  return 0;
}
