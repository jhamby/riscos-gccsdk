/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/perror.c,v $
 * $Date: 2001/01/29 15:10:21 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: perror.c,v 1.2 2001/01/29 15:10:21 admin Exp $";
#endif

#include <errno.h>
#include <stdio.h>
#include <string.h>

__STDIOLIB__

void
perror (const char *string)
{
  const char *error = strerror (errno);

  if (string)
    fprintf (stderr, "%s: %s\n", string, error);
  else
    fprintf (stderr, "%s\n", error);
}
