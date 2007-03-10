/* index (), rindex ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <string.h>
#include <strings.h>

char *
index (const char *s, int c)
{
  return strchr (s, c);
}

char *
rindex (const char *s, int c)
{
  return strrchr (s, c);
}
