/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/mkdir.c,v $
 * $Date: 2003/04/12 11:31:39 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: mkdir.c,v 1.4 2003/04/12 11:31:39 alex Exp $";
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
  int regs[6], objtype;
  _kernel_oserror *err;
  char path[_POSIX_PATH_MAX];

  if (__object_get_attrs (ux_path, path, sizeof (path),
                          &objtype, NULL, NULL, NULL, NULL, NULL) && errno != ENOENT)
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
