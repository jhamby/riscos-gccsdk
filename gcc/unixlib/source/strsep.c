/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/strsep.c,v $
 * $Date: 2000/07/15 14:52:09 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strsep.c,v 1.1.1.1 2000/07/15 14:52:09 nick Exp $";
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
