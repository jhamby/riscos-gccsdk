/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/wchar/c/mbstowcs,v $
 * $Date: 1997/10/30 22:22:46 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: mbstowcs,v 1.1 1997/10/30 22:22:46 unixlib Exp $";
#endif

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
