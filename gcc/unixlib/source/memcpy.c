/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/memcpy.c,v $
 * $Date: 2001/01/29 15:10:18 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: memcpy.c,v 1.2 2001/01/29 15:10:18 admin Exp $";
#endif

#include <string.h>
#include <strings.h>

void
bcopy (const void *src, void *dest, size_t n)
{
  memcpy (dest, src, n);
}
