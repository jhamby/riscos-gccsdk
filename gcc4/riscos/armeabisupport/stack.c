/* stack.c
 *
 * Copyright 2019 GCCSDK Developers
 * Written by Lee Noar
 */

#include <string.h>
#include <stdio.h>
#include "armeabisupport.h"
#include "stack.h"
#include "memory.h"
#include "abort.h"
#include "main.h"
#include "swi.h"
#include "types.h"

//#define NO_STACK_ABORT_HANDLER 1

static const char *default_stack_name = "<unnamed>";
static const char *stack_da_name = "UnixLib stacks";

/* r0 = reason
*   0 - Alloc
*     Entry:
*	R1 = Size of stack to allocate in pages
*	R2 = Number of guard pages
*	R3 = Ptr to name of stack (max 32 chars inc. term., can be NULL)
*     Exit:
*	R0 = handle of stack (V clear), or error ptr (V Set)
*	R1 = Base of stack (above any guard pages)
*	R2 = Top of stack (where register SP starts initially).
*   1 - Free
*     Entry:
*       R1 = handle of stack to free
*   2 - Get Stack
*     Entry:
*       R1 = address within stack
*     Exit:
*       R1 = handle of stack
*   3 - Get Bounds
*     Entry:
*       R1 = stack handle
*     Exit:
*       R1 = Base of stack (above any guard pages).
*       R2 = Top of stack (where register SP starts initially).
*   4 - Get Sizes
*     Entry:
*       R1 = stack handle
*     Exit:
*	R1 = Size of given stack (excluding any guard pages), in bytes, or 0 if not found.
*	R2 = Size of stack guard in bytes (may be 0).
*/

_kernel_oserror *
stack_op(_kernel_swi_regs *r)
{
  _kernel_oserror *err = NULL;
  switch (r->r[0])
  {
    case STACKOP_ALLOC: {
      stack_object *handle;
      eabi_PTR base, top;

      err = stack_alloc((unsigned)r->r[1], (unsigned)r->r[2], (const char *)r->r[3],
			&handle, &base, &top);
      if (err)
	r->r[0] = r->r[1] = r->r[2] = 0;
      else
      {
	r->r[0] = (int)handle;
	r->r[1] = (int)base;
	r->r[2] = (int)top;
      }
      break;
    }
    case STACKOP_FREE:
      err = stack_free((stack_object *)r->r[1]);
      break;
    case STACKOP_GET_STACK: {
      stack_object *handle;

      err = stack_get_stack((eabi_PTR)r->r[1], (void *)&handle);
      r->r[1] = (err == NULL) ? (int)handle : 0;
      break;
    }
    case STACKOP_GET_BOUNDS: {
      stack_object *stack;
      
      if ((stack = (stack_object *)r->r[1]) == NULL)
      {
	r->r[1] = r->r[2] = 0;
	return armeabisupport_bad_param;
      }

      r->r[1] = (int)stack->base + (stack->guard_size << PAGE_SHIFT);
      r->r[2] = (int)stack->top;

      break;
    }
    case STACKOP_GET_SIZE: {
      stack_object *stack;
      
      if ((stack = (stack_object *)r->r[1]) == NULL)
      {
	r->r[1] = r->r[2] = 0;
	return armeabisupport_bad_param;
      }

      r->r[1] = (int)(stack->size << PAGE_SHIFT);
      r->r[2] = (int)(stack->guard_size << PAGE_SHIFT);

      break;
    }
    default:
      err = armeabisupport_bad_reason;
      break;
  }

  return err;
}

