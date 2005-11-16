/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/string/strncat.c,v $
 * $Date: 2005/04/20 17:12:10 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#include <string.h>

char *
strncat (char *s, const char *s2, size_t n)
{
  char *s1 = s;

  while (*s1++)
    ;
  --s1;

  while ((*s1++ = *s2++))
    if (!(n--))
      {
	*--s1 = 0;
	break;
      }

  return s;
}
