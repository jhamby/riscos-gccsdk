/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/strrchr,v $
 * $Date: 1998/06/25 22:02:47 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strrchr,v 1.3 1998/06/25 22:02:47 unixlib Exp $";
#endif

#include <string.h>

char *
strrchr (register const char *s, int c_)
{
  register int i;
  register const char *_s;
  register char c = (char) c_;

  _s = 0;
  do
    if ((i = *s) == c)
      _s = s;
  while (s++, i);

  return ((char *) _s);
}
