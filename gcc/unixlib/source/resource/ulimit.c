/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/resource/c/ulimit,v $
 * $Date: 1997/10/09 20:00:27 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: ulimit,v 1.5 1997/10/09 20:00:27 unixlib Exp $";
#endif

#include <sys/resource.h>
#include <sys/syslib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

/* Function depends on CMD:
   1 = Return the limit on the size of a file, in units of 512 bytes.
   2 = Set the limit on the size of a file to NEWLIMIT.  Only the
   super-user can increase the limit.
   3 = Return the maximum possible address of the data segment.
   4 = Return the maximum number of files that the calling process
   can open.
   Returns -1 on errors.  */
long int
ulimit (int cmd, int newlimit)
{
  int status;

  switch (cmd)
    {
    case 1:
      {
	/* Get limit on file size.  */
	struct rlimit fsize;

	status = getrlimit (RLIMIT_FSIZE, &fsize);
	if (status < 0)
	  return -1;

	/* Convert from bytes to 512 byte units.  */
	return ((long int) fsize.rlim_cur) / 512;
      }
    case 2:
      /* Set limit on file size.  */
      {
	struct rlimit fsize;
	fsize.rlim_cur = (int) newlimit *512;
	fsize.rlim_max = (int) newlimit *512;

	return setrlimit (RLIMIT_FSIZE, &fsize);
      }
    case 3:
      /* Get maximum address for `brk'.  This lies between
         __lomem and __lomem + current limit on address size.  */
      {
	struct rlimit dsize;
	status = getrlimit (RLIMIT_DATA, &dsize);
	if (status < 0)
	  return -1;

	return ((long int) &__lomem) + dsize.rlim_cur;
      }
    case 4:
      return FOPEN_MAX;

    default:
      errno = EINVAL;
      return -1;
    }
}
