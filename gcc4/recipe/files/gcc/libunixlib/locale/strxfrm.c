/* strxfrm ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <locale.h>
#include <string.h>
#include <swis.h>

#include <internal/os.h>

size_t
strxfrm (char *to, const char *from, size_t size)
{
  return strxfrm_l (to, from, size, &__locale_global);
}

size_t
strxfrm_l (char *to, const char *from, size_t size, locale_t l)
{
  int regs[10];

  regs[0] = l->locale_territory[LC_COLLATE];
  regs[1] = (int)to;
  regs[2] = (int)from;
  regs[3] = size;

  __os_swi (Territory_TransformString, regs);
  return regs[0];
}
