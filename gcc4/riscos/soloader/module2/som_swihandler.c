/* som_swihandler.c
 *
 * Copyright 2006, 2007 GCCSDK Developers
 */

#include <stdlib.h>
#include "somanager.h"
#include "som.h"
#include "som_register.h"
#include "som_alloc.h"
#include "som_utilswis.h"

_kernel_oserror *
module_swihandler(int number, _kernel_swi_regs *r, void *pw)
{
_kernel_oserror *err = NULL;

  switch (number)
  {
  case (SOM_Prologue - SOM_00):
    err = som_prologue(r);
    break;

  case (SOM_Resolver - SOM_00):
    err = som_resolver(r);
    break;

  case (SOM_Alloc - SOM_00):
    /* On entry: r0 = size required */
    {
    void *block = NULL;

      if ((err = som_alloc(r->r[0], &block)) != NULL)
        break;

      r->r[0] = (int)block;
    }
    break;

  case (SOM_Free - SOM_00):
    /* On entry: r0 = block to free */
    err = som_free((void *)r->r[0]);
    break;

  case (SOM_AllocLib - SOM_00):
    /* On entry: r0 = size required */
    {
    void *block = NULL;

      if ((err = som_alloc_lib(r->r[0], &block)) != NULL)
	break;

      r->r[0] = (int)block;
    }
    break;

  case (SOM_RegisterObject - SOM_00):
    err = som_register_object(r);
    break;

  case (SOM_DeregisterClient - SOM_00):
    err = som_deregister_client(r);
    break;

  case (SOM_DeregisterSharedObject - SOM_00):
    err = som_deregister_shared_object(r);
    break;

  case (SOM_QueryObject - SOM_00):
    err = som_query_object(r);
    break;

  case (SOM_IterateObjects - SOM_00):
    err = som_iterate_objects(r);
    break;

  case (SOM_GOTFromAddr - SOM_00):
    err = som_got_from_addr(r);
    break;

  case (SOM_HandleFromAddr - SOM_00):
    err = som_handle_from_addr(r);
    break;

  case (SOM_HandleFromName - SOM_00):
    err = som_handle_from_name(r);
    break;

  case (SOM_AddrToOffset - SOM_00):
    err = som_addr_to_offset(r);
    break;

  default:
    err = error_BAD_SWI;
    break;
  }

  return err;
}
