/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/wchar/wcstombs.c,v $
 * $Date: 2000/07/15 14:52:47 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: wcstombs.c,v 1.1.1.1 2000/07/15 14:52:47 nick Exp $";
#endif

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
