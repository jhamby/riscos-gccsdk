/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/strrichr,v $
 * $Date: 1997/10/09 19:59:42 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strrichr,v 1.2 1997/10/09 19:59:42 unixlib Exp $";
#endif

#include <string.h>
#include <ctype.h>

char *
strrichr (const char *s, int c)
{
  int i;
  const char *_s;

  c = tolower (c);

  _s = 0;
  do
    {
      i = *s;
      i = tolower (i);
      if (i == c)
	_s = s;
    }
  while (s++, i);

  return ((char *) _s);
}
