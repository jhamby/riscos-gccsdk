/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/chmod.c,v $
 * $Date: 2002/02/14 15:56:38 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: chmod.c,v 1.3 2002/02/14 15:56:38 admin Exp $";
#endif

#include <errno.h>
#include <limits.h>
#include <unistd.h>

#include <unixlib/os.h>
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

  if (__object_get_attrs (ux_file, file, sizeof (file),
                          NULL, NULL, NULL, NULL, NULL, NULL))
    return -1;

  /* Set the file access permission bits.  */
  regs[5] = __set_protection (mode);
  err = __os_file (4, file, regs);
  if (err)
    {
      __seterr (err);
      return __set_errno (EPERM);
    }

  return 0;
}
