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
  int regs[10];

  regs[0] = __locale_territory[LC_COLLATE];
  regs[1] = (int)to;
  regs[2] = (int)from;
  regs[3] = size;

  __os_swi (Territory_TransformString, regs);
  return regs[0];
}
