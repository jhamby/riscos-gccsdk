/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/resource/setpgid.c,v $
 * $Date: 2001/09/04 16:32:04 $
 * $Revision: 1.2.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: setpgid.c,v 1.2.2.1 2001/09/04 16:32:04 admin Exp $";
#endif

#include <unistd.h>
#include <unixlib/unix.h>

int
setpgid (__pid_t pid, __pid_t pgid)
{
  return setpgrp (pid, pgid);
}
