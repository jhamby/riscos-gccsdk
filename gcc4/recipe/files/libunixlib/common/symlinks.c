/* symlinks.c
 *
 * Copyright 2007-2009 UnixLib Developers
 *
 * Given a filename, determine if it is a symlink file. If so extract the
 * target file and check again. Repeat until the target filename is not a
 * symlink file, at which point return as the result.
 */

#include <kernel.h>
#include <swis.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/param.h>

#include <internal/os.h>
#include <internal/symlinks.h>

#if __UNIXLIB_SYMLINKS

static int
read_link_file_info (const char *filename, int *filetype, int *type)
{
  _kernel_swi_regs regs;

  regs.r[0] = 23;
  regs.r[1] = (int) filename;
  if (_kernel_swi (XOS_Bit | OS_File, &regs, &regs) != NULL)
    {
      *filetype = 0;
      *type = 0;
      return __set_errno (ENOENT);
    }

  *filetype = regs.r[6];
  *type = regs.r[0];

  return 0;
}

/* Filenames passed and the symlinks itself are always RISC OS filenames.  */
int
__resolve_symlinks (const char *filename_in, char *filename_out,
		    size_t fn_out_size)
{
  static char *buffer = NULL; /* We're reusing our buffer.  */
  static size_t bufsize = 0;
  int link_count = 0;
  int fd = -1;
  int regs[10];

  if (strlen (filename_in) + 1 > fn_out_size)
    return __set_errno (ENAMETOOLONG);

  strcpy (filename_out, filename_in);

  while (1)
    {
      int type, filetype;
      size_t size;
      char *sep;
      char id[4];

      if (read_link_file_info (filename_out, &filetype, &type) != 0)
        break;

      if (!type || filetype != SYMLINK_FILETYPE)
	return 0;

      if (link_count >= SYMLINK_MAX_LINKS)
	{
	  __set_errno (EMLINK);	/* or ELOOP? */
	  break;
	}

      if (__os_fopen (0x4F, filename_out, &fd) != NULL)
	{
	  __set_errno (ENOENT);
	  break;
	}

      if (__os_fread (fd, id, 4, regs) != NULL
	  || *(unsigned int *)id != SYMLINK_ID
	  || __os_fread (fd, &size, 4, regs) != NULL)
	{
	  __set_errno (EIO);
	  break;
	}

      if (size >= bufsize)
        {
	  /* Our buffer is too small.  */
          if (size >= fn_out_size)
	    {
	      /* It doesn't make sense to continue as the user supplied
		 buffer is already too small for this destination link.  */
	      __set_errno (ENAMETOOLONG);
	      break;
	    }
	  free (buffer);
	  bufsize = (size < MAXPATHLEN) ? MAXPATHLEN : size + 1;
	  if ((buffer = malloc (bufsize)) == NULL)
	    {
	      bufsize = 0;
	      __set_errno (ENOMEM);
	      break;
	    }
	}

      if (__os_fread (fd, buffer, size, regs) != NULL)
	{
	  __set_errno (EIO);
	  break;
	}

      __os_fclose (fd);
      fd = -1;

      buffer[size] = '\0';

      /* Cases to consider:
	   foo + ADFS::doh.$.bar => ADFS::doh.$.bar (case 1)
	   foo + gcc:bar         => gcc:bar (case 1)
	   foo + bar             => bar (case 1)
	   foo1.foo2 + bar       => foo1.bar (case 2)
	   gcc:foo + bar         => gcc:bar (case 2)
	 Note that "ADFS::doh.$ + bar => ADFS::doh.$.bar" (which would be an
	 exception on case 2) can not happen as the root can not be a link
	 file.  */
      if (strchr (buffer, ':') != NULL
          || (sep = strrchr (filename_out, '.')) == NULL
	     && (sep = strrchr (filename_out, ':')) == NULL)
	{
	  /* symlink contains absolute file path
	     or the symlink contains a relative file path but filename_out
	     only contains leafname or xyz: (= xyz$path).  */
	  if (size + 1 > fn_out_size)
	    {
	      __set_errno (ENAMETOOLONG);
	      break;
	    }

	  memcpy (filename_out, buffer, size + 1);
	}
      else
	{
	  /* symlink contains relative file path and we have to concat with
	     filename_out after stripping its leafname.  */
	  if ((sep - filename_out) + size + 2 > fn_out_size)
	    {
	      __set_errno (ENAMETOOLONG);
	      break;
	    }

	  memcpy (sep + 1, buffer, size + 1);
	}

      link_count++;
    }

  /* Error happened.  */
  if (fd != -1)
    __os_fclose (fd);

  return -1;
}

#endif
