/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/string/strcat.c,v $
 * $Date: 2005/04/20 17:12:10 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#include <string.h>

char *
strcat (char *s, const char *s2)
{
  char *s1 = s;

  while (*s1++)
    ;
  --s1;

  while ((*s1++ = *s2++))
    ;

  return s;
}
