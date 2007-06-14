#include <kernel.h>
#include "module.h"
#include "resfs.h"

_kernel_oserror *ex_initialise(const char *tail, int podule_base, void *pw)
{
	return mkresfs_register();
}

_kernel_oserror *ex_finalise(int fatal, int podule_base, void *pw)
{
        return mkresfs_deregister();
}
