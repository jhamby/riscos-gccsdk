/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/unix/c/putenv,v $
 * $Date: 1997/10/09 20:00:52 $
 * $Revision: 1.8 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: putenv,v 1.8 1997/10/09 20:00:52 unixlib Exp $";
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
