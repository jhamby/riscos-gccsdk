/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/stpncpy,v $
 * $Date: 1997/10/09 19:59:40 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: stpncpy,v 1.2 1997/10/09 19:59:40 unixlib Exp $";
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
