/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/access.c,v $
 * $Date: 2000/07/15 14:52:43 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: access.c,v 1.1.1.1 2000/07/15 14:52:43 nick Exp $";
#endif

#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

int
access (const char *path, int mode)
{
  struct stat stats;

  if (stat (path, &stats))
    return -1;

  if (mode & F_OK)
    return 0;  /* File exists.  */

  /* Clear any bad bits.  */
  mode &= (X_OK | W_OK | R_OK);

  /* Write access.  */
  if (mode & W_OK)
    if (!(stats.st_mode & (S_IWUSR | S_IWGRP)))
      return __set_errno (EACCES);

  /* Read/execute access.  */
  if (mode & (R_OK | X_OK))
    if (!(stats.st_mode & (S_IRUSR | S_IXUSR | S_IRGRP | S_IXGRP)))
      return __set_errno (EACCES);

  return 0;
}
