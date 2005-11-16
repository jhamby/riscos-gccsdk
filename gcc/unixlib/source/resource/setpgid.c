/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/resource/setpgid.c,v $
 * $Date: 2002/02/14 15:56:36 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#include <unistd.h>
#include <unixlib/unix.h>

int
setpgid (__pid_t pid, __pid_t pgid)
{
  return setpgrp (pid, pgid);
}
