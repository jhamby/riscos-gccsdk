/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/sys/c/execv,v $
 * $Date: 1997/10/09 13:07:33 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: execv,v 1.1 1997/10/09 13:07:33 unixlib Exp $";
#endif

#include <unistd.h>

/* Execute path with arguments argv and environment from 'environ'.  */
int
execv (const char *path, char **argv)
{
  return execve (path, argv, environ);
}
