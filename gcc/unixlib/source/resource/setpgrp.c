/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/resource/setpgrp.c,v $
 * $Date: 2001/09/04 16:32:04 $
 * $Revision: 1.2.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: setpgrp.c,v 1.2.2.1 2001/09/04 16:32:04 admin Exp $";
#endif

#include <unistd.h>
#include <unixlib/unix.h>

int
setpgrp (__pid_t pid, __pid_t pgid)
{
  if (__u->pid == pid)
    __u->pgrp = pgid;
  return 0;
}
