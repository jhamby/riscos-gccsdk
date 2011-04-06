/* __get_file_ino ()
 * Copyright (c) 2000-2011 UnixLib Developers
 */

#include <stddef.h>
#include <string.h>
#include <limits.h>

#include <unixlib/local.h>
#include <unixlib/types.h>
#include <internal/os.h>

/* Generate a file serial number. This should distinguish the file from all
   other files on the same device.  DIRECTORY is either NULL and FILENAME
   is a RISC OS format name, or DIRECTORY is a RISC OS format path and
   FILENAME a is Unix filename which needs converting with
   __riscosify[_std] ().  */

__ino_t
__get_file_ino (const char *directory, const char *filename)
{
  char tmp[_POSIX_PATH_MAX + _POSIX_NAME_MAX];
  const char *name;
  if (directory != NULL)
    {
      /* Concat directory, directory separator and filename and ensure
	 buffer is not overrun.  */
      char *dst = tmp;
      const char * const name_end = &tmp[sizeof (tmp) - 1];

      while (*directory && dst <= name_end)
	*dst++ = *directory++;
      if (filename != NULL)
	{
	  if (dst > tmp && dst[-1] != ':' && dst <= name_end)
	    *dst++ = '/';
	  /* Convert Unix filename to RISC OS filename and append.  */
	  int filetype;
	  if (! __riscosify_std (filename, 0, dst, name_end - dst, &filetype))
	    return -1;
	}
      else
        {
	  if (dst > name_end)
	    return -1;
	  *dst = '\0';
        }
      name = tmp;
    }
  else
    name = filename;

  /* Need to canonicalise even when canonical dir passed in, as readdir
     generates entries . and .., which map to @ and ^
     Additionally, "foo.bar.^" != "foo" and the traditional Unix getcwd
     function would fall over because of this.
     This way sucessive readdir()s on ../../../.. will get you to root !  */

  /* Use canonicalised name if possible, otherwise use the original name.  */
  char pathname[_POSIX_PATH_MAX + _POSIX_NAME_MAX];
  if (SWI_OS_FSControl_Canonicalise (name, NULL, pathname, sizeof (pathname),
				     NULL) == NULL)
    name = pathname;

  int ino = 0;
  while (*name)
    {
      ino = (ino << 4) + *name++;
      int hash = ino & 0xf0000000;
      if (hash)
	{
	  ino = ino ^ (hash >> 24);
	  ino = ino ^ hash;
	}
    }
  return (__ino_t) ino;
}
