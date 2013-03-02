/* __object_exists_ux (), __object_exists_ro ()
   Copyright (c) 2000-2013 UnixLib Developers.  */

#include <limits.h>

#include <unixlib/local.h>
#include <internal/local.h>
#include <internal/os.h>
#include <internal/unix.h>

/* Return 0 if ux_obj does not exist; 1 if it is an existing file or an
   ImageFS image when __feature_imagefs_is_file is 1; 2 if it  an
   existing directory or an ImageFS image when __feature_imagefs_is_file
   is 0.  */
int
__object_exists_ux (const char *ux_obj)
{
  char obj[_POSIX_PATH_MAX];
  int objtype;
  if (!__riscosify_std (ux_obj, 0, obj, sizeof (obj), &objtype))
    return 0;

#if __UNIXLIB_SYMLINKS
  char target[_POSIX_PATH_MAX];
  if (__resolve_symlinks (obj, target, _POSIX_PATH_MAX) != 0)
    return 0;

  return __object_exists_ro (target);
#else
  return __object_exists_ro (obj);
#endif
}

int
__object_exists_ro (const char *ro_obj)
{
  unsigned objtype;
  if (SWI_OS_File_ReadCatInfo (ro_obj, &objtype, NULL, NULL, NULL, NULL) != NULL)
    objtype = 0;
  return objtype;
}
