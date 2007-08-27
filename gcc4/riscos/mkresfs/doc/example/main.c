#include <kernel.h>
#include "module.h"
#include "resfs.h"

_kernel_oserror *
ex_initialise (const char *tail, int podule_base, void *pw)
{
  return mkresfs_register ();
}

_kernel_oserror *
ex_finalise (int fatal, int podule_base, void *pw)
{
  return mkresfs_deregister ();
}

void
ex_service (int service, _kernel_swi_regs *r, void *pw)
{
  switch (service)
    {
      case 0x60: /* Service_ResourceFSStarting */
        mkresfs_reregister (r->r[2], r->r[3]);
        break;
    }
}
