/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/mkdir.c,v $
 * $Date: 2001/09/04 16:32:04 $
 * $Revision: 1.2.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: mkdir.c,v 1.2.2.1 2001/09/04 16:32:04 admin Exp $";
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
  int regs[6], filetype;
  _kernel_oserror *err;
  char path[_POSIX_PATH_MAX];

  if (ux_path == NULL)
    return __set_errno (EINVAL);

  if (!__riscosify_std (ux_path, 1, path, sizeof (path), &filetype))
    return __set_errno (ENAMETOOLONG);

  if (filetype != __RISCOSIFY_FILETYPE_NOTFOUND)
    return __set_errno (ENOTDIR);

  /* Fail if the directory already exists.  */
  if (__os_file (OSFILE_READCATINFO_NOPATH, path, regs) == NULL
      && regs[0] != 0)
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