_kernel_oserror *
stack_alloc(unsigned size, unsigned num_guard_pages, const char *name,
	    stack_object **handle_ret, eabi_PTR *base_ret, eabi_PTR *top_ret)
{
  if (size == 0)
    return armeabisupport_bad_param;

  _kernel_oserror *err;
  armeabisupport_allocator *allocator;

  if (global.stack_allocator == NULL)
  {
    err = allocator_new_internal(stack_da_name,
				 STACK_DA_MAX_SIZE,
				 ALLOCATOR_TYPE_PAGE | ALLOCATOR_FLAG_GLOBAL,
				 &allocator);
    if (err)
      return err;
    abort_block *block = get_data_abort_support_block();
    block->stack_allocator = global.stack_allocator = allocator;
  }
  else
    allocator = global.stack_allocator;

  eabi_PTR block;
  unsigned num_pages_required = size + num_guard_pages;
  err = allocator_alloc(allocator, num_pages_required, ALLOCATOR_ALLOC_FLAG_ALLOC_ONLY, (eabi_PTR *)&block);
  if (err)
    return err;

#ifdef NO_STACK_ABORT_HANDLER
  err = allocator_claim_pages(allocator, block + (num_guard_pages << PAGE_SHIFT), size);
  if (err)
    goto error__alloced;

  err = allocator_map_pages(allocator, block + (num_guard_pages << PAGE_SHIFT), size, PMP_MEM_ACCESS_RWE_RWE);
  if (err)
    goto error__claimed;
#else
  /* Just claim/map the page at the top of the stack.  */
  err = allocator_claim_pages(allocator, block + ((num_pages_required - 1) << PAGE_SHIFT), 1);
  if (err)
    goto error__alloced;

  err = allocator_map_pages(allocator, block + ((num_pages_required - 1) << PAGE_SHIFT), 1, PMP_MEM_ACCESS_RWE_RWE);
  if (err)
    goto error__claimed;
#endif

  app_object *app;
  if ((err = app_find(&app)) != NULL)
    err = app_new(&app);

  if (err)
    goto error__mapped;

  stack_object *stack;
  err = rma_claim(sizeof(*stack), (void **)&stack);
  if (err)
    goto error__mapped;

  stack->base = block;
  stack->top = block + (num_pages_required << PAGE_SHIFT);
  stack->size = size;
  stack->guard_size = num_guard_pages;

  if (!name)
    name = default_stack_name;

  strncpy(stack->name, name, STACK_NAME_MAX_LEN);
  stack->name[STACK_NAME_MAX_LEN - 1] = '\0';

  linklist_add_to_end(&app->stack_list, &stack->link);

  abort_block *abort = get_data_abort_support_block();
  abort->first_stack = linklist_first_stack_object(&app->stack_list);

  *handle_ret = stack;
  *base_ret = block + (num_guard_pages << PAGE_SHIFT);
  *top_ret = block + ((size + num_guard_pages) << PAGE_SHIFT);

  return NULL;

error__mapped:
  allocator_unmap_pages(allocator, block + ((num_pages_required - 1) << PAGE_SHIFT), 1);
error__claimed:
  allocator_release_pages(allocator, block + ((num_pages_required - 1) << PAGE_SHIFT), 1);
error__alloced:
  allocator_free(allocator, block);

  *handle_ret = 0;
  *base_ret = 0;
  *top_ret = 0;

  return err;
}

_kernel_oserror *
stack_free_internal(stack_object *stack, app_object *app)
{
  armeabisupport_allocator *allocator = global.stack_allocator;

  allocator_free(allocator, stack->base);
  linklist_remove(&app->stack_list, &stack->link);
  rma_free(stack);

  /* Reset the abort handler support block in case we free'd the first stack.  */
  abort_block *abort = get_data_abort_support_block();
  abort->first_stack = linklist_first_stack_object(&app->stack_list);

  return NULL;
}

_kernel_oserror *
stack_free(stack_object *stack)
{
  _kernel_oserror *err;
  app_object *app;

  if ((err = app_find(&app)) != NULL)
    return err;

  return stack_free_internal(stack, app);
}

stack_object *
find_stack(link_list *stack_list, eabi_PTR addr)
{
  stack_object *stack;
  for (stack = linklist_first_stack_object(stack_list);
       stack && (addr < stack->base || addr >= stack->top);
       stack = linklist_next_stack_object(stack))
    /* Empty loop.  */;

  return stack;
}

_kernel_oserror *
stack_get_stack(eabi_PTR addr, stack_object **stack_ret)
{
  if (!addr)
    return armeabisupport_bad_param;

  _kernel_oserror *err;
  app_object *app;

  if ((err = app_find(&app)) != NULL)
    return err;

  stack_object *stack;
  if ((stack = find_stack(&app->stack_list, addr)) == NULL)
    return armeabisupport_bad_param;

  *stack_ret = stack;

  return NULL;
}
