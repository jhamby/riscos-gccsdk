/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/stat.c,v $
 * $Date: 2003/01/21 17:48:32 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: stat.c,v 1.4 2003/01/21 17:48:32 admin Exp $";
#endif

#include <errno.h>
#include <limits.h>
#include <string.h>

#include <unixlib/dev.h>
#include <unixlib/os.h>
#include <sys/stat.h>

#include <unixlib/local.h>
#include <unixlib/swiparams.h>
#include <unixlib/unix.h>
#include <pthread.h>

int
stat (const char *filename, struct stat *buf)
{
  PTHREAD_UNSAFE

  /* Perform a special check for devices.  */
  buf->st_dev = __getdevtype (filename);

  /* Perform the device specific open operation.  */
  return __funcall ((*(__dev[buf->st_dev].stat)),(filename, buf));
}
