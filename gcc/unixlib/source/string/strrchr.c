/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/strrchr.c,v $
 * $Date: 2001/01/29 15:10:19 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strrchr.c,v 1.2 2001/01/29 15:10:19 admin Exp $";
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
