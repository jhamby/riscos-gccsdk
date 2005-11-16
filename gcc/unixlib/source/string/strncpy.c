/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/string/strncpy.c,v $
 * $Date: 2005/04/20 17:12:10 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#include <string.h>

char *
strncpy (char *s, const char *s2, size_t n)
{
  char *s1 = s;

  while (n--)
    if (!(*s1++ = *s2++))
      {
	while (n--)
	  *s1++ = 0;
	return (s);
      }

  return (s);
}
