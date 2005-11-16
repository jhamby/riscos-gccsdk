/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/string/strncmp.c,v $
 * $Date: 2005/04/20 17:12:10 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#include <string.h>

int
strncmp (const char *s1, const char *s2, size_t n)
{
  int i;

  if (!n)
    return 0;

  while ((i = *s1) && i == *s2 && --n)
    s1++, s2++;

  return (*(const unsigned char *) s1 - *(const unsigned char *) s2);
}
