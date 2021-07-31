/* strcoll ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <locale.h>
#include <string.h>
#include <swis.h>

#include <internal/os.h>

int
strcoll (const char *s1, const char *s2)
{
  int regs[10];

  regs[0] = __locale_territory[LC_COLLATE];
  regs[1] = (int)s1;
  regs[2] = (int)s2;
  regs[3] = 0;

  __os_swi (Territory_Collate, regs);
  return regs[0];
}
