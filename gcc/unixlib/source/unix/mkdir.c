/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/mkdir.c,v $
 * $Date: 2002/02/14 15:56:38 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: mkdir.c,v 1.3 2002/02/14 15:56:38 admin Exp $";
#endif

#include <errno.h>
#include <limits.h>
#include <unistd.h>

#include <unixlib/os.h>
#include <sys/stat.h>

#include <unixlib/local.h>
#include <unixlib/swiparams.h>

int
mkdir (const char *ux_path, __mode_t mode)
{
  int regs[6], filetype, objtype;
  _kernel_oserror *err;
  char path[_POSIX_PATH_MAX];

  if (__object_get_attrs (ux_path, path, sizeof (path),
                          &objtype, &filetype, NULL, NULL, NULL, NULL) && errno != ENOENT)
    return -1;

  /* Fail if the directory already exists.  */
  if (objtype != 0)
    return __set_errno (EEXIST);

  /* Create the directory, with default number of entries per directory.  */
  regs[4] = 0;
  err = __os_file (8, path, regs);
  if (err)
    {
      __seterr (err);
      return -1;
    }

  /* Set the file access permission bits.  */
  regs[5] = __set_protection (mode);
  err = __os_file (4, path, regs);
  if (err)
    {
      __seterr (err);
      return -1;
    }

  return 0;
}
