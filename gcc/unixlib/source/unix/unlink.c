/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/unix/c/unlink,v $
 * $Date: 2000/06/10 12:59:43 $
 * $Revision: 1.12 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: unlink,v 1.12 2000/06/10 12:59:43 admin Exp $";
#endif

#include <errno.h>
#include <limits.h>
#include <unistd.h>

#include <sys/os.h>
#include <unixlib/local.h>
#include <unixlib/swiparams.h>

int
unlink (const char *ux_file)
{
  int regs[10], sftype, aftype;
  _kernel_oserror *err;
  char file[_POSIX_PATH_MAX];

  if (ux_file == NULL)
    return __set_errno (EINVAL);

  if (!__riscosify_std (ux_file, 0, file, sizeof (file), &sftype))
    return __set_errno (ENAMETOOLONG);

  /* Check no RISC OS equivalent.  */
  if (*file == '\0')
    return __set_errno (ENOENT);

  /* Get file's catalogue entry (unfortunately uses wildcards.
     Still, shouldn't be a major problem).  */
  err = os_file (OSFILE_READCATINFO_NOPATH, file, regs);
  if (err)
    {
      __seterr (err);
      return -1;
    }

  /* Does the file has a filetype (at this point we aren't even sure that
     the file exists but that's not a problem, see next 'if') ? */
  if ((regs[2] & 0xfff00000U) == 0xfff00000)
    aftype = (regs[2] >> 8) & 0xfff;
  else
    aftype = __RISCOSIFY_FILETYPE_NOTFOUND;

  /* Fail if file doesn't exist or (if specified) filetype is different.  */
  if (regs[0] == 0 /* Non-existent.  */
      || (sftype != __RISCOSIFY_FILETYPE_NOTFOUND && sftype != aftype))
    return __set_errno (ENOENT);

  if (regs[0] == 2 || regs[0] == 3) /* Directory/Image FS.  */
    return __set_errno (EISDIR);

  /* Check for permission to delete the file. Bit 3 set => it is locked.  */
  if (regs[5] & (1 << 3))
    return __set_errno (EACCES);

  /* Try to zap the file.  */
  err = os_file (6, file, regs);
  if (err)
    {
      /* Should check for EROFS but this would involve
	 opening/closing the file.  */
      __seterr (err);
      return -1;
    }

  return 0;
}
