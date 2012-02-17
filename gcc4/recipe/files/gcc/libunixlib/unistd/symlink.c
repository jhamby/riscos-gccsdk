/* symlink ()
 * Copyright (c) 2002-2012 UnixLib Developers
 */

#include <errno.h>
#include <unistd.h>
#include <internal/symlinks.h>
#include <internal/os.h>
#include <internal/swiparams.h>
#include <sys/param.h>
#include <unixlib/local.h>
#include <internal/local.h>
#include <stdlib.h>
#include <string.h>

/*#define DEBUG 1*/
#ifdef DEBUG
#  include <sys/debug.h>
#endif

/* Create a symlink file called UX_NEWFILE containing a link to file
   UX_TARGETFILE. Format of file is:

   +0 LINK				4 bytes - ASCII representation of "LINK"
   +4 Length of target pathname		4 bytes
   +8 Target pathname

   Returns 0 if it succeeds or -1 if error occurs, placing the error
   code in errno.
*/
int
symlink (const char *ux_targetfile, const char *ux_newfile)
{
#if __UNIXLIB_SYMLINKS
  const unsigned int symlink_id = SYMLINK_ID;
  char *paths = NULL, *ro_targetfile, *ro_newfile;
  int fd = -1, result = 0;
  int regs[10];

#ifdef DEBUG
  debug_printf("-- Symlinking %s --> %s\n", ux_newfile, ux_targetfile);
#endif

  /* Use one buffer for both paths.  */
  if ((paths = malloc (MAXPATHLEN * 2)) == NULL)
    {
      result = __set_errno (ENOMEM);
      goto exit;
    }

  ro_newfile = paths;
  ro_targetfile = paths + MAXPATHLEN;

  if (__riscosify_std (ux_newfile, 0,
		       ro_newfile, MAXPATHLEN, NULL) == NULL)
    {
      result = __set_errno (ENAMETOOLONG);
      goto exit;
    }

  /* Don't overwrite an existing file with a symlink file.  */
  if (__object_exists_raw (ro_newfile))
    {
      result = __set_errno (EEXIST);
      goto exit;
    }

  /* Symlink files always contain RISC OS format pathnames.  */
  if (__riscosify_std (ux_targetfile, 0,
		       ro_targetfile, MAXPATHLEN, NULL) == NULL)
    {
      result = __set_errno (ENAMETOOLONG);
      goto exit;
    }

  if (__os_fopen (OSFILE_OPENOUT, ro_newfile, &fd) != NULL)
    {
      result = __set_errno (EIO);
      goto exit;
    }

  /* Write the symlink file ID - ASCII representation of LINK.  */
  if (__os_fwrite (fd, &symlink_id, 4, regs) != NULL)
    {
      result = __set_errno (EIO);
      goto exit;
    }

  /* Write the length of the target pathname - 4 bytes.  */
  int link_len = strlen (ro_targetfile);
  if (__os_fwrite (fd, &link_len, 4, regs) != NULL)
    {
      result = __set_errno (EIO);
      goto exit;
    }

  /* Write the target pathname.  */
  if (__os_fwrite (fd, ro_targetfile, link_len, regs) != NULL)
    {
      result = __set_errno (EIO);
      goto exit;
    }

  /* Set the filetype of the symlink file.  */
  regs[2] = SYMLINK_FILETYPE;
  if (__os_file (OSFILE_WRITECATINFO_FILETYPE, ro_newfile, regs) != NULL)
    result = __set_errno (EIO);

  /* Fall through. */
exit:
  free (paths);

  if (fd != -1)
    __os_fclose (fd);

  return result;
#else
  ux_targetfile = ux_targetfile;
  ux_newfile = ux_newfile;

  return __set_errno (ENOSYS);
#endif
}
