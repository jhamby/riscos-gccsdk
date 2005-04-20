/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stricmp.c,v $
 * $Date: 2002/09/24 21:02:36 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: stricmp.c,v 1.3 2002/09/24 21:02:36 admin Exp $";
#endif

#include <string.h>
#include <strings.h>
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
