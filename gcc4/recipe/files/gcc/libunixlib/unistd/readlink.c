/* readlink ()
 * Copyright (c) 2002-2013 UnixLib Developers
 */

#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <unixlib/local.h>
#include <sys/param.h>

#include <internal/local.h>
#include <internal/os.h>
#include <internal/symlinks.h>

/* Resolve symlink(s) and return target path for UX_LINKFILE in UX_TARGET

   Returns strlen(UX_TARGET) if it succeeds or -1 if error occurs,
   placing the error code in errno.  */
ssize_t
readlink (const char *ux_linkfile, char *ux_target, size_t ux_target_bufsz)
{
#if __UNIXLIB_SYMLINKS
  if (ux_target == NULL)
    return __set_errno (EFAULT);

  /* Convert to RISC OS path and ensure we're starting with a symlink.
     __object_get_lattrs will check on ux_linkfile being NULL or "".  */
  char ro_linkfile[MAXPATHLEN];
  int filetype, objtype;
  if (__object_get_lattrs (ux_linkfile, ro_linkfile, sizeof (ro_linkfile),
			   &objtype, &filetype, NULL, NULL, NULL, NULL) != 0)
    return -1;
  if (objtype != 1 || filetype != SYMLINK_FILETYPE)
    return __set_errno (EINVAL);

  /* Set one_step_only to 1 as we don't want to persue the symlink chain at
     all, only the first symlink needs to be resolved.  */
  char ro_target[MAXPATHLEN];
  if (__resolve_symlinks (ro_linkfile, ro_target, sizeof (ro_target), 1) != 0)
    return -1;

  /* Translate target path to Unix.  */
  char *ux_buf = __unixify_std (ro_target, NULL, 0,
				__RISCOSIFY_FILETYPE_NOTSPECIFIED);
  if (ux_buf == NULL)
    return __set_errno (ENAMETOOLONG);

  /* Do not copy terminating \0 char.
     If to_copy is bigger than SSIZE_MAX then the caller will probably
     incorrectly assume this call has been failed, but that means more than
     a 2 GByte long symlink, a case which probably has let to an explosion
     somewhere else. */
  size_t size_needed = strlen (ux_buf);
  size_t to_copy = size_needed < ux_target_bufsz ? size_needed : ux_target_bufsz;
  memcpy (ux_target, ux_buf, to_copy);
  free (ux_buf);

  return (ssize_t)to_copy;
#else
  ux_linkfile = ux_linkfile;
  ux_target = ux_target;
  ux_target_bufsz = ux_target_bufsz;

  return __set_errno (ENOSYS);
#endif
}

