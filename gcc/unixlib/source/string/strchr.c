/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/strchr.c,v $
 * $Date: 2001/01/29 15:10:18 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strchr.c,v 1.2 2001/01/29 15:10:18 admin Exp $";
#endif

#include <string.h>

char *
strchr (register const char *s, int c_)
{
  register int i;
  register char c = (char) c_;

  while ((i = *s) && (i != c))
    s++;

  if (i == c)
    return ((char *) s);
  else
    return (0);
}
