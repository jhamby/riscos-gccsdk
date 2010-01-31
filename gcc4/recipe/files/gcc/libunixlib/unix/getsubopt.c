/* getsubopt ()
 * Copyright (c) 2000-2007 UnixLib Developers
 */

#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int
getsubopt (char **optionp, const char *const *tokens, char **valuep)
{
  char *end, *value;
  char buf[64];
  int x;

  if (optionp == NULL || *optionp == NULL || **optionp == '\0')
    return __set_errno (EINVAL);

  /* Find option terminator.  This will be either a comma or a null.  */
  end = strchr (*optionp, ',');
  if (end == NULL)
    end = strchr (*optionp, '\0');

  /* Find start of value.  */
  value = strchr (*optionp, '=');
  if (value == NULL || value >= end)
    value = end;

  /* Copy the token into a temporary buffer.  */
  memcpy (buf, *optionp, value - *optionp);
  buf[value - *optionp] = '\0';

  if (*end != '\0')
    *end++ = '\0';

  /* Try to match a token against the characters between *optionp
     and value.  */
  for (x = 0; tokens[x] != NULL; x++)
    if (strcmp (buf, tokens[x]) == 0)
      {
        if (value != end)
          *valuep = value + 1;
        else
          *valuep = NULL;

        *optionp = end;

        return x;
      }

  /* The suboption does not match.  */
  *valuep = *optionp;
  *optionp = end;
  return -1;
}
