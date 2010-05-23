#include <stdio.h>
#include <kernel.h>

#include "MdlHdr.h"

_kernel_oserror *
gccmodule_init (const char *tail, int podule_base, void *pw)
{
  printf ("*** gccmodule_init()\n");
  return NULL;
}

_kernel_oserror *
gccmodule_final (int fatal, int podule_base, void *pw)
{
  printf ("*** gccmodule_final()\n");
  return NULL;
}

