/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/string/stpncpy.c,v $
 * $Date: 2005/04/20 17:12:10 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

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
