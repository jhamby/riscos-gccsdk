/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/execv.c,v $
 * $Date: 2001/01/29 15:10:21 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: execv.c,v 1.2 2001/01/29 15:10:21 admin Exp $";
#endif

#include <unistd.h>

/* Execute path with arguments argv and environment from 'environ'.  */
int
execv (const char *path, char *const argv[])
{
  return execve (path, argv, environ);
}
