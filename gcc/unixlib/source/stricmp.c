/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/stricmp,v $
 * $Date: 1997/10/09 19:59:41 $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: stricmp,v 1.7 1997/10/09 19:59:41 unixlib Exp $";
#endif

#include <string.h>
#include <ctype.h>

int
stricmp (register const char *s1, register const char *s2)
{
  register int i, j;

  do
    {
      i = *s1++, j = *s2++;
      i = toupper (i);
      j = toupper (j);
    }
  while (i && i == j);

  return (i - j);
}

int
strcasecmp (const char *s1, const char *s2)
{
  return stricmp (s1, s2);
}
