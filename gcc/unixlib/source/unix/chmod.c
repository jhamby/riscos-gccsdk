/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/chmod.c,v $
 * $Date: 2000/07/15 14:52:43 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: chmod.c,v 1.1.1.1 2000/07/15 14:52:43 nick Exp $";
#endif

#include <errno.h>
#include <limits.h>
#include <unistd.h>

#include <sys/os.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <unixlib/local.h>
#include <unixlib/swiparams.h>

int
chmod (const char *ux_file, mode_t mode)
{
  char file[_POSIX_PATH_MAX];
  _kernel_oserror *err;
  int regs[10], sftype, aftype;

  if (ux_file == NULL)
    return __set_errno (EINVAL);

  if (!__riscosify_std (ux_file, 0, file, sizeof (file), &sftype))
    return __set_errno (ENAMETOOLONG);

  /* Get catalogue information.  */
  err = os_file (OSFILE_READCATINFO_NOPATH, file, regs);
  if (err)
    {
      __seterr (err);
      return __set_errno (EIO);
    }

  /* Does the file has a filetype (at this point we aren't even sure that
     the file exists but that's not a problem, see next 'if') ? */
  if ((regs[2] & 0xfff00000U) == 0xfff00000U)
    aftype = (regs[2] >> 8) & 0xfff;
  else
    aftype = __RISCOSIFY_FILETYPE_NOTFOUND;

  /* Fail if file doesn't exist or (if specified) filetype is different.  */
  if (regs[0] == 0
      || (sftype != __RISCOSIFY_FILETYPE_NOTFOUND && sftype != aftype))
    return __set_errno (ENOENT);

  /* Set the file access permission bits.  */
  regs[5] = __set_protection (mode);
  err = os_file (4, file, regs);
  if (err)
    {
      __seterr (err);
      return __set_errno (EPERM);
    }

  return 0;
}
