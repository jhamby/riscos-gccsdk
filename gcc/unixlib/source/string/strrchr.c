/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/string/strrchr.c,v $
 * $Date: 2005/04/20 17:12:10 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#include <string.h>

char *
strrchr (const char *s, int c_)
{
  int i;
  const char *_s;
  char c = (char) c_;

  _s = 0;
  do
    if ((i = *s) == c)
      _s = s;
  while (s++, i);

  return ((char *) _s);
}
