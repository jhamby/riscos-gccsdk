/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/stat.c,v $
 * $Date: 2001/09/04 16:32:04 $
 * $Revision: 1.2.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: stat.c,v 1.2.2.1 2001/09/04 16:32:04 admin Exp $";
#endif

#include <errno.h>
#include <limits.h>
#include <string.h>

#include <unixlib/dev.h>
#include <unixlib/os.h>
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
  err = __os_file (OSFILE_READCATINFO, filename, regs);
  if (err)
    {
      __seterr (err);
      return __set_errno (EIO);
    }

  /* Trap case 'stat("/dev/null")' */
  if (strcmp(filename, "null:") == 0)
    {
    buf->st_ino = 0;
    regs[0] = regs[2] = regs[3] = regs[4] = regs[5] = 0;
    buf->st_dev = DEV_NULL;
    __stat(regs, buf);
    buf->st_mode = S_IRUSR | S_IWUSR;
    return 0;
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
