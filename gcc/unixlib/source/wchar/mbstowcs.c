/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/wchar/mbstowcs.c,v $
 * $Date: 2000/07/15 14:52:46 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: mbstowcs.c,v 1.1.1.1 2000/07/15 14:52:46 nick Exp $";
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
