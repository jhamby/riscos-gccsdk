/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/remove.c,v $
 * $Date: 2000/07/15 14:52:32 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: remove.c,v 1.1.1.1 2000/07/15 14:52:32 nick Exp $";
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
