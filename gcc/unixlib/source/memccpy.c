/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/memccpy.c,v $
 * $Date: 2000/07/15 14:52:08 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: memccpy.c,v 1.1.1.1 2000/07/15 14:52:08 nick Exp $";
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
