/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/strpbrk.c,v $
 * $Date: 2000/07/15 14:52:09 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strpbrk.c,v 1.1.1.1 2000/07/15 14:52:09 nick Exp $";
#endif

#include <string.h>

char *
strpbrk (const char *s1, const char *s)
{
  int c1, c2;
  const char *s2;

  while ((c1 = *s1))
    {
      s2 = s;
      while ((c2 = *s2++))
	if (c2 == c1)
	  return (char *) s1;
      s1++;
    }

  return NULL;
}
