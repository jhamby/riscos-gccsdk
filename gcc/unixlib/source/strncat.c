/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/strncat,v $
 * $Date: 1999/02/07 20:52:40 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strncat,v 1.3 1999/02/07 20:52:40 unixlib Exp $";
#endif

#include <string.h>

char *
strncat (char *s, const char *s2, register size_t n)
{
  char *s1 = s;

  while (*s1++)
    ;
  --s1;

  while ((*s1++ = *s2++))
    if (!(n--))
      {
	*--s1 = 0;
	break;
      }

  return s;
}
