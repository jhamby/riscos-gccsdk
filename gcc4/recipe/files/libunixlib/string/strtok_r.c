/* String tokeniser (reentrant version).
   Copyright (c) 2002, 2003, 2004, 2005, UnixLib Developers.  */

#include <string.h>

char *
strtok_r (char *s, const char *delim, char **save_ptr)
{
  char *token;

  if (s == NULL && (s = *save_ptr) == NULL)
    return NULL;

  s += strspn (s, delim);
  if (*s == '\0')
    return NULL;

  token = s;
  s = strpbrk (token, delim);
  if (s)
    {
      *s = 0;
      *save_ptr = s + 1;
    }
  else
    *save_ptr = strchr (token, '\0');

  return token;
}
