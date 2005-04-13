/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/resource/ulimit.c,v $
 * $Date: 2004/06/12 08:59:48 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: peter $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: ulimit.c,v 1.4 2004/06/12 08:59:48 peter Exp $";
#endif

#include <sys/resource.h>
#include <unixlib/unix.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <ulimit.h>

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
         __image_rw_lomem and __image_rw_lomem + current limit on address size.  */
      {
	struct rlimit dsize;
	status = getrlimit (RLIMIT_DATA, &dsize);
	if (status < 0)
	  return -1;

	return ((long int) &__image_rw_lomem) + dsize.rlim_cur;
      }
    case 4:
      return FOPEN_MAX;

    default:
      errno = EINVAL;
      return -1;
    }
}
