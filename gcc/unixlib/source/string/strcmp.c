/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/string/strcmp.c,v $
 * $Date: 2005/04/20 17:12:10 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#include <string.h>

int
strcmp (const char *s1, const char *s2)

{
  int i;

  while ((i = *s1) && i == *s2)
    s1++, s2++;

  return (*(const unsigned char *) s1 - *(const unsigned char *) s2);
}
