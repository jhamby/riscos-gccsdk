/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/chmod.c,v $
 * $Date: 2003/04/12 11:31:39 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: chmod.c,v 1.4 2003/04/12 11:31:39 alex Exp $";
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
  int regs[10];

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
