/* strdup ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <stdlib.h>
#include <string.h>
#include <sys/cdefs.h>

char *
__strdup (const char *string)
{
  int len;
  char *newstr;

  if (!string)
    return 0;

  len = strlen (string) + 1;
  if ((newstr = malloc (len)) == NULL)
    return NULL;
  return memcpy (newstr, string, len);
}
weak_alias(__strdup, strdup);
