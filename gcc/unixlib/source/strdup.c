/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/strdup,v $
 * $Date: 1997/10/09 19:59:41 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strdup,v 1.5 1997/10/09 19:59:41 unixlib Exp $";
#endif

#include <stdlib.h>
#include <string.h>
#include <sys/unix.h>

char *
strdup (const char *string)
{
  int len;
  char *newstr;

  if (!string)
    return 0;

  len = strlen (string) + 1;
  if ((newstr = malloc (len)) == NULL)
    return NULL;
  return memcpy (newstr, string, len);
}
