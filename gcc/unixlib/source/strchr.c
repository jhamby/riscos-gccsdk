/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/strchr,v $
 * $Date: 1998/06/25 22:02:47 $
 * $Revision: 1.8 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strchr,v 1.8 1998/06/25 22:02:47 unixlib Exp $";
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
