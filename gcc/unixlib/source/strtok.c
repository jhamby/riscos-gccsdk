/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/strtok,v $
 * $Date: 1997/10/09 19:59:43 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strtok,v 1.5 1997/10/09 19:59:43 unixlib Exp $";
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
