/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/wchar/c/mbtowc,v $
 * $Date: 1997/10/30 22:22:46 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: mbtowc,v 1.1 1997/10/30 22:22:46 unixlib Exp $";
#endif

#include <stdlib.h>

int
mbtowc (wchar_t *pwc, const char *s, size_t n)
{
  if (s == NULL)
    return 0;
  if (n == 0)
    return -1;
  if (pwc)
    *pwc = (wchar_t) * s;
  return (*s != '\0');
}
