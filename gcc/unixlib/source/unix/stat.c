/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/stat.c,v $
 * $Date: 2000/07/15 14:52:45 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: stat.c,v 1.1.1.1 2000/07/15 14:52:45 nick Exp $";
#endif

#include <errno.h>
#include <limits.h>

#include <sys/dev.h>
#include <sys/os.h>
#include <sys/stat.h>

#include <unixlib/local.h>
#include <unixlib/swiparams.h>

int
stat (const char *ux_filename, struct stat *buf)
{
  char filename[_POSIX_PATH_MAX];
  _kernel_oserror *err;
  int regs[6], sftype, aftype;

  if (buf == NULL)
    return __set_errno (EINVAL);

  if (!__riscosify_std (ux_filename, 0, filename, sizeof (filename),
      &sftype))
    return __set_errno (ENAMETOOLONG);

  /* Get vital file statistics and use File$Path.  */
  err = os_file (OSFILE_READCATINFO, filename, regs);
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

  buf->st_dev = DEV_RISCOS;
  buf->st_ino = __get_file_ino (NULL, filename);

  __stat (regs, buf);

  return 0;
}
