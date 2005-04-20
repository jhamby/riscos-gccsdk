/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/strichr.c,v $
 * $Date: 2001/01/29 15:10:19 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strichr.c,v 1.2 2001/01/29 15:10:19 admin Exp $";
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
