/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/memcmp.c,v $
 * $Date: 2000/07/15 14:52:08 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: memcmp.c,v 1.1.1.1 2000/07/15 14:52:08 nick Exp $";
#endif

#include <string.h>

int
memcmp (const void *s1, const void *s2, size_t n)
{
  register unsigned char *_s1 = (unsigned char *) s1, *_s2 = (unsigned char *) s2;

  while (n & 0x07)
    {
      if (*_s1 != *_s2)
	goto differs;
      _s1++, _s2++;
      n--;
    }
  n >>= 3;
  while (n)
    {
      if (*_s1 != *_s2)
	goto differs;
      _s1++, _s2++;
      if (*_s1 != *_s2)
	goto differs;
      _s1++, _s2++;
      if (*_s1 != *_s2)
	goto differs;
      _s1++, _s2++;
      if (*_s1 != *_s2)
	goto differs;
      _s1++, _s2++;
      if (*_s1 != *_s2)
	goto differs;
      _s1++, _s2++;
      if (*_s1 != *_s2)
	goto differs;
      _s1++, _s2++;
      if (*_s1 != *_s2)
	goto differs;
      _s1++, _s2++;
      if (*_s1 != *_s2)
	goto differs;
      _s1++, _s2++;
      n--;
    }

  if (n == 0)
    return 0;

differs:
  return (*_s1 - *_s2);
}

int
bcmp (const void *s1, const void *s2, size_t n)
{
  return memcmp (s1, s2, n);
}
