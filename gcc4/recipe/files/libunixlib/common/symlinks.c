/* symlinks.c
 *
 * Copyright 2007 GCCSDK Developers
 *
 * Given a filename, determine if it is a symlink file. If so extract the
 * target file and check again. Repeat until the target filename is not a
 * symlink file, at which point return as the result.
 *
 */

#include <kernel.h>
#include <stdio.h>
#include <swis.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

/* Offically allocated filetype. */
#define	LINK_FILETYPE		0x1C8

/* Arbitrary limit to prevent cyclical links. */
#define MAX_LINKS		64

/* ASCII representation of 'LINK'. */
#define LINK_ID			0x4B4E494C

static int read_link_file_info(const char *filename, int *filetype, int *type)
{
  _kernel_swi_regs regs;
  int err = 0;

  regs.r[0] = 23;
  regs.r[1] = (int)filename;
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

int __resolve_symlinks(const char *filename_in, char *filename_out, size_t fn_out_size)
{
  int link_count = 0;
  int size, type, filetype;
  char *buffer = NULL;
  FILE *file = NULL;

  if (strlen(filename_in) + 1 > fn_out_size)
    return __set_errno(ENAMETOOLONG);

  if ((buffer = malloc(fn_out_size)) == NULL)
    return __set_errno(ENOMEM);

  strcpy(filename_out, filename_in);

  if (read_link_file_info(filename_out, &filetype, &type) != 0)
    goto error;

  while (type && filetype == LINK_FILETYPE)
    {
      if (link_count >= MAX_LINKS)
	{
	  __set_errno(EMLINK); /* or ELOOP? */
	  goto error;
	}

      if ((file = fopen(filename_out, "r")) == NULL)
	{
	  __set_errno(ENOENT);
	  goto error;
	}

      if (fread(buffer, 4, 1, file) != 1)
	{
	  __set_errno(EIO);
	  goto error;
	}

      if (*((unsigned int *)buffer) != LINK_ID)
	{
	  __set_errno(EIO);
	  goto error;
	}

       if (fread(&size, 4, 1, file) != 1)
	{
	  __set_errno(EIO);
	  goto error;
	}

      if (fread(buffer, 1, size, file) != size)
	{
	  __set_errno(EIO);
	  goto error;
	}

      fclose(file);
      file = NULL;

      buffer[size] = '\0';

      if (strchr(buffer, ':') != 0)
	{
	  /* symlink contains absolute file path */
	  if (strlen(buffer) + 1 > fn_out_size)
	    {
	      __set_errno(ENAMETOOLONG);
	      goto error;
	    }

	  strcpy(filename_out, buffer);
	}
      else
	{
	  /* symlink contains relative file path */
	  char *sep;

	  /* Strip the link leaf name */
	  if ((sep = strrchr(filename_out, '.')) != NULL)
	    *(sep + 1)= '\0';
	  else if ((sep = strrchr(filename_out, ':')) != NULL)
	  *(sep + 1) = '\0';
	  else
	    *filename_out = '\0';

	  if (strlen(filename_out) + strlen(buffer) + 2 > fn_out_size)
	  {
	    __set_errno(ENAMETOOLONG);
	    goto error;
	  }

	  strcat(filename_out, buffer);
        }

      if (read_link_file_info(filename_out, &filetype, &type) != 0)
	goto error;

      link_count++;
    }

  free(buffer);

  return 0;

error:
  if (buffer)
    free(buffer);

  if (file)
    fclose(file);

  return -1;
}
