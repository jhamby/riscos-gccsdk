/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/unix/c/getcwd,v $
 * $Date: 2000/08/17 16:16:07 $
 * $Revision: 1.12 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getcwd,v 1.12 2000/08/17 16:16:07 admin Exp $";
#endif

#include <errno.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/os.h>
#include <sys/syslib.h>
#include <sys/swis.h>
#include <sys/unix.h>

#include <unixlib/local.h>

/* Return current working directory in Unix format ('.' and '/' swapped).  */
char *
getcwd (char *buffer, size_t size)
{
  char ro_path[_POSIX_PATH_MAX];
  char temp_buf[_POSIX_PATH_MAX];
  char *rp = &ro_path[sizeof (ro_path) - 1];
  _kernel_oserror *err;
  int regs[10];

  if (size == 0 && buffer)
    {
      (void) __set_errno (EINVAL);
      return NULL;
    }

  /* Try the DDE's prefix directory first, so that the program behaves as
     expected in the presence of a prefix directory. Does this affect
     !Virtual given the comment below for the CSD variable method ?
     I would hope not, given that all other SWIs on file operations
     get affected by any prefix directory.  -- PJB.  */

  if (__getenv_from_os ("Prefix$Dir", ro_path, sizeof (ro_path)) != NULL
      && *ro_path != '\0')
    return __unixify_std (ro_path, buffer, size,
    	     	     	  __RISCOSIFY_FILETYPE_NOTSPECIFIED);

  /* Try to read CSD variable - added by Steffan Corley for !Virtual
     compatibility.  */

  if (__getenv_from_os ("FileSwitch$CurrentFilingSystem",
			ro_path, sizeof (ro_path)) != NULL)
    {
      int len;

      stpcpy (stpcpy (stpcpy (temp_buf, "FileSwitch$"), ro_path), "$CSD");
      len = strlen (ro_path);
      ro_path[len++] = ':';
      ro_path[len] = '\0';
      if (__getenv_from_os (temp_buf, &ro_path[len],
			    sizeof (ro_path) - len) != NULL)
	/* If read CSD var worked then parse the path name */
	return __unixify_std (ro_path, buffer, size,
	  	   	      __RISCOSIFY_FILETYPE_NOTSPECIFIED);
    }

  /* Why don't we try OS_FSControl,37,"@" to get CSD ?  -- PJB */

  /* Try OS_Args 7 (get cannonicalised name) on "@" by opening CSD.  */
  {
    int handle;
    if (os_fopen (0x40, "@", &handle) == NULL)
      {
	if (__fd_to_name (handle, ro_path, sizeof (ro_path)) != NULL)
	  {
	    os_fclose (handle);
	    return __unixify_std (ro_path, buffer, size,
	    	     	     	  __RISCOSIFY_FILETYPE_NOTSPECIFIED);
	  }
	os_fclose (handle);
      }
  }

  /* Search backwards up the tree.  */
  *rp = '\0';
  while (1)
    {
      /* Read current directory name.  */
      regs[0] = 6;
      regs[2] = (int) temp_buf;
      err = os_swi (OS_GBPB, regs);
      if (err)
	{
	  /* Failed to read dirname.
	     So swap CSD & PSD then chdir back to original directory.  */
	  __seterr (err);
	  if (*rp != '\0')
	    {
	      if (os_fsctrl (0, "\\", NULL, 0) == NULL)
		{
		  /* cd \ successful, so now need to cd \.{rp}  */
		  *--rp = '.';
		  *--rp = '\\';
		}
	      os_fsctrl (0, rp, NULL, 0);
	    }
	  return NULL;
	}

      /* If at root directory then exit loop.  */
      if ((temp_buf[2] == '$') && (temp_buf[1] == 1))
	break;

      /* Copy the directory name into the RISC OS name buffer.  */
      rp -= temp_buf[1];
      if (rp - ro_path < 1)
	goto range_fail;
      memcpy (rp, temp_buf + 2, temp_buf[1]);
      *--rp = '.';

      /* Now select PSD.  */
      err = os_fsctrl (0, "\\", NULL, 0);
      if (err)
	{
	  /* If cd \ failed then just cd ^.  */
	  err = os_fsctrl (0, "^", NULL, 0);
	  if (err)
	    goto swi_fail;
	}
      else
	{
	  /* Select new PSD.^
	     This ensures that the PSD doesn't get corrupted by
	     calling getcwd().  */
	  err = os_fsctrl (0, "\\.^", NULL, 0);
	  if (err)
	    goto swi_fail;
	}
    }

  /* OK, reached the root directory.  */
  *--rp = '$';

  /* Again, PSD then original directory.  */
  err = os_fsctrl (0, "\\", NULL, 0);
  if (err)
    goto swi_fail;

  err = os_fsctrl (0, rp, NULL, 0);
  if (err)
    goto swi_fail;

  /* OK, rp now contains the current directory, and the PSD is unchanged.
     Next find the FS name.  */

  /* Get FS number.  */
  err = os_args (0, 0, 0, regs);
  if (err)
    goto swi_fail;

  /* Convert FS number to name.  */
  err = os_fsctrl (33, (const char *) regs[0], temp_buf, _POSIX_PATH_MAX - 3);
  if (err)
    goto swi_fail;

  {
    int len;

    len = strlen (temp_buf);
    temp_buf[len++] = ':';
    /* Now have "ADFS:", for example, in temp_buf.  */

    /* Next find the disk name.  */
    regs[0] = 5;
    regs[2] = (int) &temp_buf[len];
    err = os_swi (OS_GBPB, regs);
    if (err)
      goto swi_fail;

    temp_buf[len + temp_buf[len] + 1] = '.';
    temp_buf[len + temp_buf[len] + 2] = '\0';
    temp_buf[len] = ':';
    /* Now have "ADFS::Winnie.", for example, in temp_buf.  */

    len = strlen (temp_buf);
    if (ro_path + len > rp)
      goto range_fail;
    /* Move temp_buf before pathname at rp and convert to unix form.  */
    memmove (rp - len, temp_buf, len);
    return __unixify_std (rp - len, buffer, size,
			  __RISCOSIFY_FILETYPE_NOTSPECIFIED);
  }

range_fail:
  (void) __set_errno (ERANGE);
  return NULL;

swi_fail:
  __seterr (err);
  return NULL;
}

/* BSD version of getcwd.  */

char *
getwd (char *buffer)
{
  return getcwd (buffer, _POSIX_PATH_MAX);
}
