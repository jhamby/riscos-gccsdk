/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/index,v $
 * $Date: 1997/10/09 19:59:38 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: index,v 1.2 1997/10/09 19:59:38 unixlib Exp $";
#endif

#include <string.h>

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
