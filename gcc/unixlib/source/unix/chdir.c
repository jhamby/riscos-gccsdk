/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/chdir.c,v $
 * $Date: 2000/07/15 14:52:43 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: chdir.c,v 1.1.1.1 2000/07/15 14:52:43 nick Exp $";
#endif

#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/os.h>
#include <sys/swis.h>
#include <sys/unix.h>

#include <unixlib/local.h>

/* Change the current directory to PATH, taking into consideration any setting
   of Prefix$Dir.  */
int
chdir (const char *ux_path)
{
  char *prefix_path, *full_path;
  _kernel_oserror *err;
  int regs[10], filetype;
  char canon_path[_POSIX_PATH_MAX];
  char path[_POSIX_PATH_MAX];
  char *path_end;

  if (ux_path == NULL)
    return __set_errno (EINVAL);

  path_end = __riscosify_std (ux_path, 0, path, sizeof (path), &filetype);
  if (path_end == NULL || filetype != __RISCOSIFY_FILETYPE_NOTFOUND)
    return __set_errno (ENAMETOOLONG);

  /* Check for existence.  */
  if (!__object_exists_raw (path))
    return __set_errno (ENOENT);

  /* We can only change to directories.  */
  if (!__isdir_raw (path))
    return __set_errno (ENOTDIR);

  /* All of the above path checks are affected by Prefix$Dir. Unfortunately
     OS_FSControl 0 does not use Prefix$Dir if it is set when changing
     directory. So, we check to see whether Prefix$Dir is set first and if it
     is we use the DDEUtils_Prefix call.  */

  prefix_path = __getenv_from_os ("Prefix$Dir", NULL, 0);
  if (prefix_path != NULL && *prefix_path != '\0')
    {
      /* Now we have to see if the supplied unix path is an absolute one
         or a relative one.  If absolute, we define Prefix$Dir as the
         riscosified unix path.  If relative, we concat Prefix$Dir with
         the relative riscosified unix path and define that as Prefix$Dir. */
      if (ux_path[0] == '/') /* Cfr. riscosify.c */
        {
          free(prefix_path);

          regs[0] = (int) path;
          err = os_swi (DDEUtils_Prefix, regs);

          if (! err)
            return 0;
          __seterr (err);
        }
      else
        {
          int len = strlen (prefix_path);

          /* Use malloc rather than a fixed buffer since path could
             contain parent components (^.), so could be longer than
             _POSIX_PATH_MAX before canonicalisation. */
          full_path = realloc (prefix_path, len + 1 + (path_end - path) + 1);
          if (full_path)
            {
              full_path[len++] = '.';
              strcpy (full_path + len, path);

              /* We must pass a canonicalised directory to
              	 DDEUtils_Prefix.  */
	      regs[0] = 37;
              regs[1] = (int) full_path;
              regs[2] = (int) canon_path;
              regs[3] = regs[4] = 0;
              regs[5] = sizeof (canon_path);

              err = os_swi (OS_FSControl, regs);
              if (!err)
                {
                  regs[0] = (int) canon_path;
                  err = os_swi (DDEUtils_Prefix, regs);
                }
              free (full_path);

              if (!err)
                return 0;
              __seterr (err);
            }
          else
            free (prefix_path);
        }

      /* Failed to set path, so return an error rather than resorting to old
	 OS_FSControl 0 method which would be wrong given a prefix dir.  */
      return -1;
    }

  /* No Prefix$Dir, so just change directory with OS_FSControl 0.  */
  err = os_fsctrl (0, path, 0, 0);
  if (err)
    {
      __seterr (err);
      return -1;
    }

  return 0;
}
