/* Compatibility functions for the libscl.
   Implementation taken from UnixLib.  */

#include <stdlib.h>
#include <string.h>

char *strdup (const char *string)
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
