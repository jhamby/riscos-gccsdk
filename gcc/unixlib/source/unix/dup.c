/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/unix/c/dup,v $
 * $Date: 1997/10/09 20:00:49 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: dup,v 1.6 1997/10/09 20:00:49 unixlib Exp $";
#endif

#include <fcntl.h>
#include <unistd.h>

int
dup (int fd)
{
  return fcntl (fd, F_DUPFD, 0);
}
