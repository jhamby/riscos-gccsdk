/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/sys/c/execlp,v $
 * $Date: 2000/06/03 14:28:30 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: execlp,v 1.2 2000/06/03 14:28:30 admin Exp $";
#endif

#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <sys/param.h>
#include <stdlib.h>

/* Execute file, searching in the 'path' environment variable if
   it contains no slashes, with all arguments after file until a
   NULL pointer and environment from 'environ'.  */
int
execlp (const char *file, const char *arg,...)
{
  const char **argv;
  unsigned int i, n = 16;
  va_list args;

  va_start (args, arg);
  argv = (const char **) malloc (n * sizeof (const char *));
  if (argv == NULL)
    return __set_errno (E2BIG);
  argv[0] = arg;

  i = 1;
  do
    {
      if (i >= n)
        {
          n *= 2;
          argv = (const char **) realloc (argv, n * sizeof (const char *));
          if (argv == NULL)
            return __set_errno (E2BIG);
        }
      argv[i] = va_arg (args, const char *);
    }
  while (argv[i++] != NULL);

  va_end (args);

  i = execvp (file, (char **) argv);
  free (argv);
  return i;
}
