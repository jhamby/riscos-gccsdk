/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/index.c,v $
 * $Date: 2000/07/15 14:52:08 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: index.c,v 1.1.1.1 2000/07/15 14:52:08 nick Exp $";
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
