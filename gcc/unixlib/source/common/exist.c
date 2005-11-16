/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/common/exist.c,v $
 * $Date: 2001/01/29 15:10:19 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#include <limits.h>
#include <unixlib/local.h>

/* Return nonzero if DIR is an existent directory.  */
int
__isdir (const char *ux_dir)
{
  char dir[_POSIX_PATH_MAX];
  int filetype;

  if (! __riscosify_std (ux_dir, 0, dir, sizeof (dir), &filetype)
      || filetype != __RISCOSIFY_FILETYPE_NOTFOUND)
    return 0;

  return __isdir_raw (dir);
}

/* Return nonzero if FILE exists.  */
int
__object_exists (const char *ux_file)
{
  char file[_POSIX_PATH_MAX];
  int filetype;

  if (! __riscosify_std (ux_file, 0, file, sizeof (file), &filetype)
      || filetype != __RISCOSIFY_FILETYPE_NOTFOUND)
    return 0;

  return __object_exists_raw (file);
}
