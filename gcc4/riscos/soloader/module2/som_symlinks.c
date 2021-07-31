/* som_symlinks.c
 *
 * Copyright 2007 GCCSDK Developers
 */

#include <kernel.h>
#include <stdio.h>
#include <swis.h>
#include <string.h>
#include "som_alloc.h"
#include "somanager.h"

#define	LINK_FILETYPE		0x1C8

#define MAX_LINKS		64

#define LINK_ID			0x4B4E494C

#define MAX_FILENAME_LEN	256

/* Reduce stack use by not inlining ? */
static __attribute__ ((noinline))
     _kernel_oserror *read_link_file_info (const char *filename,
					   int *filetype, int *type)
{
  _kernel_swi_regs regs;
  _kernel_oserror *err;

  regs.r[0] = 23;
  regs.r[1] = (int) filename;
  if ((err = _kernel_swi (XOS_Bit | OS_File, &regs, &regs)) == NULL)
    {
      *filetype = regs.r[6];
      *type = regs.r[0];
    }
  else
    {
      *filetype = 0;
      *type = 0;
    }

  return err;
}

_kernel_oserror *
som_resolve_links (const char *file_in, char **filename_ret)
{
  int link_count = 0;
  int size, type, filetype;
  _kernel_oserror *err = NULL;
  char *buffer = NULL, *file_out = NULL;
  FILE *file = NULL;

  if (strlen (file_in) + 1 > MAX_FILENAME_LEN)
    return somerr_symlink_overflow;

  if ((err = som_alloc (MAX_FILENAME_LEN,
			(void **) (void *) &file_out)) != NULL)
    goto error;
  if ((err = som_alloc (MAX_FILENAME_LEN,
			(void **) (void *) &buffer)) != NULL)
    goto error;

  strcpy (file_out, file_in);

  if ((err = read_link_file_info (file_out, &filetype, &type)) != NULL)
    goto error;

  while (type && filetype == LINK_FILETYPE)
    {
      if (link_count >= MAX_LINKS)
	{
	  err = somerr_symlink_overflow;
	  goto error;
	}

      if ((file = fopen (file_out, "r")) == NULL)
	goto error;

      if (fread (buffer, 4, 1, file) != 1)
	goto error;

      if (*((unsigned int *) buffer) != LINK_ID)
	{
	  err = somerr_symlink_bad_ID;
	  goto error;
	}

      if (fread (&size, 4, 1, file) != 1)
	goto error;

      if (fread (buffer, 1, size, file) != size)
	goto error;

      fclose (file);
      file = NULL;

      buffer[size] = '\0';

      if (strchr (buffer, ':') != 0)
	{
	  /* symlink contains absolute file path.  */
	  if (strlen (buffer) + 1 > MAX_FILENAME_LEN)
	    {
	      err = somerr_symlink_overflow;
	      goto error;
	    }

	  strcpy (file_out, buffer);
	}
      else
	{
	  /* symlink contains relative file path.  */
	  char *sep;

	  /* Strip the link leaf name.  */
	  if ((sep = strrchr (file_out, '.')) != NULL)
	    *(sep + 1) = '\0';
	  else if ((sep = strrchr (file_out, ':')) != NULL)
	    *(sep + 1) = '\0';
	  else
	    *file_out = '\0';

	  if (strlen (file_out) + strlen (buffer) + 2 > MAX_FILENAME_LEN)
	    {
	      err = somerr_symlink_overflow;
	      goto error;
	    }

	  strcat (file_out, buffer);
	}

      if ((err = read_link_file_info (file_out, &filetype, &type)) != NULL)
	goto error;

      link_count++;
    }

  *filename_ret = file_out;
  som_free (buffer);

  return NULL;

error:
  if (err == NULL)
    if ((err = _kernel_last_oserror ()) == NULL)
      err = somerr_file_error;

  if (buffer)
    som_free (buffer);
  if (file_out)
    som_free (file_out);

  if (file)
    fclose (file);

  return err;
}
