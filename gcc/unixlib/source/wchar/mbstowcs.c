/* mbstowcs ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <stdlib.h>

size_t
mbstowcs (wchar_t *pwcs, const char *s, size_t n)
{
  int count = 0;

  if (n != 0)
    {
      do
	{
	  if ((*pwcs++ = (wchar_t) * s++) == 0)
	    break;
	  count++;
	}
      while (--n != 0);
    }

  return count;
}
