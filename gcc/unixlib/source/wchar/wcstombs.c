/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/wchar/wcstombs.c,v $
 * $Date: 2001/01/29 15:10:22 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

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
