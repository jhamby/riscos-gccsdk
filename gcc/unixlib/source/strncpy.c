/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/strncpy,v $
 * $Date: 1997/10/09 19:59:41 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strncpy,v 1.2 1997/10/09 19:59:41 unixlib Exp $";
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
