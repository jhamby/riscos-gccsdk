/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/memccpy,v $
 * $Date: 1997/10/09 19:59:38 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: memccpy,v 1.4 1997/10/09 19:59:38 unixlib Exp $";
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
