/* UnixLib putchar() and putchar_unlocked implementation.
   Copyright 2002-2011 UnixLib Developers.  */

#include <stdio.h>

int
putchar (int c)
{
  return fputc (c, stdout);
}

int
putchar_unlocked (int c)
{
  return fputc_unlocked (c, stdout);
}

