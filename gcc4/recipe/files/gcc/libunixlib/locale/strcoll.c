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
  return strcoll_l (s1, s2, &__locale_global);
}

int
strcoll_l (const char *s1, const char *s2, locale_t l)
{
  int regs[10];

  regs[0] = l->locale_territory[LC_COLLATE];
  regs[1] = (int)s1;
  regs[2] = (int)s2;
  regs[3] = 0;

  __os_swi (Territory_Collate, regs);
  return regs[0];
}

