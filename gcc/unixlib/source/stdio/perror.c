/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/stdio/c/perror,v $
 * $Date: 1998/11/02 21:05:51 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: perror,v 1.5 1998/11/02 21:05:51 unixlib Exp $";
#endif

#include <errno.h>
#include <stdio.h>

__STDIOLIB__

void
perror (const char *string)
{
  const char *error = (errno < sys_nerr) ? sys_errlist[errno]
					 : "Unknown Error";
  if (string)
    fprintf (stderr, "%s: %s\n", string, error);
  else
    fprintf (stderr, "%s\n", error);
}
