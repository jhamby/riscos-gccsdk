/* Change directory.
   Copyright (c) 2004-2011 UnixLib Developers.  */

#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <swis.h>

#include <unixlib/local.h>
#include <internal/os.h>
#include <internal/unix.h>
#include <internal/local.h>

/* Change the current directory to PATH, taking into consideration any
   setting of Prefix$Dir.  */
int
chdir (const char *ux_path)
{
  const _kernel_oserror *err;

  /* Filter this trivial case, otherwise we have to take care of this
     case later on (Unix '.' -> '@' in RISC OS).  */
  if (ux_path[0] == '.' && ux_path[1] == '\0')
    return 0;

  /* We don't want to do suffix swapping for directory objects.  */
  const int riscosify_control = __get_riscosify_control ();
  __set_riscosify_control (riscosify_control | __RISCOSIFY_NO_SUFFIX);

  int objtype;
  char ro_path[_POSIX_PATH_MAX];
  int rtrn_get_attrs = __object_get_attrs (ux_path, ro_path, sizeof (ro_path),
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
     is we use the DDEUtils_Prefix call exclusively (i.e. we don't use
     OS_FSControl 0 as it would give wrong results).  */
  const char *prefix_path;
  if ((prefix_path = __get_dde_prefix()) != NULL)
    {
      /* Now we have to see if the supplied unix path is an absolute one
         or a relative one.  If absolute, we define Prefix$Dir as the
         riscosified unix path.  If relative, we concat Prefix$Dir with
         the relative riscosified unix path and define that as Prefix$Dir. */
      if (ux_path[0] == '/') /* Cfr. riscosify.c */
        {
          free((void *)prefix_path);

	  if ((err = SWI_DDEUtils_Prefix (ro_path)) != NULL)
	    return __ul_seterr (err, EOPSYS);

          return 0;
        }

      size_t path_len = strlen (ro_path);
      size_t prefix_len = strlen (prefix_path);

      /* Use malloc rather than a fixed buffer since path could
         contain parent components (^.), so could be longer than
         _POSIX_PATH_MAX before canonicalisation. */
      char *full_path = realloc ((void *)prefix_path,
				 prefix_len + 1 + path_len + 1);
      if (full_path == NULL)
	{
          free ((void *)prefix_path);
	  return -1;
        }

      /* Append 'prefix' with relative path but take care of the RISC OS CSD
	 indicator.  */
      const char *rel_path = (ro_path[0] == '@' && ro_path[1] == '.') ? ro_path + 2 : ro_path;

      full_path[prefix_len++] = '.';
      strcpy (full_path + prefix_len, rel_path);

      /* We must pass a canonicalised directory to DDEUtils_Prefix.  */
      char canon_path[_POSIX_PATH_MAX];
      int regs[10];
      regs[0] = 37;
      regs[1] = (int) full_path;
      regs[2] = (int) canon_path;
      regs[3] = regs[4] = 0;
      regs[5] = sizeof (canon_path);

      err = __os_swi (OS_FSControl, regs);
      if (!err)
	err = SWI_DDEUtils_Prefix (canon_path);
      free (full_path);

      if (err)
        return __ul_seterr (err, EOPSYS);
      return 0;
    }

  /* No Prefix$Dir, so just change directory with OS_FSControl 0.  */
  if ((err = __os_fsctrl (0, ro_path, 0, 0)) != NULL)
    return __ul_seterr (err, EOPSYS);

  return 0;
}
