/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/execvp.c,v $
 * $Date: 2000/07/15 14:52:33 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: execvp.c,v 1.1.1.1 2000/07/15 14:52:33 nick Exp $";
#endif

#include <unistd.h>

/* #define DEBUG 1 */
#ifdef DEBUG
#include <sys/os.h>
#endif

/* Execute file, searching in the 'path' environment variable if it
   contains no slashes, with arguments argv and environment from
   'environ'.  */
int
execvp (const char *name, char **argv)
{
#ifdef DEBUG
  if (environ != NULL)
    {
      int x = -1;
      while (environ[++x])
        {
          os_print ("--execvp: environ["); os_prdec (x); os_print ("]: ");
          os_print (environ[x]); os_print ("\r\n");
        }
    }
#endif

  /* Path searching has not been implemented.  */
  return execve (name, argv, environ);
}
