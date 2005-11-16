/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/string/strstr.c,v $
 * $Date: 2005/04/20 17:12:10 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#include <string.h>

char *
strstr (const char *s1, const char *s2)

{
  size_t l1 = strlen (s1), l2 = strlen (s2);
  const char *e1 = s1 + l1 - l2;

  while (s1 <= e1)
    {
      if (!strncmp (s1, s2, l2))
	return ((char *) s1);
      s1++;
    }

  return (0);
}
