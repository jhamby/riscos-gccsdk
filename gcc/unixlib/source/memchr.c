/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/memchr.c,v $
 * $Date: 2000/07/15 14:52:08 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: memchr.c,v 1.1.1.1 2000/07/15 14:52:08 nick Exp $";
#endif

#include <string.h>

void *
memchr (register const void *s, int c_, register size_t n)

{
  register unsigned char *_s = (unsigned char *) s;
  register char c = (char) c_;

  while (n & 0x07)
    {
      n--;
      if (*_s++ == c)
	{
	ret:return ((void *) (--_s));
	}
    }
  n >>= 3;
  while (n)
    {
      n--;
      if (*_s++ == c)
	goto ret;
      if (*_s++ == c)
	goto ret;
      if (*_s++ == c)
	goto ret;
      if (*_s++ == c)
	goto ret;
      if (*_s++ == c)
	goto ret;
      if (*_s++ == c)
	goto ret;
      if (*_s++ == c)
	goto ret;
      if (*_s++ == c)
	goto ret;
    }

  return (0);
}
