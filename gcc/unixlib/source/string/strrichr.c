/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/strrichr.c,v $
 * $Date: 2001/01/29 15:10:19 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strrichr.c,v 1.2 2001/01/29 15:10:19 admin Exp $";
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
