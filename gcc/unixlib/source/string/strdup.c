/* strdup ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <stdlib.h>
#include <string.h>
#include <unixlib/unix.h>

char *
strdup (const char *string)
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
