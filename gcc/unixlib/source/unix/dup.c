/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/dup.c,v $
 * $Date: 2000/07/15 14:52:43 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: dup.c,v 1.1.1.1 2000/07/15 14:52:43 nick Exp $";
#endif

#include <fcntl.h>
#include <unistd.h>

int
dup (int fd)
{
  return fcntl (fd, F_DUPFD, 0);
}
