/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/string/strichr.c,v $
 * $Date: 2005/04/20 17:12:10 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#include <string.h>
#include <ctype.h>

char *
strichr (const char *s, int c)
{
  int i;

  c = tolower (c);

  do
    {
      i = *s;
      i = tolower (i);
      if (i == c)
	return ((char *) s);
    }
  while (*s++);

  return (0);
}
