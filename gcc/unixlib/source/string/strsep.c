/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/string/strsep.c,v $
 * $Date: 2005/04/20 17:12:10 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#include <string.h>

char *
strsep (char **s, const char *delim)
{
  char *begin, *end;

  begin = *s;
  if (! begin)
    return NULL;

  end = strpbrk (begin, delim);
  if (end)
    {
      *end++ = '\0';
      *s = end;
    }
  else
    *s = NULL;

  return begin;
}
