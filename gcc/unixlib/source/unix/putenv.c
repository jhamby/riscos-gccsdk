/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/putenv.c,v $
 * $Date: 2000/07/15 14:52:44 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: putenv.c,v 1.1.1.1 2000/07/15 14:52:44 nick Exp $";
#endif

#include <stdlib.h>
#include <string.h>
#include <sys/unix.h>

/* Put STRING, which is of the form "NAME=VALUE", in the environment.
   If there is no '=' in STRING, then remove it from the environment.  */
int
putenv (const char *string)
{
  const char * const equal = strchr (string, '=');
  char *name;
  int rval;

  if (equal)
    {
      if ((name = malloc (equal - string + 1)) == NULL)
	return 1;
      memcpy (name, string, equal - string);
      name [equal - string] = '\0';
      rval = setenv (name, equal + 1, 1);
      free (name);
      return rval;
    }
  else
    {
      unsetenv (string);
      return 0;
    }
}
