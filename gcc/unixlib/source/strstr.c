/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/strstr,v $
 * $Date: 1997/10/09 19:59:42 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strstr,v 1.4 1997/10/09 19:59:42 unixlib Exp $";
#endif

#include <string.h>

char *
strstr (register const char *s1, register const char *s2)

{
  register int l1 = strlen (s1), l2 = strlen (s2);
  register const char *e1 = s1 + l1 - l2;

  while (s1 <= e1)
    {
      if (!strncmp (s1, s2, l2))
	return ((char *) s1);
      s1++;
    }

  return (0);
}
