/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/utime.c,v $
 * $Date: 2001/09/04 16:32:04 $
 * $Revision: 1.2.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: utime.c,v 1.2.2.1 2001/09/04 16:32:04 admin Exp $";
#endif

#include <errno.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>
#include <utime.h>

#include <unixlib/os.h>
#include <unixlib/local.h>
#include <unixlib/swiparams.h>

int
utime (const char *ux_filename, const struct utimbuf *times)
{
  time_t unix_time;
  unsigned int low, high;
  _kernel_oserror *err;
  int regs[10], sftype, aftype;
  char filename[_POSIX_PATH_MAX];

  if (ux_filename == NULL)
    return __set_errno (EINVAL);

  if (! __riscosify_std (ux_filename, 0, filename, sizeof (filename),
     	  	    	 &sftype))
    return __set_errno (ENAMETOOLONG);

  /* See if the file exists first and get file type and attributes.  */
  err = __os_file (OSFILE_READCATINFO_NOPATH, filename, regs);
  if (err)
    {
      __seterr (err);
      return -1;
    }

  /* Does the file have a filetype ? At this point we aren't even sure
     that the file exists but that is not a problem.  */
  if ((regs[2] & 0xfff00000U) == 0xfff00000U)
    aftype = (regs[2] >> 8) & 0xfff;
  else
    aftype = __RISCOSIFY_FILETYPE_NOTFOUND;

  /* Fail if the file doesn't exist or (if specified) the filetype is
     different.  */
  if (regs[0] == 0
      || (sftype != __RISCOSIFY_FILETYPE_NOTFOUND && sftype != aftype))
    return __set_errno (ENOENT);

  /* If times is null then get the current time.  */
  unix_time = times ? times->modtime : time (NULL);

  /* Convert time to RISC OS and set the file's time.  */
  __cvt_unix_time (unix_time, &high, &low);
  regs[2] = (regs[2] & 0x000fff00U) | 0xfff00000U | high;
  regs[3] = (int) low;
  /* Object attributes (regs[5]) come from __os_file above.  */
  err = __os_file (1, filename, regs);
  if (err)
    {
      __seterr (err);
      return -1;
    }

  return 0;
}
