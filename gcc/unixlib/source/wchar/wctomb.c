/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/wchar/wctomb.c,v $
 * $Date: 2000/07/15 14:52:48 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: wctomb.c,v 1.1.1.1 2000/07/15 14:52:48 nick Exp $";
#endif

#include <stdlib.h>

int
wctomb (char *s, wchar_t wchar)
{
  if (s == NULL)
    return 0;

  *s = (char) wchar;
  return 1;
}
