/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/perror.c,v $
 * $Date: 2000/07/15 14:52:31 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: perror.c,v 1.1.1.1 2000/07/15 14:52:31 nick Exp $";
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
