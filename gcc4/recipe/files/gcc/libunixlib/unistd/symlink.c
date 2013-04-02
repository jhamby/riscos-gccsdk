/* symlink ()
 * Copyright (c) 2002-2013 UnixLib Developers
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
#ifdef DEBUG
  debug_printf("-- Symlinking %s --> %s\n", ux_newfile, ux_targetfile);
#endif

  unsigned fd = 0;
  int result = 0;

  /* Use one buffer for both paths.  */
  char *paths = NULL;
  if ((paths = malloc (MAXPATHLEN * 2)) == NULL)
    {
      result = __set_errno (ENOMEM);
      goto exit;
    }

  char *ro_newfile = paths;
  char *ro_targetfile = paths + MAXPATHLEN;

  if (__riscosify_std (ux_newfile, 0,
		       ro_newfile, MAXPATHLEN, NULL) == NULL)
    {
      result = __set_errno (ENAMETOOLONG);
      goto exit;
    }

  /* Don't overwrite an existing file with a symlink file.  */
  if (__object_exists_ro (ro_newfile))
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

  const unsigned int symlink_id = SYMLINK_ID;
  const unsigned link_len = strlen (ro_targetfile);
  const _kernel_oserror *err;
  if ((err = SWI_OS_Find_Open (OSFIND_OPENOUT, ro_newfile, &fd)) == NULL
      /* Write the symlink file ID - ASCII representation of LINK.  */
      && (err = SWI_OS_GBPB_WriteBytes (fd, &symlink_id, 4, NULL)) == NULL
      /* Write the length of the target pathname - 4 bytes.  */
      && (err = SWI_OS_GBPB_WriteBytes (fd, &link_len, 4, NULL)) == NULL
      /* Write the target pathname.  */
      && (err = SWI_OS_GBPB_WriteBytes (fd, ro_targetfile, link_len, NULL)) == NULL
      /* Set the filetype of the symlink file.  */
      && (err = SWI_OS_File_WriteCatInfoFileType (ro_newfile, SYMLINK_FILETYPE)) != NULL)
    result = __ul_seterr (err, EIO);

  /* Fall through. */
exit:
  free (paths);

  if (fd != 0)
    SWI_OS_Find_Close (fd);

  return result;
#else
  ux_targetfile = ux_targetfile;
  ux_newfile = ux_newfile;

  return __set_errno (ENOSYS);
#endif
}
