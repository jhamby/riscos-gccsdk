/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/string/strchr.c,v $
 * $Date: 2005/04/20 17:12:10 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#include <string.h>

char *
strchr (const char *s, int c_)
{
  int i;
  char c = (char) c_;

  while ((i = *s) && (i != c))
    s++;

  if (i == c)
    return ((char *) s);
  else
    return (0);
}
