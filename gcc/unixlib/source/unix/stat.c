/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/stat.c,v $
 * $Date: 2004/09/17 18:39:57 $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Author: peter $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: stat.c,v 1.7 2004/09/17 18:39:57 peter Exp $";
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

/* #define DEBUG */

int
stat (const char *filename, struct stat *buf)
{
  PTHREAD_UNSAFE

#ifdef DEBUG
  __os_print ("stat(): file = ");
  __os_print (filename);
  __os_nl ();
#endif

  /* Perform a special check for devices.  */
  buf->st_dev = __getdevtype (filename);

  /* Perform the device specific open operation.  */
  return __funcall ((*(__dev[buf->st_dev].stat)),(filename, buf));
}

#undef stat

int
stat (const char *filename, struct stat64 *buf)
{
  return stat64 (filename, buf);
}
