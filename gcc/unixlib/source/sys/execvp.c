/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/sys/c/execvp,v $
 * $Date: 2000/08/17 16:16:06 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: execvp,v 1.2 2000/08/17 16:16:06 admin Exp $";
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
