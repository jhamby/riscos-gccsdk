/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/creat.c,v $
 * $Date: 2001/01/29 15:10:22 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#include <fcntl.h>
#include <unixlib/types.h>

int
creat (const char *file, __mode_t mode)
{
  return open (file, O_WRONLY | O_CREAT | O_TRUNC, mode);
}
