/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/strnicmp,v $
 * $Date: 1997/10/09 19:59:42 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strnicmp,v 1.2 1997/10/09 19:59:42 unixlib Exp $";
#endif

#include <string.h>
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
