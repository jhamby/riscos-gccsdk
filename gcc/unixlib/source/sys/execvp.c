/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/execvp.c,v $
 * $Date: 2001/09/04 16:32:04 $
 * $Revision: 1.2.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: execvp.c,v 1.2.2.1 2001/09/04 16:32:04 admin Exp $";
#endif

#include <unistd.h>

/* #define DEBUG 1 */
#ifdef DEBUG
#include <unixlib/os.h>
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
          __os_print ("--execvp: environ["); __os_prdec (x); __os_print ("]: ");
          __os_print (environ[x]); __os_print ("\r\n");
        }
    }
#endif

  /* Path searching has not been implemented.  */
  return execve (name, argv, environ);
}
