/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/stdio/c/remove,v $
 * $Date: 1997/10/09 20:00:36 $
 * $Revision: 1.8 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: remove,v 1.8 1997/10/09 20:00:36 unixlib Exp $";
#endif

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
