/* UnixLib getchar() and getchar_unlocked implementation.
   Copyright 2000-2011 UnixLib Developers.  */

#include <stdio.h>

#undef getchar
#undef getchar_unlocked

int
getchar (void)
{
  return fgetc (stdin);
}

int
getchar_unlocked (void)
{
  return fgetc_unlocked (stdin);
}
