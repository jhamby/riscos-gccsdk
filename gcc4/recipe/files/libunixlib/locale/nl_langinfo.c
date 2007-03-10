/* ln_langinfo ()
 * Written by Peter Naulls
 * Copyright (c) 2004-2006 UnixLib Developers
 */

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
