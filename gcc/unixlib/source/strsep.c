/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/strsep,v $
 * $Date: 1999/11/16 13:26:49 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strsep,v 1.5 1999/11/16 13:26:49 admin Exp $";
#endif

#include <string.h>

char *
strsep (char **s, const char *delim)
{
  char *begin, *end;

  begin = *s;
  if (! begin)
    return NULL;

  end = strpbrk (begin, delim);
  if (end)
    {
      *end++ = '\0';
      *s = end;
    }
  else
    *s = NULL;

  return begin;
}
