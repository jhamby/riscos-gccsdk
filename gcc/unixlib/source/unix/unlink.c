/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/unlink.c,v $
 * $Date: 2004/02/07 18:09:19 $
 * $Revision: 1.8 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: unlink.c,v 1.8 2004/02/07 18:09:19 alex Exp $";
#endif

#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>

#include <unixlib/os.h>
#include <unixlib/local.h>
#include <unixlib/swiparams.h>

/* Removes the suffix swap directory if it is empty.
   The filename passed to it is modified. */
void
__unlinksuffix (char *file)
{
  char *dot;
  int regs[10];

  /* Delete the suffix swap dir if it is now empty */
  dot = strrchr (file, '.');
  if (dot)
    {
      *dot = '\0'; /* Remove leafname */

      while (dot > file && *dot != '.')
        dot--;

      if (*dot == '.')
        dot++;

      if (!(__get_riscosify_control () & __RISCOSIFY_NO_SUFFIX)
          && __sfixfind (dot, strlen (dot)))
        __os_file (OSFILE_DELETENAMEDOBJECT, file, regs); /* This will only delete empty directories */
    }
}

int
unlink (const char *ux_file)
{
  int regs[10], objtype, attr;
  _kernel_oserror *err;
  char file[_POSIX_PATH_MAX];

  if (__object_get_attrs (ux_file, file, sizeof (file),
                          &objtype, NULL, NULL, NULL, NULL, &attr))
    return -1;

  if (objtype == 2 || (objtype == 3 && ! __get_feature_imagefs_is_file ())) /* Directory/Image FS.  */
    return __set_errno (EISDIR);

  /* Check for permission to delete the file. Bit 3 set => it is locked.  */
  if (attr & (1 << 3))
    return __set_errno (EACCES);

  /* Try to zap the file.  */
  err = __os_file (OSFILE_DELETENAMEDOBJECT, file, regs);
  if (err)
    {
      /* Should check for EROFS but this would involve
	 opening/closing the file.  */
      __seterr (err);
      return -1;
    }

  __unlinksuffix (file);

  return 0;
}
