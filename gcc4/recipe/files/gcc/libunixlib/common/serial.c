/* __get_file_ino ()
 * Copyright (c) 2000-2012 UnixLib Developers
 */

#include <stddef.h>
#include <string.h>
#include <limits.h>

#include <unixlib/local.h>
#include <unixlib/types.h>
#include <internal/os.h>

/* Generate a file serial number. This should distinguish the file from all
   other files on the same device.  DIRECTORY is either NULL and FILENAME
   is a RISC OS format name, or DIRECTORY is a canonicalised RISC OS format
   path and FILENAME is an Unix filename which needs converting with
   __riscosify[_std] () first before it will concatenated with DIRECTORY.  */

__ino_t
__get_file_ino (const char *directory, const char *filename)
{
  char tmp[_POSIX_PATH_MAX + _POSIX_NAME_MAX];
  if (directory != NULL)
    {
      /* Concat 'directory' and RISC OS version of 'filename' together.  */
      char *dst = tmp;
      const char * const name_end = &tmp[sizeof (tmp)-1];
      while (*directory && dst != name_end)
	*dst++ = *directory++;
      if (*directory)
	return (__ino_t) -1;
      /* Check for '.' and '..' as Unix filenames.  */
      if (filename[0] == '.' && filename[1] == '\0')
	{
	  /* Nothing to append.  */
	  *dst = '\0';
	}
      else if (filename[0] == '.' && filename[1] == '.' && filename[2] == '\0')
	{
	  /* Go one directory up but take care when 'directory' is already a
	     RISC OS root directory.  */
	  if (dst == tmp)
	    *dst = '\0';
	  else
	    {
	      char *prev_dir;
	      for (prev_dir = dst - 1;
		   prev_dir != tmp && *prev_dir != '.' && *prev_dir != '$' && *prev_dir != ':';
		   --prev_dir)
		/* */;
	      if (*prev_dir == '.')
		*prev_dir = '\0';
	      else
		*dst = '\0';
	    }
	}
      else
	{
	  /* Just append 'directory' with the RISC OS version of 'filename'.
	     As 'directory' is already canonicalised, the append result is also
	     be canonicalised.  */
	  *dst++ = '.';

	  /* Convert Unix filename to RISC OS filename and append.  */
	  int filetype;
	  if (! __riscosify_std (filename, 0, dst, name_end - dst, &filetype))
	    return (__ino_t) -1;
	}
    }
  else
    {
      if (SWI_OS_FSControl_Canonicalise (filename, NULL, tmp, sizeof (tmp),
					 NULL) != NULL)
	return (__ino_t) -1;
    }

  unsigned int ino = 0;
  for (const char *name = tmp; *name; ++name)
    {
      ino = (ino << 4) + *name;
      unsigned int hash = ino & 0xf0000000U;
      if (hash)
	{
	  ino = ino ^ (hash >> 24);
	  ino = ino ^ hash;
	}
    }
  return (__ino_t) ino;
}
