/* abort.c
 *
 * Copyright (c) 2019 GCCSDK Developers
 * Written by Lee Noar
 */

#include "abort.h"
#include "swi.h"
#include "types.h"
#include "main.h"

#define DEBUG_PRINT 0
#define DEBUG_REPORT 0

#include "debug.h"

/* r0 = reason code:
*	0 - register abort handler
*	 Entry:
*	  r1 = ptr to abort handler routine
*	  r2 = data to be passed in r12 to routine
*	 Exit:
*	  r0 = error pointer if failure (V set) or 0 (V clear)
*	1 - deregister abort handler
*	 Entry:
*	  r1 = ptr to abort handler routine
*	  r2 = data to be passed in r12 to routine
*	 Exit:
*	  r0 = error pointer if failure (V set) or 0 (V clear)
*	2 - install system abort handler
*	 Entry:
*	  None
*	 Exit:
*	  r0 = error pointer if failure (V set) or 0 (V clear)
*	3 - remove system abort handler
*	 Entry:
*	  None
*	 Exit:
*	  r0 = error pointer if failure (V set) or 0 (V clear)
*/

_kernel_oserror *
abort_op(_kernel_swi_regs *r)
{
  _kernel_oserror *err = NULL;
  int reason = r->r[0];

  switch (reason)
  {
    case ABORTOP_REGISTER: {
      err = abort_register((void *)r->r[1], (unsigned)r->r[2]);
      break;
    }
    case ABORTOP_DEREGISTER: {
      err = abort_deregister((void *)r->r[1], (unsigned)r->r[2]);
      break;
    }
    case ABORTOP_INSTALL: {
      err = abort_install();
      break;
    }
    case ABORTOP_REMOVE: {
      err = abort_remove();
      break;
    }
  }

  if (err)
  {
    DEBUG_OUTPUT("%s - AbortOp reason %d",err->errmess,reason);
  }

  return err;
}

_kernel_oserror *
abort_register_internal(app_object *app, void *routine, unsigned r12)
{
  if (!app)
    return armeabisupport_bad_param;

  _kernel_oserror *err;
  abort_handler *handler;
  err = rma_claim(sizeof(*handler), (void **)&handler);
  if (err)
    return err;

  handler->routine = routine;
  handler->r12 = r12;
  linklist_add_to_end(&app->abort_handler_list, &handler->link);

  abort_block *block = get_data_abort_support_block();
  block->first_abort = linklist_first_abort_handler(&app->abort_handler_list);

  return NULL;
}

_kernel_oserror *
abort_register(void *routine, unsigned r12)
{
  if (!routine)
    return armeabisupport_bad_param;
  
  app_object *app;
  _kernel_oserror *err = app_find(&app);
  if (err)
    err = app_new(&app);

  if (err)
    return err;

  return abort_register_internal(app, routine, r12);
}

_kernel_oserror *
abort_deregister(void *routine, unsigned r12)
{
  if (!routine)
    return armeabisupport_bad_param;
  
  app_object *app;
  _kernel_oserror *err = app_find(&app);
  if (err)
    return err;

  abort_handler *handler;
  for (handler = linklist_first_abort_handler(&app->abort_handler_list);
       handler != NULL && handler->routine != routine && handler->r12 != r12;
       handler = linklist_next_abort_handler(handler))
    /* Empty loop */;

  if (!handler)
    return armeabisupport_bad_param;

  linklist_remove(&app->abort_handler_list, &handler->link);

  rma_free(handler);

  return NULL;
}
