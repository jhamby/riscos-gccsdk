/****************************************************************************
 *
 * $Source$
 * $Date$
 * $Revision$
 * $State$
 * $Author$
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id$";
#endif

#include <string.h>

/* Defined by POSIX as not threadsafe */ 
char *
strtok (register char *s1, register const char *s2)

{
  static char *s;

  if (!s1)
    {
      if (!s)
	return (0);
      else
	s1 = s;
    }

  s1 += strspn (s1, s2);

  s = strpbrk (s1, s2);

  if (s)
    *s++ = 0;

  return *s1 ? s1 : NULL;
}
