/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/strtok_r,v $
 * $Date: 1997/10/09 19:59:43 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strtok_r,v 1.3 1997/10/09 19:59:43 unixlib Exp $";
#endif

#include <string.h>

char *
strtok_r (char *s, const char *delim, char **save_ptr)
{
  char *token;

  if (s == NULL)
    s = *save_ptr;

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
