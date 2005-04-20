/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/memccpy.c,v $
 * $Date: 2001/01/29 15:10:18 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: memccpy.c,v 1.2 2001/01/29 15:10:18 admin Exp $";
#endif
#include <stddef.h>
#include <string.h>

void *
memccpy (void *dest, const void *src, int c, size_t n)
{
  const char *s = src;
  char *d = dest;

  while (n-- > 0)
    if ((*d++ = *s++) == c)
      return d;

  return NULL;
}
