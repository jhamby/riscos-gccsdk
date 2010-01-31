/* memchr ()
 * Copyright (c) 2000-2007 UnixLib Developers
 */

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
ret:
	  return (void *) (--_s);
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

  return NULL;
}
