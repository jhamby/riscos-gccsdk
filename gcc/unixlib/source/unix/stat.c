/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/stat.c,v $
 * $Date: 2004/12/11 14:18:57 $
 * $Revision: 1.8 $
 * $State: Exp $
 * $Author: joty $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: stat.c,v 1.8 2004/12/11 14:18:57 joty Exp $";
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
  return dev_funcall (buf->st_dev, stat, (filename, buf));
}

#undef stat

int
stat (const char *filename, struct stat64 *buf)
{
  return stat64 (filename, buf);
}
