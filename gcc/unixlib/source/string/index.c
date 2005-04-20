/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/index.c,v $
 * $Date: 2002/09/24 21:02:36 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: index.c,v 1.3 2002/09/24 21:02:36 admin Exp $";
#endif

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
