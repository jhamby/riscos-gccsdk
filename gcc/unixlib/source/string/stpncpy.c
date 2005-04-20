/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stpncpy.c,v $
 * $Date: 2001/01/29 15:10:18 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: stpncpy.c,v 1.2 2001/01/29 15:10:18 admin Exp $";
#endif

#include <string.h>

char *
stpncpy (char *s, const char *s2, size_t n)
{
  while (n--)
    if (!(*s++ = *s2++))
      {
	char *s1 = s;

	/* Zero fill the rest.  */
	while (n--)
	  *s1++ = 0;

	/* Return a pointer to the *first* null.  */
	return s - 1;
      }

  /* size of s2 is greater than n so return a pointer
     to the character after the last one copied.  */
  return s;
}
