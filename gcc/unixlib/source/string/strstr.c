/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/strstr.c,v $
 * $Date: 2001/01/29 15:10:19 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strstr.c,v 1.2 2001/01/29 15:10:19 admin Exp $";
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
