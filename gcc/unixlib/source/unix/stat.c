/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/stat.c,v $
 * $Date: 2003/04/05 09:33:57 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: stat.c,v 1.5 2003/04/05 09:33:57 alex Exp $";
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

// #define DEBUG

int
stat (const char *filename, struct stat *buf)
{
  PTHREAD_UNSAFE

#ifdef DEBUG
  __os_print ("stat(): file = ");
  __os_print (filename);
  __os_print ("\r\n");
#endif

  /* Perform a special check for devices.  */
  buf->st_dev = __getdevtype (filename);

  /* Perform the device specific open operation.  */
  return __funcall ((*(__dev[buf->st_dev].stat)),(filename, buf));
}
