/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/wchar/c/wctomb,v $
 * $Date: 1997/10/30 22:22:46 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: wctomb,v 1.1 1997/10/30 22:22:46 unixlib Exp $";
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
