/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/strncpy.c,v $
 * $Date: 2001/01/29 15:10:19 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strncpy.c,v 1.2 2001/01/29 15:10:19 admin Exp $";
#endif

#include <string.h>

char *
strncpy (char *s, register const char *s2, register size_t n)
{
  register char *s1 = s;

  while (n--)
    if (!(*s1++ = *s2++))
      {
	while (n--)
	  *s1++ = 0;
	return (s);
      }

  return (s);
}
