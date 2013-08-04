/* realpath ()
 * Copyright 2002-2013 UnixLib Developers
 */

#include <errno.h>
#include <stdlib.h>
#include <limits.h>

#include <internal/os.h>
#include <internal/symlinks.h>

#ifndef __TARGET_SCL__
#include <unixlib/local.h>

char *
realpath (const char *file_name, char *resolved_name)
{
  if (file_name == NULL)
    {
      __set_errno (EINVAL);
      return NULL;
    }

  char *out_resolved_name;
  if (resolved_name == NULL)
    {
      if ((out_resolved_name = malloc (PATH_MAX)) == NULL)
	return NULL;
    }
  else
    out_resolved_name = resolved_name;

  int filetype;
  if (__riscosify_std (file_name, 0, out_resolved_name, PATH_MAX, &filetype) == NULL)
    {
      __set_errno (ENAMETOOLONG);
      goto error;
    }

  const char *ro_filename;
# if __UNIXLIB_SYMLINKS
  char target[PATH_MAX];
  if (__resolve_symlinks (out_resolved_name, target, PATH_MAX, 0) != 0)
    goto error;

  ro_filename = target;
# else
  ro_filename = out_resolved_name;
# endif

  char buffer[PATH_MAX];
  unsigned objtype;
  const _kernel_oserror *err;
  if ((err = SWI_OS_FSControl_Canonicalise (ro_filename, NULL,
					    buffer, sizeof (buffer),
					    NULL)) != NULL
      || (err = SWI_OS_File_ReadCatInfo (buffer, &objtype, NULL, NULL, NULL,
					 NULL)) != NULL
      || objtype == 0)
    {
      if (err != NULL)
	__ul_seterr (err, ENOENT);
      else
	__set_errno (ENOENT);
      goto error;
    }

  if (__unixify_std (buffer, out_resolved_name, PATH_MAX, filetype) == NULL)
    {
      __set_errno (ENAMETOOLONG);
      goto error;
    }

  return out_resolved_name;

error:
  if (resolved_name == NULL)
    free (out_resolved_name);
  return NULL;
}

#else

char *
realpath (const char *file_name, char *resolved_name)
{
  if (file_name == NULL)
    {
      __set_errno (EINVAL);
      return NULL;
    }

  char *out_resolved_name;
  size_t out_size;
  if (resolved_name == NULL)
    {
      SWI_OS_FSControl_Canonicalise (file_name, NULL, NULL, 0, &out_size);
      if ((out_resolved_name = malloc (out_size)) == NULL)
	return NULL;
    }
  else
    {
      out_resolved_name = resolved_name;
      out_size = PATH_MAX;
    }

  unsigned objtype;
  const _kernel_oserror *err;
  if ((err = SWI_OS_FSControl_Canonicalise (file_name, NULL,
					    out_resolved_name, out_size,
					    NULL)) != NULL
      || (err = SWI_OS_File_ReadCatInfo (out_resolved_name, &objtype, NULL,
					 NULL, NULL, NULL)) != NULL
      || objtype == 0)
    {
      if (err != NULL)
	__ul_seterr (err, ENOENT);
      else
	__set_errno (ENOENT);
      if (resolved_name == NULL)
	free (out_resolved_name);
      return NULL;
    }

  return out_resolved_name;
}
#endif
