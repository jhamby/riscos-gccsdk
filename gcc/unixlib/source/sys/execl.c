/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/execl.c,v $
 * $Date: 2000/07/15 14:52:33 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: execl.c,v 1.1.1.1 2000/07/15 14:52:33 nick Exp $";
#endif

#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <sys/param.h>
#include <stdlib.h>

/* Execute path with all arguments after path until a NULL
   pointer and environment from 'environ'.  */

int
execl (const char *path, const char *arg, ...)
{
  const char **argv;
  unsigned int i, n;
  va_list args;

  va_start (args, arg);

  n = 8;
  argv = (const char **) malloc (n * sizeof (const char *));
  if (argv == NULL)
    return __set_errno (E2BIG);
  argv[0] = arg;
  i = 0;
  while (argv[i++] != NULL)
    {
      if (i == n)
        {
          n *= 2;
          argv = (const char **) realloc (argv, n * sizeof (char *));
          if (argv == NULL)
            return __set_errno (E2BIG);
        }
      argv[i] = va_arg (args, const char *);
    }

  va_end (args);
  i = execve (path, (char **) argv, (char **) environ);
  free (argv);
  return i;
}
