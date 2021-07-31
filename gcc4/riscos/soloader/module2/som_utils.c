/* Miscellaneous utility functions. */

#include "som_utils.h"
#include "som_types.h"
#include "som_alloc.h"
#include <stddef.h>

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

/* Make a C copy of an argument string that may be terminated by any control character.
 * SOM_Alloc is used for memory, the caller is responsible for freeing it.  */
char *copy_arg_string(const char *arg_string)
{
  int len;
  const char *p;

  for (p = arg_string; *p >= 32; p++)
    /* Empty loop */;

  len = p - arg_string;
  if (len == 0)
    return NULL;

  char *cstring;
  if (som_alloc (len + 1, (void **) &cstring) != NULL)
    return NULL;

  char *dst = cstring;

  p = arg_string;
  while (*p >= 32)
    *dst++ = *p++;
  *dst = '\0';

  return cstring;
}
