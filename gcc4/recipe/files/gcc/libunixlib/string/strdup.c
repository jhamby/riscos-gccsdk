/* strdup ()
 * Copyright (c) 2000-2011 UnixLib Developers
 */

#include <stdlib.h>
#include <string.h>

char *
__strdup (const char *string)
{
  if (!string)
    return NULL;

  size_t len = strlen (string) + 1;
  char *newstr;
  if ((newstr = malloc (len)) == NULL)
    return NULL;
  return memcpy (newstr, string, len);
}
weak_alias(__strdup, strdup)
