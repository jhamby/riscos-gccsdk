/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/strcspn,v $
 * $Date: 1999/02/07 20:52:39 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strcspn,v 1.3 1999/02/07 20:52:39 unixlib Exp $";
#endif

#include <string.h>

size_t
strcspn (const char *s1, const char *s)
{
  int c1, c2;
  const char *s2;
  size_t n = 0;

  while ((c1 = *s1++))
    {
      s2 = s;
      while ((c2 = *s2++))
	if (c2 == c1)
	  return n;
      n++;
    }

  return n;
}
