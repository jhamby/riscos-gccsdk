/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/memcmp,v $
 * $Date: 1997/10/09 19:59:39 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: memcmp,v 1.6 1997/10/09 19:59:39 unixlib Exp $";
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
