/* symlinks.c
 *
 * Copyright 2007-2013 UnixLib Developers
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
#include <internal/swiparams.h>

#if __UNIXLIB_SYMLINKS

static int
read_link_file_info (const char *filename, int *filetype, int *type)
{
  _kernel_swi_regs regs;

  regs.r[0] = 23;
  regs.r[1] = (int) filename;
  _kernel_oserror *err;
  if ((err = _kernel_swi (XOS_Bit | OS_File, &regs, &regs)) != NULL)
    {
      *filetype = 0;
      *type = 0;
      return __ul_seterr (err, ENOENT);
    }

  *filetype = regs.r[6];
  *type = regs.r[0];

  return 0;
}

/* Filenames passed and the symlinks itself are always RISC OS filenames.  */
int
__resolve_symlinks (const char *filename_in, char *filename_out,
		    size_t fn_out_size, int one_step_only)
{
  static char *buffer = NULL; /* We're reusing our buffer.  */
  static size_t bufsize = 0;

  if (strlen (filename_in) + 1 > fn_out_size)
    return __set_errno (ENAMETOOLONG);

  strcpy (filename_out, filename_in);

  unsigned fd = 0;
  int link_count = 0;
  while (1)
    {
      int type, filetype;
      if (read_link_file_info (filename_out, &filetype, &type) != 0)
        break;

      if (!type || filetype != SYMLINK_FILETYPE)
	return 0;

      if (link_count >= SYMLINK_MAX_LINKS)
	{
	  /* Note, EMLINK is FreeBSD'ism while ELOOP is POSIX.  */
	  __set_errno (ELOOP);
	  break;
	}

      const _kernel_oserror *err;
      if ((err = SWI_OS_Find_Open (OSFIND_OPENIN | OSFIND_OPEN_ERRORIFDIR
				     | OSFIND_OPEN_ERRORIFABSENT,
				   filename_out, &fd)) != NULL)
	{
	  __ul_seterr (err, ENOENT);
	  break;
	}

      size_t size;
      char id[4];
      if ((err = SWI_OS_GBPB_ReadBytes (fd, id, 4, NULL)) != NULL
	  || *(unsigned int *)id != SYMLINK_ID
	  || (err = SWI_OS_GBPB_ReadBytes (fd, &size, 4, NULL)) != NULL)
	{
	  __ul_seterr (err, EIO);
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
	      break;
	    }
	}

      if ((err = SWI_OS_GBPB_ReadBytes (fd, buffer, size, NULL)) != NULL)
	{
	  __ul_seterr (err, EIO);
	  break;
	}

      SWI_OS_Find_Close (fd);
      fd = 0;

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
      char *sep;
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

      if (one_step_only)
	return 0;
    }

  /* Error happened.  */
  if (fd != 0)
    SWI_OS_Find_Close (fd);

  return -1;
}

#endif
