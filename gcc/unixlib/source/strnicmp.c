/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/strnicmp.c,v $
 * $Date: 2001/01/29 15:10:19 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strnicmp.c,v 1.2 2001/01/29 15:10:19 admin Exp $";
#endif

#include <string.h>
#include <strings.h>
#include <ctype.h>

int
strnicmp (const char *s1, const char *s2, size_t n)
{
  int i, j;

  if (!n)
    return (0);

  do
    {
      i = *s1++, j = *s2++;
      i = toupper (i);
      j = toupper (j);
    }
  while (i && i == j && --n);

  return (i - j);
}

int
strncasecmp (const char *s1, const char *s2, size_t n)
{
  return strnicmp (s1, s2, n);
}
