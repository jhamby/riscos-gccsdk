/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/unix/c/mkdir,v $
 * $Date: 2000/06/10 12:59:43 $
 * $Revision: 1.11 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: mkdir,v 1.11 2000/06/10 12:59:43 admin Exp $";
#endif

#include <errno.h>
#include <limits.h>
#include <unistd.h>

#include <sys/os.h>
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
  if (os_file (OSFILE_READCATINFO_NOPATH, path, regs) == NULL
      && regs[0] != 0)
    return __set_errno (EEXIST);

  /* Create the directory, with default number of entries per directory.  */
  regs[4] = 0;
  err = os_file (8, path, regs);
  if (err)
    {
      __seterr (err);
      return -1;
    }

  /* Set the file access permission bits.  */
  regs[5] = __set_protection (mode);
  err = os_file (4, path, regs);
  if (err)
    {
      __seterr (err);
      return -1;
    }

  return 0;
}
