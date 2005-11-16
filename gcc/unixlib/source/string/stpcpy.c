/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/string/stpcpy.c,v $
 * $Date: 2005/04/20 17:12:10 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#include <string.h>

char *
stpcpy (char *s, const char *s2)
{
  while ((*s++ = *s2++))
    ;

  return s - 1;
}

