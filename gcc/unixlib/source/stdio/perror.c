/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/perror.c,v $
 * $Date: 2002/12/15 13:16:55 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

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
