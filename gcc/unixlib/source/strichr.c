/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/strichr,v $
 * $Date: 1997/10/09 19:59:41 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strichr,v 1.6 1997/10/09 19:59:41 unixlib Exp $";
#endif

#include <string.h>
#include <ctype.h>

char *
strichr (register const char *s, register int c)

{
  register int i;

  c = tolower (c);

  do
    {
      i = *s;
      i = tolower (i);
      if (i == c)
	return ((char *) s);
    }
  while (*s++);

  return (0);
}
