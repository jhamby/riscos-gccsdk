/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/memcpy.c,v $
 * $Date: 2000/07/15 14:52:08 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: memcpy.c,v 1.1.1.1 2000/07/15 14:52:08 nick Exp $";
#endif

#include <string.h>

void
bcopy (const void *src, void *dest, size_t n)
{
  memcpy (dest, src, n);
}
