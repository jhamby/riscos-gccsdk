/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/resource/setpgid.c,v $
 * $Date: 2000/07/15 14:52:28 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: setpgid.c,v 1.1.1.1 2000/07/15 14:52:28 nick Exp $";
#endif

#include <unistd.h>
#include <sys/unix.h>

int
setpgid (__pid_t pid, __pid_t pgid)
{
  return setpgrp (pid, pgid);
}
