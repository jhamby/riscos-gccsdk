/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/memchr,v $
 * $Date: 1998/06/25 22:02:47 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: memchr,v 1.5 1998/06/25 22:02:47 unixlib Exp $";
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
