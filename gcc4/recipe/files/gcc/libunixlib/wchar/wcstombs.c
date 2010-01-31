/* wcstombs ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <stdlib.h>

size_t
wcstombs (char *s, const wchar_t *pwcs, size_t n)
{
  int count = 0;

  if (n != 0)
    {
      do
	{
	  if ((*s++ = (char) *pwcs++) == 0)
	    break;
	  count++;
	}
      while (--n != 0);
    }

  return count;
}
