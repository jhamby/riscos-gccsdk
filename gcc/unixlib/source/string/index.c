/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/string/index.c,v $
 * $Date: 2005/04/20 17:12:10 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#include <string.h>
#include <strings.h>

char *
index (const char *s, int c)
{
  return strchr (s, c);
}

char *
rindex (const char *s, int c)
{
  return strrchr (s, c);
}
