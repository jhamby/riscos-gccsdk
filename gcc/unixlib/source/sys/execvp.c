/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/execvp.c,v $
 * $Date: 2002/09/24 21:02:38 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: execvp.c,v 1.4 2002/09/24 21:02:38 admin Exp $";
#endif

#include <unistd.h>

/* #define DEBUG 1 */
#ifdef DEBUG
#include <unixlib/os.h>
#include <pthread.h>
#endif

/* Execute file, searching in the 'path' environment variable if it
   contains no slashes, with arguments argv and environment from
   'environ'.  */
int
execvp (const char *name, char *const argv[])
{
#ifdef DEBUG
  PTHREAD_UNSAFE

  if (environ != NULL)
    {
      int x = -1;
      while (environ[++x])
        {
          __os_print ("--execvp: environ["); __os_prdec (x); __os_print ("]: ");
          __os_print (environ[x]); __os_print ("\r\n");
        }
    }
#endif

  /* Path searching has not been implemented.  */
  return execve (name, argv, environ);
}
