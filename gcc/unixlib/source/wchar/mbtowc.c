/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/wchar/mbtowc.c,v $
 * $Date: 2000/07/15 14:52:46 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: mbtowc.c,v 1.1.1.1 2000/07/15 14:52:46 nick Exp $";
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
