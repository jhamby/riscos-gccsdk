/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/string/memchr.c,v $
 * $Date: 2005/04/20 17:12:10 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#include <string.h>

void *
memchr (const void *s, int c_, size_t n)

{
  unsigned char *_s = (unsigned char *)(int) s;
  char c = (char) c_;

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
