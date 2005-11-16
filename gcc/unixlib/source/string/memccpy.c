/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/string/memccpy.c,v $
 * $Date: 2005/04/20 17:12:10 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#include <stddef.h>
#include <string.h>

void *
memccpy (void *dest, const void *src, int c, size_t n)
{
  const char *s = src;
  char *d = dest;

  while (n-- > 0)
    if ((*d++ = *s++) == c)
      return d;

  return NULL;
}
