/* Miscellaneous utility functions. */

#include "som_utils.h"
#include "som_types.h"

static inline int
lcase(char a,
      bool any)
{
  return ((a >= 'A' && a <= 'Z' && any) ? a + 'a' - 'A' : a);
}

/* Compare two strings where s2 may be wildcarded. If lower is true
   then disregard case.
   Return TRUE if a match is found.
   Origin unknown.  */
bool
wildcmp(const char *s1,
	const char *s2,
	bool lower)
{
  char c1, c2;

  do
  {
    c1 = lcase (*s1, lower);
    c2 = lcase (*s2, lower);

    if (c1 == '\0')
      {
        while (*s2 == '*')
          s2++;

        return *s2 == '\0';
      }

    if (c1 == c2 || c2 == '#')
      {
        s1++;
        s2++;
      }
    else if (c2 == '*')
      {
        int ok = false;

        if (s2[1] == '\0')
          return true;

        while (!ok && *s1 != '\0')
         {
           ok = ok || wildcmp (s1, &s2[1], lower);
           s1++;
         }

        return ok;
      }
  } while (c2 != '\0' && (c1 == c2 || c2 == '#'));

  return false;
}
