/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/common/serial.c,v $
 * $Date: 2001/09/04 16:32:04 $
 * $Revision: 1.2.2.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: serial.c,v 1.2.2.2 2001/09/04 16:32:04 admin Exp $";
#endif

#include <stddef.h>
#include <string.h>
#include <limits.h>
#include <unixlib/local.h>
#include <unixlib/os.h>
#include <swis.h>

/* Generate a file serial number. This should distinguish the file from all
   other files on the same device.  DIRECTORY is either NULL and FILENAME
   is a RISC OS format name, or DIRECTORY is a RISC OS format path and
   FILENAME a is Unix filename which needs converting with __uname.  */

__ino_t
__get_file_ino (const char *directory, const char *filename)
{
  char tmp[_POSIX_PATH_MAX + _POSIX_NAME_MAX];
  char pathname[_POSIX_PATH_MAX + _POSIX_NAME_MAX];
  char *name = tmp;
  const char * const name_end = &tmp[sizeof (tmp) - 1];
  int hash, ino, filetype, regs[10];

  if (directory != NULL)
    {
      /* Concat directory, directory separator and filename and ensure
	 buffer is not overrun.  */
      const char *src = directory;
      while (*src && name <= name_end)
	*name++ = *src++;
      if (filename != NULL)
	{
	  if (name[-1] != ':' && name <= name_end)
	    *name++ = '.';
	  /* Convert Unix filename to RISC OS filename and append.  */
	  src = __riscosify_std (filename, 0, name, sizeof (tmp), &filetype);

	  if (!src)
	    return -1;
	}
      else
        {
	  if (name <= name_end)
	    *name = '\0';
	  else
	    return -1;
        }
      name = tmp;
    }
  else
    name = (char *) filename;	/* It's okay, we won't be modifying name.  */

  /* Need to canonicalise even when canonical dir passed in, as readdir
     generates entries . and .., which map to @ and ^
     Additionally, "foo.bar.^" != "foo" and the traditional Unix getcwd
     function would fall over because of this.
     This way sucessive readdir()s on ../../../.. will get you to root !  */

  regs[0] = 37;
  regs[1] = (int) name;
  regs[2] = (int) pathname;
  regs[3] = regs[4] = 0;
  regs[5] = sizeof (pathname);

  /* Use canonicalised name if possible, otherwise use the original name.  */
  if (! __os_swi (OS_FSControl, regs))
    name = pathname;

  ino = 0;
  while (*name)
    {
      ino = (ino << 4) + *name++;
      hash = ino & 0xf0000000;
      if (hash)
	{
	  ino = ino ^ (hash >> 24);
	  ino = ino ^ hash;
	}
    }
  return (__ino_t) ino;
}
