/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/string/strrichr.c,v $
 * $Date: 2005/04/20 17:12:10 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#include <string.h>
#include <ctype.h>

char *
strrichr (const char *s, int c)
{
  int i;
  const char *_s;

  c = tolower (c);

  _s = 0;
  do
    {
      i = *s;
      i = tolower (i);
      if (i == c)
	_s = s;
    }
  while (s++, i);

  return ((char *) _s);
}
