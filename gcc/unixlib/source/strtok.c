/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/strtok.c,v $
 * $Date: 2000/07/15 14:52:09 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strtok.c,v 1.1.1.1 2000/07/15 14:52:09 nick Exp $";
#endif

#include <string.h>

char *
strtok (register char *s1, register const char *s2)

{
  static char *s;

  if (!s1)
    {
      if (!s)
	return (0);
      else
	s1 = s;
    }

  s1 += strspn (s1, s2);

  s = strpbrk (s1, s2);

  if (s)
    *s++ = 0;

  return *s1 ? s1 : NULL;
}
