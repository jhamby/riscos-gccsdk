/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/execv.c,v $
 * $Date: 2000/07/15 14:52:33 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: execv.c,v 1.1.1.1 2000/07/15 14:52:33 nick Exp $";
#endif

#include <unistd.h>

/* Execute path with arguments argv and environment from 'environ'.  */
int
execv (const char *path, char **argv)
{
  return execve (path, argv, environ);
}
