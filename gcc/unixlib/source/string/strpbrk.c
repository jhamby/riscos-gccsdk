/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/strpbrk.c,v $
 * $Date: 2001/01/29 15:10:19 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strpbrk.c,v 1.2 2001/01/29 15:10:19 admin Exp $";
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
