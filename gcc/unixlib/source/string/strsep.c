/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/strsep.c,v $
 * $Date: 2001/01/29 15:10:19 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strsep.c,v 1.2 2001/01/29 15:10:19 admin Exp $";
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
