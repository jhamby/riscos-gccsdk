/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/strncpy.c,v $
 * $Date: 2000/07/15 14:52:09 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strncpy.c,v 1.1.1.1 2000/07/15 14:52:09 nick Exp $";
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
