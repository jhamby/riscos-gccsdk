/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/index.c,v $
 * $Date: 2001/01/29 15:10:18 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: index.c,v 1.2 2001/01/29 15:10:18 admin Exp $";
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
