/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/memchr.c,v $
 * $Date: 2001/01/29 15:10:18 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: memchr.c,v 1.2 2001/01/29 15:10:18 admin Exp $";
#endif

#include <string.h>

void *
memchr (register const void *s, int c_, register size_t n)

{
  register unsigned char *_s = (unsigned char *)(int) s;
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
