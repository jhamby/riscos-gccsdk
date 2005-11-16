/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/execv.c,v $
 * $Date: 2002/09/24 21:02:38 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#include <unistd.h>

/* Execute path with arguments argv and environment from 'environ'.  */
int
execv (const char *path, char *const argv[])
{
  return execve (path, argv, environ);
}
