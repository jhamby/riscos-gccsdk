/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/chdir.c,v $
 * $Date: 2003/05/11 12:49:07 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: joty $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: chdir.c,v 1.6 2003/05/11 12:49:07 joty Exp $";
#endif

#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <unixlib/os.h>
#include <swis.h>
#include <unixlib/unix.h>

#include <unixlib/local.h>

/* Change the current directory to PATH, taking into consideration any setting
   of Prefix$Dir.  */
int
chdir (const char *ux_path)
{
  char *prefix_path, *full_path;
  _kernel_oserror *err;
  int regs[10], objtype;
  char canon_path[_POSIX_PATH_MAX];
  char path[_POSIX_PATH_MAX];
  int riscosify_control, rtrn_get_attrs;

  /* Filter this trivial case, otherwise we have to take care of this
     case later on (Unix '.' -> '@' in RISC OS).  */
  if (ux_path[0] == '.' && ux_path[1] == '\0')
    return 0;

  /* We don't want to do suffix swapping for directory objects.  */
  riscosify_control = __get_riscosify_control ();
  __set_riscosify_control (riscosify_control | __RISCOSIFY_NO_SUFFIX);

  rtrn_get_attrs = __object_get_attrs (ux_path, path, sizeof (path),
                                       &objtype, NULL, NULL, NULL, NULL, NULL);

  /* Restore suffix swapping status.  */
  __set_riscosify_control (riscosify_control);

  if (rtrn_get_attrs)
    return -1;

  /* We can only change to directories.  */
  if ((objtype & 2) == 0)
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
          err = __os_swi (DDEUtils_Prefix, regs);

          if (! err)
            return 0;
          __seterr (err);
        }
      else
        {
          int path_len = strlen (path);
          int prefix_len = strlen (prefix_path);

          /* Use malloc rather than a fixed buffer since path could
             contain parent components (^.), so could be longer than
             _POSIX_PATH_MAX before canonicalisation. */
          full_path = realloc (prefix_path, prefix_len + 1 + path_len + 1);
          if (full_path)
            {
              /* Append 'prefix' with relative path but take care of the
                 RISC OS CSD indicator.  */
              const char *rel_path = (path[0] == '@' && path[1] == '.') ? path + 2 : path;

              full_path[prefix_len++] = '.';
              strcpy (full_path + prefix_len, rel_path);

              /* We must pass a canonicalised directory to
              	 DDEUtils_Prefix.  */
	      regs[0] = 37;
              regs[1] = (int) full_path;
              regs[2] = (int) canon_path;
              regs[3] = regs[4] = 0;
              regs[5] = sizeof (canon_path);

              err = __os_swi (OS_FSControl, regs);
              if (!err)
                {
                  regs[0] = (int) canon_path;
                  err = __os_swi (DDEUtils_Prefix, regs);
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
  err = __os_fsctrl (0, path, 0, 0);
  if (err)
    {
      __seterr (err);
      return -1;
    }

  return 0;
}
