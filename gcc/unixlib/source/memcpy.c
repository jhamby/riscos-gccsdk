/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/memcpy,v $
 * $Date: 1997/10/09 19:59:39 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: memcpy,v 1.6 1997/10/09 19:59:39 unixlib Exp $";
#endif

#include <string.h>

void
bcopy (const void *src, void *dest, size_t n)
{
  memcpy (dest, src, n);
}
