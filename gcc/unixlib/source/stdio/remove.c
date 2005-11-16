/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/remove.c,v $
 * $Date: 2001/01/29 15:10:21 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int
remove (const char *file)
{
  int save_errno, status;

  /* Preserve errno.  */
  save_errno = errno;

  /* Try and delete the file.  */
  status = unlink (file);
  if (status == -1 && errno == EISDIR)
    {
      (void) __set_errno (save_errno);
      /* The pathname specifies a directory.  */
      status = rmdir (file);
    }
  return status;
}
