/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/locale/nl_langinfo.c,v $
 * $Date: 2004/09/17 18:39:54 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: peter $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: nl_langinfo.c,v 1.1 2004/09/17 18:39:54 peter Exp $";
#endif

#include <locale.h>
#include <nl_types.h>
#include <langinfo.h>

char *
nl_langinfo(nl_item item)
{
  const char *value;

  switch (item)
  {
    case CODESET:
      value = "ISO-8859-1";
      break;

    default:
      value = "";
  }

  return (char *)value;
}

