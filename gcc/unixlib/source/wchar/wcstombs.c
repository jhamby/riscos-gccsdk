/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/wchar/c/wcstombs,v $
 * $Date: 1997/10/30 22:22:46 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: wcstombs,v 1.1 1997/10/30 22:22:46 unixlib Exp $";
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
