/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/strspn,v $
 * $Date: 1999/02/07 20:52:42 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strspn,v 1.6 1999/02/07 20:52:42 unixlib Exp $";
#endif

#include <string.h>

size_t
strspn (const char *s1, const char *s)
{
  int c1, c2;
  const char *s2;
  size_t n = 0;

  while ((c1 = *s1++))
    {
      s2 = s;
      while ((c2 = *s2++) != c1)
	if (!c2)
	  return n;
      n++;
    }

  return n;
}
