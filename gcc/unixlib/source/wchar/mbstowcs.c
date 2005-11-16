/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/wchar/mbstowcs.c,v $
 * $Date: 2001/01/29 15:10:22 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

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
