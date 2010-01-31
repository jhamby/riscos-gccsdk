/* __isdir (), __object_exists ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <limits.h>

#include <unixlib/local.h>
#include <internal/local.h>
#include <internal/unix.h>

/* Return nonzero if DIR is an existent directory.  */
int
__isdir (const char *ux_dir)
{
  char dir[_POSIX_PATH_MAX];
  int filetype;

  if (!__riscosify_std (ux_dir, 0, dir, sizeof (dir), &filetype)
      || filetype != __RISCOSIFY_FILETYPE_NOTFOUND)
    return 0;

#if __UNIXLIB_SYMLINKS
  {
    char target[_POSIX_PATH_MAX];

    if (__resolve_symlinks (dir, target, _POSIX_PATH_MAX) != 0)
      return 0;

    return __isdir_raw (target);
  }
#else
  return __isdir_raw (dir);
#endif
}

/* Return nonzero if FILE exists.  */
int
__object_exists (const char *ux_file)
{
  char file[_POSIX_PATH_MAX];
  int filetype;

  if (!__riscosify_std (ux_file, 0, file, sizeof (file), &filetype)
      || filetype != __RISCOSIFY_FILETYPE_NOTFOUND)
    return 0;

#if __UNIXLIB_SYMLINKS
  {
    char target[_POSIX_PATH_MAX];

    if (__resolve_symlinks (file, target, _POSIX_PATH_MAX) != 0)
      return 0;

    return __object_exists_raw (target);
  }
#else
  return __object_exists_raw (file);
#endif
}
