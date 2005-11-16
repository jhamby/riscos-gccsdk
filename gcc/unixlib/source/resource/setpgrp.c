/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/resource/setpgrp.c,v $
 * $Date: 2005/03/04 20:59:06 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

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
