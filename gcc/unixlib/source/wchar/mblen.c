/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/wchar/c/mblen,v $
 * $Date: 1997/10/30 22:22:46 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: mblen,v 1.1 1997/10/30 22:22:46 unixlib Exp $";
#endif

#include <stdlib.h>

int
mblen (const char *s, size_t n)
{
  if (s == NULL || *s == '\0')
    return 0;
  if (n == 0)
    return -1;
  return 1;
}
