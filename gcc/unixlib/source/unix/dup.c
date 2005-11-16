/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/dup.c,v $
 * $Date: 2001/01/29 15:10:22 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#include <fcntl.h>
#include <unistd.h>

int
dup (int fd)
{
  return fcntl (fd, F_DUPFD, 0);
}
