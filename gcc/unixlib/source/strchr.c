/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/strchr.c,v $
 * $Date: 2000/07/15 14:52:09 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strchr.c,v 1.1.1.1 2000/07/15 14:52:09 nick Exp $";
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
